package com.dragonbones

import android.content.Context
import android.graphics.PixelFormat
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.util.Log
import android.view.GestureDetector
import android.view.MotionEvent
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.viewinterop.AndroidView
import java.io.File
import java.io.IOException
import java.lang.ref.WeakReference
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10
import kotlinx.coroutines.launch

private const val TAG = "DragonBonesView"

/** DragonBones 骨骼动画视图组件 使用 C++ JNI 和 OpenGL ES 进行渲染，以获得高性能 */
class DragonBonesView
@JvmOverloads
constructor(context: Context, attrs: AttributeSet? = null, zOrderOnTop: Boolean = true) :
        GLSurfaceView(context, attrs) {

    private val renderer: DragonBonesRenderer
    var onSlotTapListener: ((String) -> Unit)? = null
    var onRenderStart: (() -> Unit)? = null
    var onRenderFrame: (() -> Unit)? = null
    var onRenderEnd: (() -> Unit)? = null

    companion object {
        private var activeInstance: WeakReference<DragonBonesView>? = null

        fun isInstanceActive(): Boolean {
            return activeInstance?.get() != null
        }
    }

    init {
        if (isInstanceActive()) {
            throw IllegalStateException(
                    "Only one DragonBonesView instance can be active at a time."
            )
        }

        JniBridge.init()

        // 1. 设置EGL配置以支持Alpha通道，实现透明背景
        setEGLConfigChooser(8, 8, 8, 8, 16, 0)
        holder.setFormat(PixelFormat.TRANSLUCENT)

        // 2. 设置OpenGL ES 2.0上下文
        setEGLContextClientVersion(2)

        // 3. 创建并设置渲染器
        renderer =
                DragonBonesRenderer(
                        onSurfaceCreated = { onRenderStart?.invoke() },
                        onDrawFrame = { onRenderFrame?.invoke() },
                        onSurfaceDestroyed = { onRenderEnd?.invoke() }
                )
        setRenderer(renderer)

        // 4. 设置渲染模式为连续渲染，以驱动动画
        renderMode = RENDERMODE_CONTINUOUSLY

        // 5. 将视图的背景设置为透明，并置于顶层
        setBackgroundColor(0x00000000)
        setZOrderOnTop(zOrderOnTop)

        activeInstance = WeakReference(this)

        val gestureDetector =
                GestureDetector(
                        context,
                        object : GestureDetector.SimpleOnGestureListener() {
                            override fun onSingleTapUp(e: MotionEvent): Boolean {
                                onSlotTapListener?.let { listener ->
                                    queueEvent {
                                        val slotName = JniBridge.containsPoint(e.x, e.y)
                                        if (slotName != null) {
                                            post { // run on UI thread
                                                listener(slotName)
                                            }
                                        }
                                    }
                                }
                                return true
                            }
                        }
                )

        setOnTouchListener { _, event ->
            // Let the gesture detector handle the event.
            // Return true if the event was consumed, false otherwise.
            // This allows unconsumed events to be passed up to Compose's pointer input handlers.
            gestureDetector.onTouchEvent(event)
        }
    }

    /** GL渲染器，负责调用JNI代码执行实际的OpenGL绘制 */
    private class DragonBonesRenderer(
            private val onSurfaceCreated: () -> Unit,
            private val onDrawFrame: () -> Unit,
            private val onSurfaceDestroyed: () -> Unit
    ) : Renderer {
        override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
            JniBridge.onSurfaceCreated()
            onSurfaceCreated.invoke()
        }

        override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
            JniBridge.onSurfaceChanged(width, height)
        }

        override fun onDrawFrame(gl: GL10?) {
            JniBridge.onDrawFrame()
            onDrawFrame.invoke()
        }
    }

    private fun readBytesFromPath(path: String): ByteArray {
        return try {
            if (path.startsWith("/")) { // Absolute file path
                File(path).readBytes()
            } else { // Asset path
                context.assets.open(path).readBytes()
            }
        } catch (e: IOException) {
            throw IOException("Failed to read data from path: $path", e)
        }
    }

    fun loadModel(model: DragonBonesModel, onComplete: (() -> Unit)? = null) {
        try {
            Log.d(TAG, "loadModel: Reading data for ${model.skeletonPath}")
            val skeletonData = readBytesFromPath(model.skeletonPath)
            val textureJsonData = readBytesFromPath(model.textureJsonPath)
            val textureImageData = readBytesFromPath(model.textureImagePath)
            queueEvent {
                Log.d(TAG, "loadModel: Queuing JNI call for ${model.skeletonPath}")
                JniBridge.loadDragonBones(skeletonData, textureJsonData, textureImageData)
                onComplete?.let { post(it) }
            }
        } catch (e: IOException) {
            // Forward exception to be handled by the caller, e.g., in Compose
            throw e
        }
    }

    override fun onPause() {
        queueEvent {
            super.onPause()
            JniBridge.onPause()
        }
    }

    override fun onResume() {
        queueEvent {
            super.onResume()
            JniBridge.onResume()
        }
    }

    fun destroy() {
        Log.d(TAG, "destroy() called on view $this")
        queueEvent {
            JniBridge.onDestroy()
            onRenderEnd?.invoke()
        }
        if (activeInstance?.get() == this) {
            activeInstance?.clear()
        }
    }

    /**
     * 异步获取当前模型所有可播放的动画名称列表。
     *
     * @param callback 回调函数，将在UI线程上接收一个包含所有动画名称的列表。
     * ```
     *                 如果模型未加载或没有动画，将返回一个空列表。
     * ```
     */
    fun getAnimationNames(callback: (List<String>) -> Unit) {
        queueEvent {
            val names = JniBridge.getAnimationNames()?.toList() ?: emptyList()
            post { // 确保回调在UI线程上执行
                callback(names)
            }
        }
    }

    /**
     * 播放指定名称的动画。 如果动画名称不存在，将不会播放任何动画（并会在logcat中打印警告）。
     *
     * @param name 要播放的动画的名称。
     */
    fun fadeInAnimation(name: String, layer: Int, loop: Int, fadeInTime: Float) {
        Log.d(TAG, "JNI Call: Queueing fadeInAnimation for '$name' on layer $layer")
        queueEvent { JniBridge.fadeInAnimation(name, layer, loop, fadeInTime) }
    }

    fun stopAnimation(name: String) {
        Log.d(TAG, "JNI Call: Queueing stopAnimation for '$name'")
        queueEvent { JniBridge.stopAnimation(name) }
    }

    fun setWorldScale(scale: Float) {
        queueEvent { JniBridge.setWorldScale(scale) }
    }

    fun setWorldTranslation(x: Float, y: Float) {
        queueEvent { JniBridge.setWorldTranslation(x, y) }
    }

    fun overrideBonePosition(boneName: String, x: Float, y: Float) {
        queueEvent { JniBridge.overrideBonePosition(boneName, x, y) }
    }

    fun resetBone(boneName: String) {
        Log.d("DragonBonesView", "Queueing resetBone: $boneName")
        queueEvent { JniBridge.resetBone(boneName) }
    }
}

/**
 * DragonBones 模型信息数据类
 * @param skeletonPath DragonBones骨骼数据（.json文件）的路径，可以是assets相对路径或设备绝对路径。
 * @param textureJsonPath 纹理图集数据（.json文件）的路径。
 * @param textureImagePath 纹理图集图片（.png文件）的路径。
 */
data class DragonBonesModel(
        val skeletonPath: String,
        val textureJsonPath: String,
        val textureImagePath: String
)

@Composable
fun DragonBonesViewCompose(
        modifier: Modifier = Modifier,
        model: DragonBonesModel?,
        controller: DragonBonesController,
        zOrderOnTop: Boolean = true,
        onError: (String) -> Unit = {},
        onRenderStart: () -> Unit = {},
        onRenderFrame: () -> Unit = {},
        onRenderEnd: () -> Unit = {}
) {
    var creationError by remember { mutableStateOf<String?>(null) }
    val context = LocalContext.current

    // The view instance is now stable and will not be recreated.
    val viewInstance = remember {
        try {
            DragonBonesView(context, zOrderOnTop = zOrderOnTop).apply {
                this.onRenderStart = onRenderStart
                this.onRenderFrame = onRenderFrame
                this.onRenderEnd = onRenderEnd
            }
        } catch (e: IllegalStateException) {
            creationError = e.message ?: "Failed to create DragonBonesView"
            null
        }
    }

    if (viewInstance != null) {
        // This effect now triggers whenever the model object changes.
        // It loads the new model data into the existing, stable view.
        LaunchedEffect(viewInstance, model) {
            if (model != null) {
                Log.d(TAG, "LaunchedEffect for model change. New model: ${model.skeletonPath}")
                try {
                    // CRITICAL: Immediately clear animation names to prevent effects
                    // from using stale data on the new model.
                    controller.setAnimationNames(emptyList())
                    // IMPORTANT: Clear any pending animation commands from the previous model.
                    controller.clearAnimationQueue()

                    // Load the model and then fetch the animation names for the new model.
                    // This guarantees a sequential, race-free state update.
                    // CRUCIALLY, we use the controller's stable scope, which is not
                    // cancelled when this LaunchedEffect is restarted.
                    viewInstance.loadModel(model) {
                        Log.d(
                                TAG,
                                "loadModel onComplete: Fetching animations for ${model.skeletonPath}"
                        )
                        controller.coroutineScope.launch { controller.fetchAnimationNames() }
                    }
                } catch (e: Exception) {
                    Log.e(TAG, "Error in model change effect", e)
                    onError("Failed to load model: ${e.message}")
                }
            }
        }

        // Update render callbacks when they change
        LaunchedEffect(onRenderStart, onRenderFrame, onRenderEnd) {
            viewInstance.onRenderStart = onRenderStart
            viewInstance.onRenderFrame = onRenderFrame
            viewInstance.onRenderEnd = onRenderEnd
        }

        // This effect runs only once to associate the view with the controller.
        LaunchedEffect(viewInstance) { controller.setView(viewInstance) }

        // This robust, single-instance effect handles the command queue processing.
        // It's decoupled from recomposition and safe from race conditions.
        LaunchedEffect(viewInstance, controller) {
            while (true) {
                controller.consumeNextAnimationCommand()?.let { command ->
                    when (command) {
                        is FadeInAnimationCommand -> {
                            viewInstance.fadeInAnimation(
                                    name = command.name,
                                    layer = command.layer,
                                    loop = command.loop,
                                    fadeInTime = command.fadeInTime
                            )
                        }
                        is StopAnimationCommand -> {
                            viewInstance.stopAnimation(command.name)
                        }
                    }
                }
                kotlinx.coroutines.delay(16) // Check for new commands roughly every frame
            }
        }

        LaunchedEffect(controller.scale) { viewInstance?.setWorldScale(controller.scale) }

        LaunchedEffect(controller.translationX, controller.translationY) {
            viewInstance?.setWorldTranslation(controller.translationX, controller.translationY)
        }

        // The view is disposed only when the composable leaves the screen entirely.
        DisposableEffect(viewInstance) { onDispose { controller.destroyView() } }

        AndroidView(factory = { viewInstance }, modifier = modifier)
    } else if (creationError != null) {
        LaunchedEffect(creationError) { onError(creationError!!) }
    }
}
