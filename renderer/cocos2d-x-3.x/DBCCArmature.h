#ifndef __DBCC_ARMATURE_H__
#define __DBCC_ARMATURE_H__

#include "../dragonBones/DragonBonesHeaders.h"
#include "cocos2d.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class DBCCSlot;
class DBCCArmature : public Armature
{
protected:
    bool _autoUpdate;
    
public:
    virtual cocos2d::Node *getCCDisplay() const;
    virtual cocos2d::EventDispatcher *getCCEventDispatcher() const;
    
    virtual bool getAutoUpdate() const;
    virtual void setAutoUpdate(bool autoUpdate);
    
public:
    DBCCArmature(ArmatureData *armatureData, Animation *animation, IEventDispatcher *eventDispatcher, cocos2d::Node *display);
    virtual ~DBCCArmature();
    virtual void dispose() override;
    
    virtual DBCCSlot *getCCSlot(const String &slotName) const;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCArmature);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __DBCC_ARMATURE_H__
