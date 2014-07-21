#ifndef __DBCC_ARMATURE_H__
#define __DBCC_ARMATURE_H__

#include "../dragonBones/DragonBonesHeaders.h"
#include "cocos2d.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class DBCCArmature : public Armature
{
public:
    DBCCArmature(ArmatureData *armatureData, Animation *animation, IEventDispatcher *eventDispatcher, void *display)
        : Armature(armatureData, animation, eventDispatcher, display)
    {
    }
    virtual ~DBCCArmature()
    {
        dispose();
    }
    virtual void dispose()
    {
        if (_display)
        {
            cocos2d::Sprite *spriteDisplay = (cocos2d::Sprite *)(spriteDisplay);
            spriteDisplay->cleanup();
        }
        
        Armature::dispose();
    }
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCArmature);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __DBCC_ARMATURE_H__
