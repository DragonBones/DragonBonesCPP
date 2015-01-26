#ifndef __dragonbones_bindings_h__
#define __dragonbones_bindings_h__

#include "jsapi.h"
#include "jsfriendapi.h"


extern JSClass  *jsb_dragonBones_DragonBonesData_class;
extern JSObject *jsb_dragonBones_DragonBonesData_prototype;

bool js_dragonbones_bindings_DragonBonesData_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_dragonbones_bindings_DragonBonesData_finalize(JSContext *cx, JSObject *obj);
void js_register_dragonbones_bindings_DragonBonesData(JSContext *cx, JSObject *global);
void register_all_dragonbones_bindings(JSContext* cx, JSObject* obj);
bool js_dragonbones_bindings_DragonBonesData_dispose(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DragonBonesData_getArmatureData(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DragonBonesData_DragonBonesData(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_dragonBones_ITextureAtlas_class;
extern JSObject *jsb_dragonBones_ITextureAtlas_prototype;

bool js_dragonbones_bindings_ITextureAtlas_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_dragonbones_bindings_ITextureAtlas_finalize(JSContext *cx, JSObject *obj);
void js_register_dragonbones_bindings_ITextureAtlas(JSContext *cx, JSObject *global);
void register_all_dragonbones_bindings(JSContext* cx, JSObject* obj);
bool js_dragonbones_bindings_ITextureAtlas_dispose(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_dragonBones_Animation_class;
extern JSObject *jsb_dragonBones_Animation_prototype;

bool js_dragonbones_bindings_Animation_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_dragonbones_bindings_Animation_finalize(JSContext *cx, JSObject *obj);
void js_register_dragonbones_bindings_Animation(JSContext *cx, JSObject *global);
void register_all_dragonbones_bindings(JSContext* cx, JSObject* obj);
bool js_dragonbones_bindings_Animation_getAnimationList(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_Animation_setTimeScale(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_Animation_stop(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_Animation_clear(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_Animation_dispose(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_Animation_play(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_Animation_hasAnimation(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_Animation_getLastAnimationState(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_Animation_gotoAndPlay(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_Animation_getState(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_Animation_getIsComplete(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_Animation_advanceTime(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_Animation_getIsPlaying(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_Animation_gotoAndStop(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_Animation_getTimeScale(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_Animation_Animation(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_dragonBones_DBCCArmature_class;
extern JSObject *jsb_dragonBones_DBCCArmature_prototype;

bool js_dragonbones_bindings_DBCCArmature_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_dragonbones_bindings_DBCCArmature_finalize(JSContext *cx, JSObject *obj);
void js_register_dragonbones_bindings_DBCCArmature(JSContext *cx, JSObject *global);
void register_all_dragonbones_bindings(JSContext* cx, JSObject* obj);
bool js_dragonbones_bindings_DBCCArmature_getCCBoundingBox(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCArmature_getCCEventDispatcher(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCArmature_dispose(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCArmature_getCCSlot(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCArmature_getCCDisplay(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCArmature_DBCCArmature(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_dragonBones_DBCCArmatureNode_class;
extern JSObject *jsb_dragonBones_DBCCArmatureNode_prototype;

bool js_dragonbones_bindings_DBCCArmatureNode_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_dragonbones_bindings_DBCCArmatureNode_finalize(JSContext *cx, JSObject *obj);
void js_register_dragonbones_bindings_DBCCArmatureNode(JSContext *cx, JSObject *global);
void register_all_dragonbones_bindings(JSContext* cx, JSObject* obj);
bool js_dragonbones_bindings_DBCCArmatureNode_getAnimation(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCArmatureNode_getCCEventDispatcher(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCArmatureNode_getArmature(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCArmatureNode_getCCSlot(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCArmatureNode_advanceTime(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCArmatureNode_initWithDBCCArmature(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCArmatureNode_getCCDisplay(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCArmatureNode_create(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCArmatureNode_createWithWorldClock(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCArmatureNode_DBCCArmatureNode(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_dragonBones_DBCCFactory_class;
extern JSObject *jsb_dragonBones_DBCCFactory_prototype;

bool js_dragonbones_bindings_DBCCFactory_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_dragonbones_bindings_DBCCFactory_finalize(JSContext *cx, JSObject *obj);
void js_register_dragonbones_bindings_DBCCFactory(JSContext *cx, JSObject *global);
void register_all_dragonbones_bindings(JSContext* cx, JSObject* obj);
bool js_dragonbones_bindings_DBCCFactory_buildArmatureNode(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCFactory_hasDragonBones(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCFactory_buildArmature(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCFactory_loadTextureAtlas(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCFactory_refreshAllTextureAtlasTexture(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCFactory_refreshTextureAtlasTexture(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCFactory_loadDragonBonesData(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCFactory_destroyInstance(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCFactory_getInstance(JSContext *cx, uint32_t argc, jsval *vp);
bool js_dragonbones_bindings_DBCCFactory_DBCCFactory(JSContext *cx, uint32_t argc, jsval *vp);
#endif

