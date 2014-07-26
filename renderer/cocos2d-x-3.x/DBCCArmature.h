#ifndef __DBCC_ARMATURE_H__
#define __DBCC_ARMATURE_H__

#include "../dragonBones/DragonBonesHeaders.h"
#include "cocos2d.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class DBCCArmature : public Armature
{
public:
    cocos2d::Node *getDisplay() const
    {
        return static_cast<cocos2d::Node *>(_display);
    }
    
public:
    DBCCArmature(ArmatureData *armatureData, Animation *animation, IEventDispatcher *eventDispatcher, cocos2d::Node *display)
        : Armature(armatureData, animation, eventDispatcher, display)
    {
    }
    virtual ~DBCCArmature()
    {
        dispose();
    }
    virtual void dispose() override
    {
        if (_display)
        {
            getDisplay()->cleanup();
            getDisplay()->release();
        }
        Armature::dispose();
    }
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCArmature);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __DBCC_ARMATURE_H__
