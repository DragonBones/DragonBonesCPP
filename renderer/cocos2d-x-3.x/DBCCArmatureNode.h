#ifndef __DBCC_ARMATURE_NODE_H__
#define __DBCC_ARMATURE_NODE_H__

#include "DragonBonesHeaders.h"
#include "cocos2d.h"
#include "DBCCSlot.h"
#include "DBCCArmature.h"

#if (DRAGON_BONES_ENABLE_LUA == 1)
#include "CCLuaValue.h"
#endif // DRAGON_BONES_ENABLE_LUA



NAME_SPACE_DRAGON_BONES_BEGIN
class DBCCArmatureNode : public cocos2d::Node
{
public:
	virtual cocos2d::Node *getCCDisplay() const { return _armature->getCCDisplay(); };
	virtual cocos2d::EventDispatcher *getCCEventDispatcher() const { return _armature->getCCEventDispatcher(); };
	virtual cocos2d::Rect getBoundingBox() const override;

#if (DRAGON_BONES_ENABLE_LUA == 1)
	virtual void registerFrameEventHandler(cocos2d::LUA_FUNCTION func);
	virtual void registerMovementEventHandler(cocos2d::LUA_FUNCTION func);
	virtual void unregisterFrameEventHandler();
	virtual void unregisterMovementEventHandler();
#endif // DRAGON_BONES_ENABLE_LUA

public:

	static DBCCArmatureNode* create(DBCCArmature* armature);
	virtual bool initWithDBCCArmature(DBCCArmature* armature);

    DBCCArmatureNode();
    virtual ~DBCCArmatureNode();
	DBCCArmature* getArmature() const { return _armature; };
	Animation* getAnimation() const { return _armature->getAnimation(); };

	virtual void update(float dt) override;

protected:
	DBCCArmature* _armature;
    
private:
	int _frameEventHandler;
	int _movementEventHandler;
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCArmatureNode);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __DBCC_ARMATURE_NODE_H__
