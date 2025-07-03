#ifndef DRAGONBONES_JNIBRIDGE_H
#define DRAGONBONES_JNIBRIDGE_H

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_init(JNIEnv *env, jclass clazz);

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_loadDragonBones(JNIEnv *env, jclass clazz, jbyteArray skeleton_data, jbyteArray texture_json_data, jbyteArray texture_png_data);

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_onPause(JNIEnv *env, jclass clazz);

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_onResume(JNIEnv *env, jclass clazz);

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_onDestroy(JNIEnv *env, jclass clazz);

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_onSurfaceCreated(JNIEnv *env, jclass clazz);

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_onSurfaceChanged(JNIEnv *env, jclass clazz, jint width, jint height);

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_onDrawFrame(JNIEnv *env, jclass clazz);

JNIEXPORT jobjectArray JNICALL
Java_com_dragonbones_JniBridge_getAnimationNames(JNIEnv *env, jclass clazz);

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_fadeInAnimation(JNIEnv *env, jclass clazz, jstring animation_name, jint layer, jint loop, jfloat fade_in_time);

JNIEXPORT jstring JNICALL
Java_com_dragonbones_JniBridge_containsPoint(JNIEnv *env, jclass clazz, jfloat x, jfloat y);

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_setWorldScale(JNIEnv *env, jclass clazz, jfloat scale);

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_setWorldTranslation(JNIEnv *env, jclass clazz, jfloat x, jfloat y);

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_overrideBonePosition(JNIEnv *env, jclass clazz, jstring bone_name, jfloat x, jfloat y);

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_resetBone(JNIEnv *env, jclass clazz, jstring bone_name);

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_stopAnimation(JNIEnv *env, jclass clazz, jstring animation_name);

#ifdef __cplusplus
}
#endif

#endif //DRAGONBONES_JNIBRIDGE_H 