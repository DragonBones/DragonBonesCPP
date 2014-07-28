#ifndef __DBCC_ARMATURE_H__
#define __DBCC_ARMATURE_H__

#include "../dragonBones/DragonBonesHeaders.h"
#include "cocos2d.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class DBCCArmature : public Armature
{
protected:
    bool _autoUpdate;
    
public:
    virtual cocos2d::Node *getDisplay() const;
    virtual cocos2d::EventDispatcher *getEventDispatcher() const;
    
    virtual bool getAutoUpdate() const;
    virtual void setAutoUpdate(bool autoUpdate);
    
public:
    DBCCArmature(ArmatureData *armatureData, Animation *animation, IEventDispatcher *eventDispatcher, cocos2d::Node *display);
    virtual ~DBCCArmature();
    virtual void dispose() override;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCArmature);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __DBCC_ARMATURE_H__
