#ifndef DBCC_ARMATURE_H
#define DBCC_ARMATURE_H


#include "dbccMacro.h"
#include "DragonBonesHeaders.h"
#include "DBCCSlot.h"
#include "cocos2d.h"

#if (DRAGON_BONES_ENABLE_LUA == 1)
#include "CCLuaValue.h"
#endif // DRAGON_BONES_ENABLE_LUA

NAME_SPACE_DRAGON_BONES_BEGIN

class DBCCArmatureNode;

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

    virtual DBCCArmatureNode* getArmatureNode() const;
    virtual void setArmatureNode(DBCCArmatureNode *armatureNode);

#if (DRAGON_BONES_ENABLE_LUA == 1)
    virtual void registerFrameEventHandler(cocos2d::LUA_FUNCTION func);
    virtual void registerAnimationEventHandler(cocos2d::LUA_FUNCTION func);
    virtual void unregisterFrameEventHandler();
    virtual void unregisterAnimationEventHandler();
#endif // DRAGON_BONES_ENABLE_LUA
	
protected:
	virtual void sortSlotsByZOrder() override;	
    
private:
    DBCCArmatureNode *_armatureNode;
#if (DRAGON_BONES_ENABLE_LUA == 1)
    int _frameEventHandler;
    int _animationEventHandler;
#endif // DRAGON_BONES_ENABLE_LUA
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCArmature);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // DBCC_ARMATURE_H
