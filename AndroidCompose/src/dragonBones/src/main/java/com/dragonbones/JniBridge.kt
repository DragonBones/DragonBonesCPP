package com.dragonbones

object JniBridge {
    init {
        System.loadLibrary("dragonbones_native")
    }

    @JvmStatic external fun init()

    @JvmStatic
    external fun loadDragonBones(
            skeletonData: ByteArray,
            textureJsonData: ByteArray,
            texturePngData: ByteArray
    )

    @JvmStatic external fun onPause()

    @JvmStatic external fun onResume()

    @JvmStatic external fun onDestroy()

    @JvmStatic external fun onSurfaceCreated()

    @JvmStatic external fun onSurfaceChanged(width: Int, height: Int)

    @JvmStatic external fun onDrawFrame()

    @JvmStatic external fun getAnimationNames(): Array<String>?

    @JvmStatic external fun fadeInAnimation(name: String, layer: Int, loop: Int, fadeInTime: Float)

    @JvmStatic external fun containsPoint(x: Float, y: Float): String?

    @JvmStatic external fun setWorldScale(scale: Float)

    @JvmStatic external fun setWorldTranslation(x: Float, y: Float)

    @JvmStatic external fun overrideBonePosition(boneName: String, x: Float, y: Float)

    @JvmStatic external fun resetBone(boneName: String)

    @JvmStatic external fun stopAnimation(name: String)
}
