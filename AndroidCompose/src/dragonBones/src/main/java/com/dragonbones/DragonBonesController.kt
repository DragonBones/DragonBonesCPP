package com.dragonbones

import androidx.compose.runtime.Composable
import androidx.compose.runtime.Stable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateListOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import android.util.Log
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.launch
import kotlinx.coroutines.suspendCancellableCoroutine
import kotlin.coroutines.resume

private const val TAG = "DBController"

/**
 * A controller for the [DragonBonesViewCompose] composable.
 *
 * This controller provides a way to interact with the DragonBones view, such as playing animations
 * and retrieving the list of available animations.
 *
 * @see rememberDragonBonesController
 */
@Stable
class DragonBonesController(val coroutineScope: CoroutineScope) {

    private var view: DragonBonesView? = null

    /**
     * The list of available animation names for the current model. This state is updated
     * automatically when a new model is loaded.
     */
    var animationNames by mutableStateOf<List<String>>(emptyList())
        private set

    internal val animationCommandQueue = mutableStateListOf<AnimationCommand>()

    /** The overall scale of the armature. Default is 0.5f. */
    var scale by mutableStateOf(0.5f)

    /** The horizontal translation of the armature from the center. */
    var translationX by mutableStateOf(0.0f)

    /** The vertical translation of the armature from the center. */
    var translationY by mutableStateOf(0.0f)

    /**
     * A callback that is invoked when a slot is tapped. The string parameter is the name of the
     * tapped slot.
     */
    var onSlotTap: ((String) -> Unit)? = null

    /** Destroys the currently associated view, if any. */
    fun destroyView() {
        Log.d(TAG, "destroyView: Destroying view $view")
        view?.destroy()
        view = null
    }

    /**
     * Asynchronously fetches the animation names from the current view and updates the state.
     * This function suspends until the names are retrieved.
     */
    suspend fun fetchAnimationNames() {
        Log.d(TAG, "fetchAnimationNames: Starting...")
        suspendCancellableCoroutine<Unit> { continuation ->
            val v = view
            if (v == null) {
                Log.w(TAG, "fetchAnimationNames: View is null, setting names to empty.")
                animationNames = emptyList()
                if (continuation.isActive) {
                    continuation.resume(Unit)
                }
                return@suspendCancellableCoroutine
            }

            v.getAnimationNames { names ->
                Log.d(TAG, "fetchAnimationNames: Completed. Names: ${names.joinToString(", ")}")
                animationNames = names
                if (continuation.isActive) {
                    continuation.resume(Unit)
                }
            }
        }
    }

    internal fun setView(dragonBonesView: DragonBonesView?) {
        // If there's already a view, and we're setting a new one (or null), destroy the old one.
        if (view != null && view != dragonBonesView) {
            Log.d(TAG, "setView: Destroying old view during setView.")
            destroyView()
        }
        Log.d(TAG, "setView: Setting view to $dragonBonesView")
        view = dragonBonesView
        if (dragonBonesView != null) {
            dragonBonesView.onSlotTapListener = { slotName -> onSlotTap?.invoke(slotName) }
            // Fetch names when the view is initially set.
            coroutineScope.launch {
                fetchAnimationNames()
            }
        } else {
            // If the view is set to null, clear the animation names.
            animationNames = emptyList()
        }
        // Do NOT clear animationNames here, as it would cause the UI to flicker during recomposition.
    }

    /**
     * Plays an animation on a specific track (layer) with looping and fade-in control.
     * This is the recommended method for playing animations.
     *
     * @param name The name of the animation to play.
     * @param layer The track index to play the animation on. Animations on higher layers will be mixed on top.
     * @param loop The number of times to loop. 0 means infinite loop, 1 means play once, etc.
     * @param fadeInTime The time in seconds to fade in the animation.
     */
    fun fadeInAnimation(name: String, layer: Int, loop: Int, fadeInTime: Float = 0.3f) {
        Log.d(TAG, "fadeInAnimation: Adding command for '$name' on layer $layer to queue.")
        // The command is added to the queue synchronously.
        // The LaunchedEffect in DragonBonesViewCompose is responsible for consuming them.
        animationCommandQueue.add(FadeInAnimationCommand(name, layer, loop, fadeInTime))
    }

    fun stopAnimation(name: String) {
        Log.d(TAG, "stopAnimation: Adding command for '$name' to queue.")
        // The command is added to the queue synchronously.
        animationCommandQueue.add(StopAnimationCommand(name))
    }

    /**
     * Clears all pending animation commands from the queue.
     * This should be called before loading a new model to prevent
     * old commands from executing on the new model.
     */
    fun clearAnimationQueue() {
        Log.d(TAG, "clearAnimationQueue: Called, queue size was ${animationCommandQueue.size}")
        animationCommandQueue.clear()
    }

    /**
     * Atomically retrieves and removes the next command from the queue.
     * Returns null if the queue is empty.
     */
    internal fun consumeNextAnimationCommand(): AnimationCommand? {
        return animationCommandQueue.removeFirstOrNull()
    }

    /**
     * Plays the specified animation.
     *
     * @param name The name of the animation to play.
     */
    @Deprecated("Use fadeInAnimation for more control over layers and looping.", ReplaceWith("fadeInAnimation(name, layer = 0, loop = 0, fadeInTime = fadeInTime)"))
    fun playAnimation(name: String, fadeInTime: Float = 0.3f) {
        fadeInAnimation(name = name, layer = 0, loop = 0, fadeInTime = fadeInTime)
    }

    fun overrideBonePosition(boneName: String, x: Float, y: Float) {
        // This is called frequently, so keep logging minimal or commented out
        // Log.d(TAG, "overrideBonePosition: $boneName to ($x, $y)")
        view?.overrideBonePosition(boneName, x, y)
    }

    fun resetBone(boneName: String) {
        Log.d(TAG, "resetBone: $boneName")
        view?.resetBone(boneName)
    }

    internal fun setAnimationNames(names: List<String>) {
        Log.d(TAG, "setAnimationNames: Setting names to ${names.joinToString(", ")}. Count: ${names.size}")
        animationNames = names
    }
}

/**
 * Creates and remembers a [DragonBonesController].
 *
 * @return A new [DragonBonesController] instance.
 */
@Composable
fun rememberDragonBonesController(): DragonBonesController {
    val coroutineScope = androidx.compose.runtime.rememberCoroutineScope()
    return remember { DragonBonesController(coroutineScope) }
}

/**
 * Represents a command to be executed on the DragonBones view.
 */
sealed interface AnimationCommand

/**
 * Represents a command to play a DragonBones animation.
 * @param name The name of the animation.
 * @param layer The layer (track) to play on.
 * @param loop The number of times to loop (0 for infinite).
 * @param fadeInTime The duration of the fade-in effect.
 * @param id A unique identifier to allow recomposition to trigger the same animation again.
 */
data class FadeInAnimationCommand(
    val name: String,
    val layer: Int,
    val loop: Int,
    val fadeInTime: Float,
    val id: Long = System.currentTimeMillis()
) : AnimationCommand

/**
 * Represents a command to stop a specific DragonBones animation.
 * @param name The name of the animation to stop.
 * @param id A unique identifier.
 */
data class StopAnimationCommand(
    val name: String,
    val id: Long = System.currentTimeMillis()
) : AnimationCommand
