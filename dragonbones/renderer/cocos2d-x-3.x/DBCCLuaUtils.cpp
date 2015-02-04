#include "DBCCLuaUtils.h"

#if (DRAGON_BONES_ENABLE_LUA == 1)

#include "DBCCArmature.h"
#include "DBCCArmatureNode.h"
#include "cocos2d.h"
#include "CCLuaEngine.h"
#include "LuaBasicConversions.h"

extern "C" {
#include "lua.h"
}

NAME_SPACE_DRAGON_BONES_BEGIN

void DBCCLuaUtils::pushEventData(EventData *eventData, DBCCArmatureNode *armatureNode)
{
    auto type = (int) eventData->getType();
    auto animationName = eventData->animationState->name;
    auto lastState = eventData->armature->getAnimation()->getLastAnimationState();
    auto isLastAnimation = lastState == eventData->animationState;

    auto stack = cocos2d::LuaEngine::getInstance()->getLuaStack();
    auto L = stack->getLuaState();
    lua_newtable(L);                                            /* L: table */

    if (armatureNode)
    {
        pushObjectKV<DBCCArmatureNode>(L, "armatureNode", armatureNode, "db.DBCCArmatureNode");
    }
    // DBCCArmature
    pushObjectKV<DBCCArmature>(L, "armature", (DBCCArmature*)eventData->armature, "db.DBCCArmature");
    LUA_PUSH_BASE_KV(L, "frameLabel", eventData->frameLabel.c_str(), lua_pushstring);
    LUA_PUSH_BASE_KV(L, "type", type, lua_pushinteger);
    LUA_PUSH_BASE_KV(L, "animationName", animationName.c_str(), lua_pushstring);
    LUA_PUSH_BASE_KV(L, "isLastAnimation", isLastAnimation, lua_pushboolean);
    if (eventData->bone)
    {
        LUA_PUSH_BASE_KV(L, "boneName", eventData->bone->name.c_str(), lua_pushstring);
    }
}

template<class T>
void DBCCLuaUtils::pushObjectKV(lua_State *L, const char *key, T* value, const char* type)
{
    lua_pushstring(L, key);
    object_to_luaval<T>(L, type,(T*)value);
    lua_rawset(L, -3);
}

NAME_SPACE_DRAGON_BONES_END

#endif // !DRAGON_BONES_ENABLE_LUA