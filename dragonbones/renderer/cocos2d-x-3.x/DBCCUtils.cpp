#include "DBCCUtils.h"

#include "DBCCArmature.h"
#include "cocos2d.h"
//#include "renderer/CCGLProgramState.h"

USING_NS_CC;

NAME_SPACE_DRAGON_BONES_BEGIN

void DBCCUtils::setAllBlendFunc(DBCCArmature *armature, int src, int dest)
{
    if (!armature) return;

    auto &list = armature->getSlots();
    for (auto slot : list)
    {
        if(!slot) continue;

        for (auto &entry : slot->getDisplayList())
        {
            if (entry.second == DisplayType::DT_ARMATURE)
            {
                setAllBlendFunc((DBCCArmature*)(entry.first), src, dest);
            } else
            {
                auto node = (Node*)(entry.first);
                auto sprite = dynamic_cast<Sprite*>(node);
                if (sprite)
                {
                    BlendFunc func = {(GLenum)src, (GLenum)dest};
                    sprite->setBlendFunc(func);
                }
            }
        }
    }
}

void DBCCUtils::setAllGLParamState(DBCCArmature *armature, cocos2d::GLProgramState *state)
{
    if (!armature) return;

    auto &list = armature->getSlots();
    for (auto slot : list)
    {
        if(!slot) continue;

        for (auto &entry : slot->getDisplayList())
        {
            if (entry.second == DisplayType::DT_ARMATURE)
            {
                setAllGLParamState((DBCCArmature*)(entry.first), state);
            } else
            {
                auto node = (Node*)(entry.first);
                node->setGLProgramState(state);
            }
        }
    }
}


NAME_SPACE_DRAGON_BONES_END