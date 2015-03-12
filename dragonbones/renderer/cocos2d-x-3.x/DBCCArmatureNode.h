#ifndef DBCC_ARMATURE_NODE_H
#define DBCC_ARMATURE_NODE_H

#include "dbccMacro.h"
#include "DragonBonesHeaders.h"
#include "cocos2d.h"
#include "DBCCSlot.h"
#include "DBCCArmature.h"

#if (DRAGON_BONES_ENABLE_LUA == 1)
#include "CCLuaValue.h"
#endif // DRAGON_BONES_ENABLE_LUA



NAME_SPACE_DRAGON_BONES_BEGIN
class DBCCArmatureNode : public cocos2d::Node, public IAnimatable
{
public:
    virtual DBCCSlot* getCCSlot(const std::string &slotName) const { return _armature->getCCSlot(slotName); };
	virtual cocos2d::Node* getCCDisplay() const { return _armature->getCCDisplay(); };
	virtual cocos2d::EventDispatcher* getCCEventDispatcher() const { return _armature->getCCEventDispatcher(); };
	virtual cocos2d::Rect getBoundingBox() const override;
    virtual cocos2d::Rect getInnerBoundingBox() const;

#if (DRAGON_BONES_ENABLE_LUA == 1)
	virtual void registerFrameEventHandler(cocos2d::LUA_FUNCTION func);
	virtual void registerAnimationEventHandler(cocos2d::LUA_FUNCTION func);
	virtual void unregisterFrameEventHandler();
	virtual void unregisterAnimationEventHandler();
#endif // DRAGON_BONES_ENABLE_LUA


public:
    // create DBCCArmatureNode without WorldClock
    static DBCCArmatureNode* create(DBCCArmature *armature);
    /**
     * create DDCCArmatureNode with WorldClock
     * @param armature
     * @param clock if null, use WorldClock::getInstance()
     * @return 
     */
    static DBCCArmatureNode* createWithWorldClock(DBCCArmature *armature, WorldClock *clock);
    virtual bool initWithDBCCArmature(DBCCArmature *armature, WorldClock *clock);
    
    DBCCArmatureNode();
    virtual ~DBCCArmatureNode();

	DBCCArmature* getArmature() const { return _armature; };
	Animation* getAnimation() const { return _armature->getAnimation(); };

	virtual void update(float dt) override;
    virtual void advanceTime(float dt) override;

protected:
    DBCCArmature *_armature;
    WorldClock *_clock;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCArmatureNode);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // DBCC_ARMATURE_NODE_H
