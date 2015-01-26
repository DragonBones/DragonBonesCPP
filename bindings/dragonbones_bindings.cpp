#include "dragonbones_bindings.hpp"
#include "cocos2d_specifics.hpp"
#include "DBCCRenderHeaders.h"

template<class T>
static bool dummy_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::RootedValue initializing(cx);
    bool isNewValid = true;
    if (isNewValid)
    {
        TypeTest<T> t;
        js_type_class_t *typeClass = nullptr;
        std::string typeName = t.s_name();
        auto typeMapIter = _js_global_type_map.find(typeName);
        CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
        typeClass = typeMapIter->second;
        CCASSERT(typeClass, "The value is null.");

        JSObject *_tmp = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
        T* cobj = new T();
        js_proxy_t *pp = jsb_new_proxy(cobj, _tmp);
        JS_AddObjectRoot(cx, &pp->obj);
        JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(_tmp));
        return true;
    }

    return false;
}

static bool empty_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
    return false;
}

static bool js_is_native_obj(JSContext *cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp)
{
    vp.set(BOOLEAN_TO_JSVAL(true));
    return true;    
}
JSClass  *jsb_dragonBones_DragonBonesData_class;
JSObject *jsb_dragonBones_DragonBonesData_prototype;

bool js_dragonbones_bindings_DragonBonesData_dispose(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DragonBonesData* cobj = (dragonBones::DragonBonesData *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DragonBonesData_dispose : Invalid Native Object");
    if (argc == 0) {
        cobj->dispose();
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DragonBonesData_dispose : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_DragonBonesData_getArmatureData(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DragonBonesData* cobj = (dragonBones::DragonBonesData *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DragonBonesData_getArmatureData : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_DragonBonesData_getArmatureData : Error processing arguments");
        dragonBones::ArmatureData* ret = cobj->getArmatureData(arg0);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::ArmatureData>(cx, (dragonBones::ArmatureData*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DragonBonesData_getArmatureData : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_dragonbones_bindings_DragonBonesData_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    dragonBones::DragonBonesData* cobj = new (std::nothrow) dragonBones::DragonBonesData();
    TypeTest<dragonBones::DragonBonesData> t;
    js_type_class_t *typeClass = nullptr;
    std::string typeName = t.s_name();
    auto typeMapIter = _js_global_type_map.find(typeName);
    CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
    typeClass = typeMapIter->second;
    CCASSERT(typeClass, "The value is null.");
    JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
    // link the native object with the javascript object
    js_proxy_t* p = jsb_new_proxy(cobj, obj);
    if (JS_HasProperty(cx, obj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", argc, argv);
    return true;
}



void js_dragonBones_DragonBonesData_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (DragonBonesData)", obj);
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    jsproxy = jsb_get_js_proxy(obj);
    if (jsproxy) {
        nproxy = jsb_get_native_proxy(jsproxy->ptr);

        dragonBones::DragonBonesData *nobj = static_cast<dragonBones::DragonBonesData *>(nproxy->ptr);
        if (nobj)
            delete nobj;
        
        jsb_remove_proxy(nproxy, jsproxy);
    }
}

void js_register_dragonbones_bindings_DragonBonesData(JSContext *cx, JSObject *global) {
    jsb_dragonBones_DragonBonesData_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_dragonBones_DragonBonesData_class->name = "DragonBonesData";
    jsb_dragonBones_DragonBonesData_class->addProperty = JS_PropertyStub;
    jsb_dragonBones_DragonBonesData_class->delProperty = JS_DeletePropertyStub;
    jsb_dragonBones_DragonBonesData_class->getProperty = JS_PropertyStub;
    jsb_dragonBones_DragonBonesData_class->setProperty = JS_StrictPropertyStub;
    jsb_dragonBones_DragonBonesData_class->enumerate = JS_EnumerateStub;
    jsb_dragonBones_DragonBonesData_class->resolve = JS_ResolveStub;
    jsb_dragonBones_DragonBonesData_class->convert = JS_ConvertStub;
    jsb_dragonBones_DragonBonesData_class->finalize = js_dragonBones_DragonBonesData_finalize;
    jsb_dragonBones_DragonBonesData_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        {"__nativeObj", 0, JSPROP_ENUMERATE | JSPROP_PERMANENT, JSOP_WRAPPER(js_is_native_obj), JSOP_NULLWRAPPER},
        {0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("dispose", js_dragonbones_bindings_DragonBonesData_dispose, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getArmatureData", js_dragonbones_bindings_DragonBonesData_getArmatureData, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    jsb_dragonBones_DragonBonesData_prototype = JS_InitClass(
        cx, global,
        NULL, // parent proto
        jsb_dragonBones_DragonBonesData_class,
        js_dragonbones_bindings_DragonBonesData_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);
    // make the class enumerable in the registered namespace
//  bool found;
//FIXME: Removed in Firefox v27 
//  JS_SetPropertyAttributes(cx, global, "DragonBonesData", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

    // add the proto and JSClass to the type->js info hash table
    TypeTest<dragonBones::DragonBonesData> t;
    js_type_class_t *p;
    std::string typeName = t.s_name();
    if (_js_global_type_map.find(typeName) == _js_global_type_map.end())
    {
        p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
        p->jsclass = jsb_dragonBones_DragonBonesData_class;
        p->proto = jsb_dragonBones_DragonBonesData_prototype;
        p->parentProto = NULL;
        _js_global_type_map.insert(std::make_pair(typeName, p));
    }
}

JSClass  *jsb_dragonBones_ITextureAtlas_class;
JSObject *jsb_dragonBones_ITextureAtlas_prototype;

bool js_dragonbones_bindings_ITextureAtlas_dispose(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::ITextureAtlas* cobj = (dragonBones::ITextureAtlas *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_ITextureAtlas_dispose : Invalid Native Object");
    if (argc == 0) {
        cobj->dispose();
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_ITextureAtlas_dispose : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}


void js_dragonBones_ITextureAtlas_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (ITextureAtlas)", obj);
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    jsproxy = jsb_get_js_proxy(obj);
    if (jsproxy) {
        nproxy = jsb_get_native_proxy(jsproxy->ptr);

        dragonBones::ITextureAtlas *nobj = static_cast<dragonBones::ITextureAtlas *>(nproxy->ptr);
        if (nobj)
            delete nobj;
        
        jsb_remove_proxy(nproxy, jsproxy);
    }
}

void js_register_dragonbones_bindings_ITextureAtlas(JSContext *cx, JSObject *global) {
    jsb_dragonBones_ITextureAtlas_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_dragonBones_ITextureAtlas_class->name = "ITextureAtlas";
    jsb_dragonBones_ITextureAtlas_class->addProperty = JS_PropertyStub;
    jsb_dragonBones_ITextureAtlas_class->delProperty = JS_DeletePropertyStub;
    jsb_dragonBones_ITextureAtlas_class->getProperty = JS_PropertyStub;
    jsb_dragonBones_ITextureAtlas_class->setProperty = JS_StrictPropertyStub;
    jsb_dragonBones_ITextureAtlas_class->enumerate = JS_EnumerateStub;
    jsb_dragonBones_ITextureAtlas_class->resolve = JS_ResolveStub;
    jsb_dragonBones_ITextureAtlas_class->convert = JS_ConvertStub;
    jsb_dragonBones_ITextureAtlas_class->finalize = js_dragonBones_ITextureAtlas_finalize;
    jsb_dragonBones_ITextureAtlas_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        {"__nativeObj", 0, JSPROP_ENUMERATE | JSPROP_PERMANENT, JSOP_WRAPPER(js_is_native_obj), JSOP_NULLWRAPPER},
        {0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("dispose", js_dragonbones_bindings_ITextureAtlas_dispose, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    jsb_dragonBones_ITextureAtlas_prototype = JS_InitClass(
        cx, global,
        NULL, // parent proto
        jsb_dragonBones_ITextureAtlas_class,
        empty_constructor, 0,
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);
    // make the class enumerable in the registered namespace
//  bool found;
//FIXME: Removed in Firefox v27 
//  JS_SetPropertyAttributes(cx, global, "ITextureAtlas", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

    // add the proto and JSClass to the type->js info hash table
    TypeTest<dragonBones::ITextureAtlas> t;
    js_type_class_t *p;
    std::string typeName = t.s_name();
    if (_js_global_type_map.find(typeName) == _js_global_type_map.end())
    {
        p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
        p->jsclass = jsb_dragonBones_ITextureAtlas_class;
        p->proto = jsb_dragonBones_ITextureAtlas_prototype;
        p->parentProto = NULL;
        _js_global_type_map.insert(std::make_pair(typeName, p));
    }
}

JSClass  *jsb_dragonBones_Animation_class;
JSObject *jsb_dragonBones_Animation_prototype;

bool js_dragonbones_bindings_Animation_getAnimationList(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::Animation* cobj = (dragonBones::Animation *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_Animation_getAnimationList : Invalid Native Object");
    if (argc == 0) {
        const std::vector<std::string>& ret = cobj->getAnimationList();
        jsval jsret = JSVAL_NULL;
        jsret = std_vector_string_to_jsval(cx, ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_Animation_getAnimationList : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_Animation_setTimeScale(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::Animation* cobj = (dragonBones::Animation *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_Animation_setTimeScale : Invalid Native Object");
    if (argc == 1) {
        double arg0;
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[0]), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_setTimeScale : Error processing arguments");
        cobj->setTimeScale(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_Animation_setTimeScale : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_dragonbones_bindings_Animation_stop(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::Animation* cobj = (dragonBones::Animation *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_Animation_stop : Invalid Native Object");
    if (argc == 0) {
        cobj->stop();
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_Animation_stop : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_Animation_clear(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::Animation* cobj = (dragonBones::Animation *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_Animation_clear : Invalid Native Object");
    if (argc == 0) {
        cobj->clear();
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_Animation_clear : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_Animation_dispose(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::Animation* cobj = (dragonBones::Animation *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_Animation_dispose : Invalid Native Object");
    if (argc == 0) {
        cobj->dispose();
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_Animation_dispose : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_Animation_play(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::Animation* cobj = (dragonBones::Animation *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_Animation_play : Invalid Native Object");
    if (argc == 0) {
        cobj->play();
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_Animation_play : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_Animation_hasAnimation(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::Animation* cobj = (dragonBones::Animation *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_Animation_hasAnimation : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_hasAnimation : Error processing arguments");
        bool ret = cobj->hasAnimation(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_Animation_hasAnimation : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_dragonbones_bindings_Animation_getLastAnimationState(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::Animation* cobj = (dragonBones::Animation *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_Animation_getLastAnimationState : Invalid Native Object");
    if (argc == 0) {
        dragonBones::AnimationState* ret = cobj->getLastAnimationState();
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_Animation_getLastAnimationState : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_Animation_gotoAndPlay(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::Animation* cobj = (dragonBones::Animation *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_Animation_gotoAndPlay : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_gotoAndPlay : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->gotoAndPlay(arg0);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        double arg1;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[1]), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_gotoAndPlay : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->gotoAndPlay(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 3) {
        std::string arg0;
        double arg1;
        double arg2;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[1]), &arg1);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[2]), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_gotoAndPlay : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->gotoAndPlay(arg0, arg1, arg2);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 4) {
        std::string arg0;
        double arg1;
        double arg2;
        int arg3;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[1]), &arg1);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[2]), &arg2);
        ok &= jsval_to_int32(cx, argv[3], (int32_t *)&arg3);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_gotoAndPlay : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->gotoAndPlay(arg0, arg1, arg2, arg3);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 5) {
        std::string arg0;
        double arg1;
        double arg2;
        int arg3;
        int arg4;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[1]), &arg1);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[2]), &arg2);
        ok &= jsval_to_int32(cx, argv[3], (int32_t *)&arg3);
        ok &= jsval_to_int32(cx, argv[4], (int32_t *)&arg4);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_gotoAndPlay : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->gotoAndPlay(arg0, arg1, arg2, arg3, arg4);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 6) {
        std::string arg0;
        double arg1;
        double arg2;
        int arg3;
        int arg4;
        std::string arg5;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[1]), &arg1);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[2]), &arg2);
        ok &= jsval_to_int32(cx, argv[3], (int32_t *)&arg3);
        ok &= jsval_to_int32(cx, argv[4], (int32_t *)&arg4);
        ok &= jsval_to_std_string(cx, argv[5], &arg5);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_gotoAndPlay : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->gotoAndPlay(arg0, arg1, arg2, arg3, arg4, arg5);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 7) {
        std::string arg0;
        double arg1;
        double arg2;
        int arg3;
        int arg4;
        std::string arg5;
        dragonBones::Animation::AnimationFadeOutMode arg6;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[1]), &arg1);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[2]), &arg2);
        ok &= jsval_to_int32(cx, argv[3], (int32_t *)&arg3);
        ok &= jsval_to_int32(cx, argv[4], (int32_t *)&arg4);
        ok &= jsval_to_std_string(cx, argv[5], &arg5);
        ok &= jsval_to_int32(cx, argv[6], (int32_t *)&arg6);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_gotoAndPlay : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->gotoAndPlay(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 8) {
        std::string arg0;
        double arg1;
        double arg2;
        int arg3;
        int arg4;
        std::string arg5;
        dragonBones::Animation::AnimationFadeOutMode arg6;
        bool arg7;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[1]), &arg1);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[2]), &arg2);
        ok &= jsval_to_int32(cx, argv[3], (int32_t *)&arg3);
        ok &= jsval_to_int32(cx, argv[4], (int32_t *)&arg4);
        ok &= jsval_to_std_string(cx, argv[5], &arg5);
        ok &= jsval_to_int32(cx, argv[6], (int32_t *)&arg6);
        arg7 = JS::ToBoolean(JS::RootedValue(cx, argv[7]));
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_gotoAndPlay : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->gotoAndPlay(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 9) {
        std::string arg0;
        double arg1;
        double arg2;
        int arg3;
        int arg4;
        std::string arg5;
        dragonBones::Animation::AnimationFadeOutMode arg6;
        bool arg7;
        bool arg8;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[1]), &arg1);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[2]), &arg2);
        ok &= jsval_to_int32(cx, argv[3], (int32_t *)&arg3);
        ok &= jsval_to_int32(cx, argv[4], (int32_t *)&arg4);
        ok &= jsval_to_std_string(cx, argv[5], &arg5);
        ok &= jsval_to_int32(cx, argv[6], (int32_t *)&arg6);
        arg7 = JS::ToBoolean(JS::RootedValue(cx, argv[7]));
        arg8 = JS::ToBoolean(JS::RootedValue(cx, argv[8]));
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_gotoAndPlay : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->gotoAndPlay(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_Animation_gotoAndPlay : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_dragonbones_bindings_Animation_getState(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::Animation* cobj = (dragonBones::Animation *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_Animation_getState : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_getState : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->getState(arg0);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        int arg1;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= jsval_to_int32(cx, argv[1], (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_getState : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->getState(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_Animation_getState : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_dragonbones_bindings_Animation_getIsComplete(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::Animation* cobj = (dragonBones::Animation *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_Animation_getIsComplete : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->getIsComplete();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_Animation_getIsComplete : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_Animation_advanceTime(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::Animation* cobj = (dragonBones::Animation *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_Animation_advanceTime : Invalid Native Object");
    if (argc == 1) {
        double arg0;
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[0]), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_advanceTime : Error processing arguments");
        cobj->advanceTime(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_Animation_advanceTime : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_dragonbones_bindings_Animation_getIsPlaying(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::Animation* cobj = (dragonBones::Animation *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_Animation_getIsPlaying : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->getIsPlaying();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_Animation_getIsPlaying : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_Animation_gotoAndStop(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::Animation* cobj = (dragonBones::Animation *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_Animation_gotoAndStop : Invalid Native Object");
    if (argc == 2) {
        std::string arg0;
        double arg1;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[1]), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_gotoAndStop : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->gotoAndStop(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 3) {
        std::string arg0;
        double arg1;
        double arg2;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[1]), &arg1);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[2]), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_gotoAndStop : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->gotoAndStop(arg0, arg1, arg2);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 4) {
        std::string arg0;
        double arg1;
        double arg2;
        double arg3;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[1]), &arg1);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[2]), &arg2);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[3]), &arg3);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_gotoAndStop : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->gotoAndStop(arg0, arg1, arg2, arg3);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 5) {
        std::string arg0;
        double arg1;
        double arg2;
        double arg3;
        double arg4;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[1]), &arg1);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[2]), &arg2);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[3]), &arg3);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[4]), &arg4);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_gotoAndStop : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->gotoAndStop(arg0, arg1, arg2, arg3, arg4);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 6) {
        std::string arg0;
        double arg1;
        double arg2;
        double arg3;
        double arg4;
        int arg5;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[1]), &arg1);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[2]), &arg2);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[3]), &arg3);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[4]), &arg4);
        ok &= jsval_to_int32(cx, argv[5], (int32_t *)&arg5);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_gotoAndStop : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->gotoAndStop(arg0, arg1, arg2, arg3, arg4, arg5);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 7) {
        std::string arg0;
        double arg1;
        double arg2;
        double arg3;
        double arg4;
        int arg5;
        std::string arg6;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[1]), &arg1);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[2]), &arg2);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[3]), &arg3);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[4]), &arg4);
        ok &= jsval_to_int32(cx, argv[5], (int32_t *)&arg5);
        ok &= jsval_to_std_string(cx, argv[6], &arg6);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_gotoAndStop : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->gotoAndStop(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 8) {
        std::string arg0;
        double arg1;
        double arg2;
        double arg3;
        double arg4;
        int arg5;
        std::string arg6;
        dragonBones::Animation::AnimationFadeOutMode arg7;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[1]), &arg1);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[2]), &arg2);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[3]), &arg3);
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[4]), &arg4);
        ok &= jsval_to_int32(cx, argv[5], (int32_t *)&arg5);
        ok &= jsval_to_std_string(cx, argv[6], &arg6);
        ok &= jsval_to_int32(cx, argv[7], (int32_t *)&arg7);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_Animation_gotoAndStop : Error processing arguments");
        dragonBones::AnimationState* ret = cobj->gotoAndStop(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::AnimationState>(cx, (dragonBones::AnimationState*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_Animation_gotoAndStop : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_dragonbones_bindings_Animation_getTimeScale(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::Animation* cobj = (dragonBones::Animation *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_Animation_getTimeScale : Invalid Native Object");
    if (argc == 0) {
        double ret = cobj->getTimeScale();
        jsval jsret = JSVAL_NULL;
        jsret = DOUBLE_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_Animation_getTimeScale : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_Animation_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    dragonBones::Animation* cobj = new (std::nothrow) dragonBones::Animation();
    TypeTest<dragonBones::Animation> t;
    js_type_class_t *typeClass = nullptr;
    std::string typeName = t.s_name();
    auto typeMapIter = _js_global_type_map.find(typeName);
    CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
    typeClass = typeMapIter->second;
    CCASSERT(typeClass, "The value is null.");
    JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
    // link the native object with the javascript object
    js_proxy_t* p = jsb_new_proxy(cobj, obj);
    if (JS_HasProperty(cx, obj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", argc, argv);
    return true;
}



void js_dragonBones_Animation_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (Animation)", obj);
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    jsproxy = jsb_get_js_proxy(obj);
    if (jsproxy) {
        nproxy = jsb_get_native_proxy(jsproxy->ptr);

        dragonBones::Animation *nobj = static_cast<dragonBones::Animation *>(nproxy->ptr);
        if (nobj)
            delete nobj;
        
        jsb_remove_proxy(nproxy, jsproxy);
    }
}

void js_register_dragonbones_bindings_Animation(JSContext *cx, JSObject *global) {
    jsb_dragonBones_Animation_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_dragonBones_Animation_class->name = "Animation";
    jsb_dragonBones_Animation_class->addProperty = JS_PropertyStub;
    jsb_dragonBones_Animation_class->delProperty = JS_DeletePropertyStub;
    jsb_dragonBones_Animation_class->getProperty = JS_PropertyStub;
    jsb_dragonBones_Animation_class->setProperty = JS_StrictPropertyStub;
    jsb_dragonBones_Animation_class->enumerate = JS_EnumerateStub;
    jsb_dragonBones_Animation_class->resolve = JS_ResolveStub;
    jsb_dragonBones_Animation_class->convert = JS_ConvertStub;
    jsb_dragonBones_Animation_class->finalize = js_dragonBones_Animation_finalize;
    jsb_dragonBones_Animation_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        {"__nativeObj", 0, JSPROP_ENUMERATE | JSPROP_PERMANENT, JSOP_WRAPPER(js_is_native_obj), JSOP_NULLWRAPPER},
        {0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("getAnimationList", js_dragonbones_bindings_Animation_getAnimationList, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setTimeScale", js_dragonbones_bindings_Animation_setTimeScale, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("stop", js_dragonbones_bindings_Animation_stop, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("clear", js_dragonbones_bindings_Animation_clear, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("dispose", js_dragonbones_bindings_Animation_dispose, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("play", js_dragonbones_bindings_Animation_play, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("hasAnimation", js_dragonbones_bindings_Animation_hasAnimation, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getLastAnimationState", js_dragonbones_bindings_Animation_getLastAnimationState, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("gotoAndPlay", js_dragonbones_bindings_Animation_gotoAndPlay, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getState", js_dragonbones_bindings_Animation_getState, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getIsComplete", js_dragonbones_bindings_Animation_getIsComplete, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("advanceTime", js_dragonbones_bindings_Animation_advanceTime, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getIsPlaying", js_dragonbones_bindings_Animation_getIsPlaying, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("gotoAndStop", js_dragonbones_bindings_Animation_gotoAndStop, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getTimeScale", js_dragonbones_bindings_Animation_getTimeScale, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    jsb_dragonBones_Animation_prototype = JS_InitClass(
        cx, global,
        NULL, // parent proto
        jsb_dragonBones_Animation_class,
        js_dragonbones_bindings_Animation_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);
    // make the class enumerable in the registered namespace
//  bool found;
//FIXME: Removed in Firefox v27 
//  JS_SetPropertyAttributes(cx, global, "Animation", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

    // add the proto and JSClass to the type->js info hash table
    TypeTest<dragonBones::Animation> t;
    js_type_class_t *p;
    std::string typeName = t.s_name();
    if (_js_global_type_map.find(typeName) == _js_global_type_map.end())
    {
        p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
        p->jsclass = jsb_dragonBones_Animation_class;
        p->proto = jsb_dragonBones_Animation_prototype;
        p->parentProto = NULL;
        _js_global_type_map.insert(std::make_pair(typeName, p));
    }
}

JSClass  *jsb_dragonBones_DBCCArmature_class;
JSObject *jsb_dragonBones_DBCCArmature_prototype;

bool js_dragonbones_bindings_DBCCArmature_getCCBoundingBox(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DBCCArmature* cobj = (dragonBones::DBCCArmature *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCArmature_getCCBoundingBox : Invalid Native Object");
    if (argc == 0) {
        cocos2d::Rect ret = cobj->getCCBoundingBox();
        jsval jsret = JSVAL_NULL;
        jsret = ccrect_to_jsval(cx, ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCArmature_getCCBoundingBox : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_DBCCArmature_getCCEventDispatcher(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DBCCArmature* cobj = (dragonBones::DBCCArmature *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCArmature_getCCEventDispatcher : Invalid Native Object");
    if (argc == 0) {
        cocos2d::EventDispatcher* ret = cobj->getCCEventDispatcher();
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<cocos2d::EventDispatcher>(cx, (cocos2d::EventDispatcher*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCArmature_getCCEventDispatcher : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_DBCCArmature_dispose(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DBCCArmature* cobj = (dragonBones::DBCCArmature *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCArmature_dispose : Invalid Native Object");
    if (argc == 0) {
        cobj->dispose();
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCArmature_dispose : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_DBCCArmature_getCCSlot(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DBCCArmature* cobj = (dragonBones::DBCCArmature *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCArmature_getCCSlot : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_DBCCArmature_getCCSlot : Error processing arguments");
        dragonBones::DBCCSlot* ret = cobj->getCCSlot(arg0);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::DBCCSlot>(cx, (dragonBones::DBCCSlot*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCArmature_getCCSlot : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_dragonbones_bindings_DBCCArmature_getCCDisplay(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DBCCArmature* cobj = (dragonBones::DBCCArmature *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCArmature_getCCDisplay : Invalid Native Object");
    if (argc == 0) {
        cocos2d::Node* ret = cobj->getCCDisplay();
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<cocos2d::Node>(cx, (cocos2d::Node*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCArmature_getCCDisplay : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_DBCCArmature_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    dragonBones::ArmatureData* arg0;
    dragonBones::Animation* arg1;
    dragonBones::IEventDispatcher* arg2;
    cocos2d::Node* arg3;
    do {
            if (!argv[0].isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JSObject *tmpObj = JSVAL_TO_OBJECT(argv[0]);
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (dragonBones::ArmatureData*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
    do {
            if (!argv[1].isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JSObject *tmpObj = JSVAL_TO_OBJECT(argv[1]);
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (dragonBones::Animation*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
    do {
            if (!argv[2].isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JSObject *tmpObj = JSVAL_TO_OBJECT(argv[2]);
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg2 = (dragonBones::IEventDispatcher*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg2, cx, false, "Invalid Native Object");
        } while (0);
    do {
            if (!argv[3].isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JSObject *tmpObj = JSVAL_TO_OBJECT(argv[3]);
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg3 = (cocos2d::Node*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg3, cx, false, "Invalid Native Object");
        } while (0);
    JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_DBCCArmature_constructor : Error processing arguments");
    dragonBones::DBCCArmature* cobj = new (std::nothrow) dragonBones::DBCCArmature(arg0, arg1, arg2, arg3);
    TypeTest<dragonBones::DBCCArmature> t;
    js_type_class_t *typeClass = nullptr;
    std::string typeName = t.s_name();
    auto typeMapIter = _js_global_type_map.find(typeName);
    CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
    typeClass = typeMapIter->second;
    CCASSERT(typeClass, "The value is null.");
    JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
    // link the native object with the javascript object
    js_proxy_t* p = jsb_new_proxy(cobj, obj);
    if (JS_HasProperty(cx, obj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", argc, argv);
    return true;
}



void js_dragonBones_DBCCArmature_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (DBCCArmature)", obj);
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    jsproxy = jsb_get_js_proxy(obj);
    if (jsproxy) {
        nproxy = jsb_get_native_proxy(jsproxy->ptr);

        dragonBones::DBCCArmature *nobj = static_cast<dragonBones::DBCCArmature *>(nproxy->ptr);
        if (nobj)
            delete nobj;
        
        jsb_remove_proxy(nproxy, jsproxy);
    }
}

void js_register_dragonbones_bindings_DBCCArmature(JSContext *cx, JSObject *global) {
    jsb_dragonBones_DBCCArmature_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_dragonBones_DBCCArmature_class->name = "CCArmature";
    jsb_dragonBones_DBCCArmature_class->addProperty = JS_PropertyStub;
    jsb_dragonBones_DBCCArmature_class->delProperty = JS_DeletePropertyStub;
    jsb_dragonBones_DBCCArmature_class->getProperty = JS_PropertyStub;
    jsb_dragonBones_DBCCArmature_class->setProperty = JS_StrictPropertyStub;
    jsb_dragonBones_DBCCArmature_class->enumerate = JS_EnumerateStub;
    jsb_dragonBones_DBCCArmature_class->resolve = JS_ResolveStub;
    jsb_dragonBones_DBCCArmature_class->convert = JS_ConvertStub;
    jsb_dragonBones_DBCCArmature_class->finalize = js_dragonBones_DBCCArmature_finalize;
    jsb_dragonBones_DBCCArmature_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        {"__nativeObj", 0, JSPROP_ENUMERATE | JSPROP_PERMANENT, JSOP_WRAPPER(js_is_native_obj), JSOP_NULLWRAPPER},
        {0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("getCCBoundingBox", js_dragonbones_bindings_DBCCArmature_getCCBoundingBox, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCCEventDispatcher", js_dragonbones_bindings_DBCCArmature_getCCEventDispatcher, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("dispose", js_dragonbones_bindings_DBCCArmature_dispose, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCCSlot", js_dragonbones_bindings_DBCCArmature_getCCSlot, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCCDisplay", js_dragonbones_bindings_DBCCArmature_getCCDisplay, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    jsb_dragonBones_DBCCArmature_prototype = JS_InitClass(
        cx, global,
        NULL, // parent proto
        jsb_dragonBones_DBCCArmature_class,
        js_dragonbones_bindings_DBCCArmature_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);
    // make the class enumerable in the registered namespace
//  bool found;
//FIXME: Removed in Firefox v27 
//  JS_SetPropertyAttributes(cx, global, "CCArmature", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

    // add the proto and JSClass to the type->js info hash table
    TypeTest<dragonBones::DBCCArmature> t;
    js_type_class_t *p;
    std::string typeName = t.s_name();
    if (_js_global_type_map.find(typeName) == _js_global_type_map.end())
    {
        p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
        p->jsclass = jsb_dragonBones_DBCCArmature_class;
        p->proto = jsb_dragonBones_DBCCArmature_prototype;
        p->parentProto = NULL;
        _js_global_type_map.insert(std::make_pair(typeName, p));
    }
}

JSClass  *jsb_dragonBones_DBCCArmatureNode_class;
JSObject *jsb_dragonBones_DBCCArmatureNode_prototype;

bool js_dragonbones_bindings_DBCCArmatureNode_getAnimation(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DBCCArmatureNode* cobj = (dragonBones::DBCCArmatureNode *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCArmatureNode_getAnimation : Invalid Native Object");
    if (argc == 0) {
        dragonBones::Animation* ret = cobj->getAnimation();
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::Animation>(cx, (dragonBones::Animation*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCArmatureNode_getAnimation : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_DBCCArmatureNode_getCCEventDispatcher(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DBCCArmatureNode* cobj = (dragonBones::DBCCArmatureNode *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCArmatureNode_getCCEventDispatcher : Invalid Native Object");
    if (argc == 0) {
        cocos2d::EventDispatcher* ret = cobj->getCCEventDispatcher();
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<cocos2d::EventDispatcher>(cx, (cocos2d::EventDispatcher*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCArmatureNode_getCCEventDispatcher : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_DBCCArmatureNode_getArmature(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DBCCArmatureNode* cobj = (dragonBones::DBCCArmatureNode *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCArmatureNode_getArmature : Invalid Native Object");
    if (argc == 0) {
        dragonBones::DBCCArmature* ret = cobj->getArmature();
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::DBCCArmature>(cx, (dragonBones::DBCCArmature*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCArmatureNode_getArmature : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_DBCCArmatureNode_getCCSlot(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DBCCArmatureNode* cobj = (dragonBones::DBCCArmatureNode *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCArmatureNode_getCCSlot : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_DBCCArmatureNode_getCCSlot : Error processing arguments");
        dragonBones::DBCCSlot* ret = cobj->getCCSlot(arg0);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::DBCCSlot>(cx, (dragonBones::DBCCSlot*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCArmatureNode_getCCSlot : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_dragonbones_bindings_DBCCArmatureNode_advanceTime(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DBCCArmatureNode* cobj = (dragonBones::DBCCArmatureNode *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCArmatureNode_advanceTime : Invalid Native Object");
    if (argc == 1) {
        double arg0;
        ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[0]), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_DBCCArmatureNode_advanceTime : Error processing arguments");
        cobj->advanceTime(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCArmatureNode_advanceTime : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_dragonbones_bindings_DBCCArmatureNode_initWithDBCCArmature(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DBCCArmatureNode* cobj = (dragonBones::DBCCArmatureNode *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCArmatureNode_initWithDBCCArmature : Invalid Native Object");
    if (argc == 2) {
        dragonBones::DBCCArmature* arg0;
        dragonBones::WorldClock* arg1;
        do {
            if (!argv[0].isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JSObject *tmpObj = JSVAL_TO_OBJECT(argv[0]);
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (dragonBones::DBCCArmature*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        do {
            if (!argv[1].isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JSObject *tmpObj = JSVAL_TO_OBJECT(argv[1]);
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (dragonBones::WorldClock*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_DBCCArmatureNode_initWithDBCCArmature : Error processing arguments");
        bool ret = cobj->initWithDBCCArmature(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCArmatureNode_initWithDBCCArmature : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_dragonbones_bindings_DBCCArmatureNode_getCCDisplay(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DBCCArmatureNode* cobj = (dragonBones::DBCCArmatureNode *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCArmatureNode_getCCDisplay : Invalid Native Object");
    if (argc == 0) {
        cocos2d::Node* ret = cobj->getCCDisplay();
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<cocos2d::Node>(cx, (cocos2d::Node*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCArmatureNode_getCCDisplay : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_DBCCArmatureNode_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    if (argc == 1) {
        dragonBones::DBCCArmature* arg0;
        do {
            if (!argv[0].isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JSObject *tmpObj = JSVAL_TO_OBJECT(argv[0]);
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (dragonBones::DBCCArmature*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_DBCCArmatureNode_create : Error processing arguments");
        dragonBones::DBCCArmatureNode* ret = dragonBones::DBCCArmatureNode::create(arg0);
        jsval jsret = JSVAL_NULL;
        do {
        if (ret) {
            js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::DBCCArmatureNode>(cx, (dragonBones::DBCCArmatureNode*)ret);
            jsret = OBJECT_TO_JSVAL(jsProxy->obj);
        } else {
            jsret = JSVAL_NULL;
        }
    } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    JS_ReportError(cx, "js_dragonbones_bindings_DBCCArmatureNode_create : wrong number of arguments");
    return false;
}

bool js_dragonbones_bindings_DBCCArmatureNode_createWithWorldClock(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    if (argc == 2) {
        dragonBones::DBCCArmature* arg0;
        dragonBones::WorldClock* arg1;
        do {
            if (!argv[0].isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JSObject *tmpObj = JSVAL_TO_OBJECT(argv[0]);
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (dragonBones::DBCCArmature*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        do {
            if (!argv[1].isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JSObject *tmpObj = JSVAL_TO_OBJECT(argv[1]);
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (dragonBones::WorldClock*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_DBCCArmatureNode_createWithWorldClock : Error processing arguments");
        dragonBones::DBCCArmatureNode* ret = dragonBones::DBCCArmatureNode::createWithWorldClock(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        do {
        if (ret) {
            js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::DBCCArmatureNode>(cx, (dragonBones::DBCCArmatureNode*)ret);
            jsret = OBJECT_TO_JSVAL(jsProxy->obj);
        } else {
            jsret = JSVAL_NULL;
        }
    } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    JS_ReportError(cx, "js_dragonbones_bindings_DBCCArmatureNode_createWithWorldClock : wrong number of arguments");
    return false;
}

bool js_dragonbones_bindings_DBCCArmatureNode_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    dragonBones::DBCCArmatureNode* cobj = new (std::nothrow) dragonBones::DBCCArmatureNode();
    TypeTest<dragonBones::DBCCArmatureNode> t;
    js_type_class_t *typeClass = nullptr;
    std::string typeName = t.s_name();
    auto typeMapIter = _js_global_type_map.find(typeName);
    CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
    typeClass = typeMapIter->second;
    CCASSERT(typeClass, "The value is null.");
    JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
    // link the native object with the javascript object
    js_proxy_t* p = jsb_new_proxy(cobj, obj);
    if (JS_HasProperty(cx, obj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", argc, argv);
    return true;
}


extern JSObject *jsb_cocos2d_Node_prototype;

void js_dragonBones_DBCCArmatureNode_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (DBCCArmatureNode)", obj);
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    jsproxy = jsb_get_js_proxy(obj);
    if (jsproxy) {
        nproxy = jsb_get_native_proxy(jsproxy->ptr);

        dragonBones::DBCCArmatureNode *nobj = static_cast<dragonBones::DBCCArmatureNode *>(nproxy->ptr);
        if (nobj)
            delete nobj;
        
        jsb_remove_proxy(nproxy, jsproxy);
    }
}

void js_register_dragonbones_bindings_DBCCArmatureNode(JSContext *cx, JSObject *global) {
    jsb_dragonBones_DBCCArmatureNode_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_dragonBones_DBCCArmatureNode_class->name = "CCArmatureNode";
    jsb_dragonBones_DBCCArmatureNode_class->addProperty = JS_PropertyStub;
    jsb_dragonBones_DBCCArmatureNode_class->delProperty = JS_DeletePropertyStub;
    jsb_dragonBones_DBCCArmatureNode_class->getProperty = JS_PropertyStub;
    jsb_dragonBones_DBCCArmatureNode_class->setProperty = JS_StrictPropertyStub;
    jsb_dragonBones_DBCCArmatureNode_class->enumerate = JS_EnumerateStub;
    jsb_dragonBones_DBCCArmatureNode_class->resolve = JS_ResolveStub;
    jsb_dragonBones_DBCCArmatureNode_class->convert = JS_ConvertStub;
    jsb_dragonBones_DBCCArmatureNode_class->finalize = js_dragonBones_DBCCArmatureNode_finalize;
    jsb_dragonBones_DBCCArmatureNode_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        {"__nativeObj", 0, JSPROP_ENUMERATE | JSPROP_PERMANENT, JSOP_WRAPPER(js_is_native_obj), JSOP_NULLWRAPPER},
        {0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("getAnimation", js_dragonbones_bindings_DBCCArmatureNode_getAnimation, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCCEventDispatcher", js_dragonbones_bindings_DBCCArmatureNode_getCCEventDispatcher, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getArmature", js_dragonbones_bindings_DBCCArmatureNode_getArmature, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCCSlot", js_dragonbones_bindings_DBCCArmatureNode_getCCSlot, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("advanceTime", js_dragonbones_bindings_DBCCArmatureNode_advanceTime, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("initWithDBCCArmature", js_dragonbones_bindings_DBCCArmatureNode_initWithDBCCArmature, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCCDisplay", js_dragonbones_bindings_DBCCArmatureNode_getCCDisplay, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", js_dragonbones_bindings_DBCCArmatureNode_create, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("createWithWorldClock", js_dragonbones_bindings_DBCCArmatureNode_createWithWorldClock, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_dragonBones_DBCCArmatureNode_prototype = JS_InitClass(
        cx, global,
        jsb_cocos2d_Node_prototype,
        jsb_dragonBones_DBCCArmatureNode_class,
        js_dragonbones_bindings_DBCCArmatureNode_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);
    // make the class enumerable in the registered namespace
//  bool found;
//FIXME: Removed in Firefox v27 
//  JS_SetPropertyAttributes(cx, global, "CCArmatureNode", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

    // add the proto and JSClass to the type->js info hash table
    TypeTest<dragonBones::DBCCArmatureNode> t;
    js_type_class_t *p;
    std::string typeName = t.s_name();
    if (_js_global_type_map.find(typeName) == _js_global_type_map.end())
    {
        p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
        p->jsclass = jsb_dragonBones_DBCCArmatureNode_class;
        p->proto = jsb_dragonBones_DBCCArmatureNode_prototype;
        p->parentProto = jsb_cocos2d_Node_prototype;
        _js_global_type_map.insert(std::make_pair(typeName, p));
    }
}

JSClass  *jsb_dragonBones_DBCCFactory_class;
JSObject *jsb_dragonBones_DBCCFactory_prototype;

bool js_dragonbones_bindings_DBCCFactory_buildArmatureNode(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;

    JSObject *obj = NULL;
    dragonBones::DBCCFactory* cobj = NULL;
    obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    cobj = (dragonBones::DBCCFactory *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCFactory_buildArmatureNode : Invalid Native Object");
    do {
        if (argc == 2) {
            std::string arg0;
            ok &= jsval_to_std_string(cx, argv[0], &arg0);
            if (!ok) { ok = true; break; }
            std::string arg1;
            ok &= jsval_to_std_string(cx, argv[1], &arg1);
            if (!ok) { ok = true; break; }
            dragonBones::DBCCArmatureNode* ret = cobj->buildArmatureNode(arg0, arg1);
            jsval jsret = JSVAL_NULL;
            do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::DBCCArmatureNode>(cx, (dragonBones::DBCCArmatureNode*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
            JS_SET_RVAL(cx, vp, jsret);
            return true;
        }
    } while(0);

    do {
        if (argc == 1) {
            std::string arg0;
            ok &= jsval_to_std_string(cx, argv[0], &arg0);
            if (!ok) { ok = true; break; }
            dragonBones::DBCCArmatureNode* ret = cobj->buildArmatureNode(arg0);
            jsval jsret = JSVAL_NULL;
            do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::DBCCArmatureNode>(cx, (dragonBones::DBCCArmatureNode*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
            JS_SET_RVAL(cx, vp, jsret);
            return true;
        }
    } while(0);

    do {
        if (argc == 5) {
            std::string arg0;
            ok &= jsval_to_std_string(cx, argv[0], &arg0);
            if (!ok) { ok = true; break; }
            std::string arg1;
            ok &= jsval_to_std_string(cx, argv[1], &arg1);
            if (!ok) { ok = true; break; }
            std::string arg2;
            ok &= jsval_to_std_string(cx, argv[2], &arg2);
            if (!ok) { ok = true; break; }
            std::string arg3;
            ok &= jsval_to_std_string(cx, argv[3], &arg3);
            if (!ok) { ok = true; break; }
            std::string arg4;
            ok &= jsval_to_std_string(cx, argv[4], &arg4);
            if (!ok) { ok = true; break; }
            dragonBones::DBCCArmatureNode* ret = cobj->buildArmatureNode(arg0, arg1, arg2, arg3, arg4);
            jsval jsret = JSVAL_NULL;
            do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::DBCCArmatureNode>(cx, (dragonBones::DBCCArmatureNode*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
            JS_SET_RVAL(cx, vp, jsret);
            return true;
        }
    } while(0);

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCFactory_buildArmatureNode : wrong number of arguments");
    return false;
}
bool js_dragonbones_bindings_DBCCFactory_hasDragonBones(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DBCCFactory* cobj = (dragonBones::DBCCFactory *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCFactory_hasDragonBones : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_DBCCFactory_hasDragonBones : Error processing arguments");
        bool ret = cobj->hasDragonBones(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        std::string arg1;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= jsval_to_std_string(cx, argv[1], &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_DBCCFactory_hasDragonBones : Error processing arguments");
        bool ret = cobj->hasDragonBones(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 3) {
        std::string arg0;
        std::string arg1;
        std::string arg2;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= jsval_to_std_string(cx, argv[1], &arg1);
        ok &= jsval_to_std_string(cx, argv[2], &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_DBCCFactory_hasDragonBones : Error processing arguments");
        bool ret = cobj->hasDragonBones(arg0, arg1, arg2);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCFactory_hasDragonBones : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_dragonbones_bindings_DBCCFactory_buildArmature(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;

    JSObject *obj = NULL;
    dragonBones::DBCCFactory* cobj = NULL;
    obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    cobj = (dragonBones::DBCCFactory *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCFactory_buildArmature : Invalid Native Object");
    do {
        if (argc == 2) {
            std::string arg0;
            ok &= jsval_to_std_string(cx, argv[0], &arg0);
            if (!ok) { ok = true; break; }
            std::string arg1;
            ok &= jsval_to_std_string(cx, argv[1], &arg1);
            if (!ok) { ok = true; break; }
            dragonBones::DBCCArmature* ret = cobj->buildArmature(arg0, arg1);
            jsval jsret = JSVAL_NULL;
            do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::DBCCArmature>(cx, (dragonBones::DBCCArmature*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
            JS_SET_RVAL(cx, vp, jsret);
            return true;
        }
    } while(0);

    do {
        if (argc == 1) {
            std::string arg0;
            ok &= jsval_to_std_string(cx, argv[0], &arg0);
            if (!ok) { ok = true; break; }
            dragonBones::DBCCArmature* ret = cobj->buildArmature(arg0);
            jsval jsret = JSVAL_NULL;
            do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::DBCCArmature>(cx, (dragonBones::DBCCArmature*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
            JS_SET_RVAL(cx, vp, jsret);
            return true;
        }
    } while(0);

    do {
        if (argc == 5) {
            std::string arg0;
            ok &= jsval_to_std_string(cx, argv[0], &arg0);
            if (!ok) { ok = true; break; }
            std::string arg1;
            ok &= jsval_to_std_string(cx, argv[1], &arg1);
            if (!ok) { ok = true; break; }
            std::string arg2;
            ok &= jsval_to_std_string(cx, argv[2], &arg2);
            if (!ok) { ok = true; break; }
            std::string arg3;
            ok &= jsval_to_std_string(cx, argv[3], &arg3);
            if (!ok) { ok = true; break; }
            std::string arg4;
            ok &= jsval_to_std_string(cx, argv[4], &arg4);
            if (!ok) { ok = true; break; }
            dragonBones::DBCCArmature* ret = cobj->buildArmature(arg0, arg1, arg2, arg3, arg4);
            jsval jsret = JSVAL_NULL;
            do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::DBCCArmature>(cx, (dragonBones::DBCCArmature*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
            JS_SET_RVAL(cx, vp, jsret);
            return true;
        }
    } while(0);

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCFactory_buildArmature : wrong number of arguments");
    return false;
}
bool js_dragonbones_bindings_DBCCFactory_loadTextureAtlas(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DBCCFactory* cobj = (dragonBones::DBCCFactory *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCFactory_loadTextureAtlas : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_DBCCFactory_loadTextureAtlas : Error processing arguments");
        dragonBones::ITextureAtlas* ret = cobj->loadTextureAtlas(arg0);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::ITextureAtlas>(cx, (dragonBones::ITextureAtlas*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        std::string arg1;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= jsval_to_std_string(cx, argv[1], &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_DBCCFactory_loadTextureAtlas : Error processing arguments");
        dragonBones::ITextureAtlas* ret = cobj->loadTextureAtlas(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::ITextureAtlas>(cx, (dragonBones::ITextureAtlas*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCFactory_loadTextureAtlas : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_dragonbones_bindings_DBCCFactory_refreshAllTextureAtlasTexture(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DBCCFactory* cobj = (dragonBones::DBCCFactory *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCFactory_refreshAllTextureAtlasTexture : Invalid Native Object");
    if (argc == 0) {
        cobj->refreshAllTextureAtlasTexture();
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCFactory_refreshAllTextureAtlasTexture : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_dragonbones_bindings_DBCCFactory_refreshTextureAtlasTexture(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DBCCFactory* cobj = (dragonBones::DBCCFactory *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCFactory_refreshTextureAtlasTexture : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_DBCCFactory_refreshTextureAtlasTexture : Error processing arguments");
        cobj->refreshTextureAtlasTexture(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCFactory_refreshTextureAtlasTexture : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_dragonbones_bindings_DBCCFactory_loadDragonBonesData(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    dragonBones::DBCCFactory* cobj = (dragonBones::DBCCFactory *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_dragonbones_bindings_DBCCFactory_loadDragonBonesData : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_DBCCFactory_loadDragonBonesData : Error processing arguments");
        dragonBones::DragonBonesData* ret = cobj->loadDragonBonesData(arg0);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::DragonBonesData>(cx, (dragonBones::DragonBonesData*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        std::string arg1;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= jsval_to_std_string(cx, argv[1], &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_dragonbones_bindings_DBCCFactory_loadDragonBonesData : Error processing arguments");
        dragonBones::DragonBonesData* ret = cobj->loadDragonBonesData(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::DragonBonesData>(cx, (dragonBones::DragonBonesData*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_dragonbones_bindings_DBCCFactory_loadDragonBonesData : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_dragonbones_bindings_DBCCFactory_destroyInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    if (argc == 0) {
        dragonBones::DBCCFactory::destroyInstance();
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }
    JS_ReportError(cx, "js_dragonbones_bindings_DBCCFactory_destroyInstance : wrong number of arguments");
    return false;
}

bool js_dragonbones_bindings_DBCCFactory_getInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    if (argc == 0) {
        dragonBones::DBCCFactory* ret = dragonBones::DBCCFactory::getInstance();
        jsval jsret = JSVAL_NULL;
        do {
        if (ret) {
            js_proxy_t *jsProxy = js_get_or_create_proxy<dragonBones::DBCCFactory>(cx, (dragonBones::DBCCFactory*)ret);
            jsret = OBJECT_TO_JSVAL(jsProxy->obj);
        } else {
            jsret = JSVAL_NULL;
        }
    } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    JS_ReportError(cx, "js_dragonbones_bindings_DBCCFactory_getInstance : wrong number of arguments");
    return false;
}

bool js_dragonbones_bindings_DBCCFactory_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    dragonBones::DBCCFactory* cobj = new (std::nothrow) dragonBones::DBCCFactory();
    TypeTest<dragonBones::DBCCFactory> t;
    js_type_class_t *typeClass = nullptr;
    std::string typeName = t.s_name();
    auto typeMapIter = _js_global_type_map.find(typeName);
    CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
    typeClass = typeMapIter->second;
    CCASSERT(typeClass, "The value is null.");
    JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
    // link the native object with the javascript object
    js_proxy_t* p = jsb_new_proxy(cobj, obj);
    if (JS_HasProperty(cx, obj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", argc, argv);
    return true;
}



void js_dragonBones_DBCCFactory_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (DBCCFactory)", obj);
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    jsproxy = jsb_get_js_proxy(obj);
    if (jsproxy) {
        nproxy = jsb_get_native_proxy(jsproxy->ptr);

        dragonBones::DBCCFactory *nobj = static_cast<dragonBones::DBCCFactory *>(nproxy->ptr);
        if (nobj)
            delete nobj;
        
        jsb_remove_proxy(nproxy, jsproxy);
    }
}

void js_register_dragonbones_bindings_DBCCFactory(JSContext *cx, JSObject *global) {
    jsb_dragonBones_DBCCFactory_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_dragonBones_DBCCFactory_class->name = "CCFactory";
    jsb_dragonBones_DBCCFactory_class->addProperty = JS_PropertyStub;
    jsb_dragonBones_DBCCFactory_class->delProperty = JS_DeletePropertyStub;
    jsb_dragonBones_DBCCFactory_class->getProperty = JS_PropertyStub;
    jsb_dragonBones_DBCCFactory_class->setProperty = JS_StrictPropertyStub;
    jsb_dragonBones_DBCCFactory_class->enumerate = JS_EnumerateStub;
    jsb_dragonBones_DBCCFactory_class->resolve = JS_ResolveStub;
    jsb_dragonBones_DBCCFactory_class->convert = JS_ConvertStub;
    jsb_dragonBones_DBCCFactory_class->finalize = js_dragonBones_DBCCFactory_finalize;
    jsb_dragonBones_DBCCFactory_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        {"__nativeObj", 0, JSPROP_ENUMERATE | JSPROP_PERMANENT, JSOP_WRAPPER(js_is_native_obj), JSOP_NULLWRAPPER},
        {0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("buildArmatureNode", js_dragonbones_bindings_DBCCFactory_buildArmatureNode, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("hasDragonBones", js_dragonbones_bindings_DBCCFactory_hasDragonBones, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("buildArmature", js_dragonbones_bindings_DBCCFactory_buildArmature, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("loadTextureAtlas", js_dragonbones_bindings_DBCCFactory_loadTextureAtlas, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("refreshAllTextureAtlasTexture", js_dragonbones_bindings_DBCCFactory_refreshAllTextureAtlasTexture, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("refreshTextureAtlasTexture", js_dragonbones_bindings_DBCCFactory_refreshTextureAtlasTexture, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("loadDragonBonesData", js_dragonbones_bindings_DBCCFactory_loadDragonBonesData, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("destroyInstance", js_dragonbones_bindings_DBCCFactory_destroyInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getInstance", js_dragonbones_bindings_DBCCFactory_getInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_dragonBones_DBCCFactory_prototype = JS_InitClass(
        cx, global,
        NULL, // parent proto
        jsb_dragonBones_DBCCFactory_class,
        js_dragonbones_bindings_DBCCFactory_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);
    // make the class enumerable in the registered namespace
//  bool found;
//FIXME: Removed in Firefox v27 
//  JS_SetPropertyAttributes(cx, global, "CCFactory", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

    // add the proto and JSClass to the type->js info hash table
    TypeTest<dragonBones::DBCCFactory> t;
    js_type_class_t *p;
    std::string typeName = t.s_name();
    if (_js_global_type_map.find(typeName) == _js_global_type_map.end())
    {
        p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
        p->jsclass = jsb_dragonBones_DBCCFactory_class;
        p->proto = jsb_dragonBones_DBCCFactory_prototype;
        p->parentProto = NULL;
        _js_global_type_map.insert(std::make_pair(typeName, p));
    }
}

void register_all_dragonbones_bindings(JSContext* cx, JSObject* obj) {
    // first, try to get the ns
    JS::RootedValue nsval(cx);
    JS::RootedObject ns(cx);
    JS_GetProperty(cx, obj, "db", &nsval);
    if (nsval == JSVAL_VOID) {
        ns = JS_NewObject(cx, NULL, NULL, NULL);
        nsval = OBJECT_TO_JSVAL(ns);
        JS_SetProperty(cx, obj, "db", nsval);
    } else {
        JS_ValueToObject(cx, nsval, &ns);
    }
    obj = ns;

    js_register_dragonbones_bindings_ITextureAtlas(cx, obj);
    js_register_dragonbones_bindings_Animation(cx, obj);
    js_register_dragonbones_bindings_DragonBonesData(cx, obj);
    js_register_dragonbones_bindings_DBCCFactory(cx, obj);
    js_register_dragonbones_bindings_DBCCArmatureNode(cx, obj);
    js_register_dragonbones_bindings_DBCCArmature(cx, obj);
}

