#ifndef DBCC_ARMATURE_H
#define DBCC_ARMATURE_H

#include "DragonBonesHeaders.h"
#include "cocos2d.h"
#include "DBCCSlot.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class DBCCArmature : public Armature
{
public:
    virtual cocos2d::Node* getCCDisplay() const;
    virtual cocos2d::EventDispatcher* getCCEventDispatcher() const;
    
public:
    DBCCArmature(ArmatureData *armatureData, Animation *animation, IEventDispatcher *eventDispatcher, cocos2d::Node *display);
    virtual ~DBCCArmature();
    virtual void dispose() override;

    virtual Rectangle getBoundingBox() override;
    virtual cocos2d::Rect getCCBoundingBox();
    
    virtual DBCCSlot* getCCSlot(const std::string &slotName) const;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCArmature);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // DBCC_ARMATURE_H
