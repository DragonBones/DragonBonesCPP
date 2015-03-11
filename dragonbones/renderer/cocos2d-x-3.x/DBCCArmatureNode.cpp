#include "dbccMacro.h"
#include "DBCCArmatureNode.h"


NAME_SPACE_DRAGON_BONES_BEGIN

DBCCArmatureNode* DBCCArmatureNode::create(DBCCArmature *armature)
{
    DBCCArmatureNode *ret = new DBCCArmatureNode();
    
    if (ret && ret->initWithDBCCArmature(armature, nullptr))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}

DBCCArmatureNode* DBCCArmatureNode::createWithWorldClock(DBCCArmature *armature, WorldClock *clock)
{
    DBCCArmatureNode *ret = new DBCCArmatureNode();
    if (!clock)
        clock = WorldClock::getInstance();

    if (ret && ret->initWithDBCCArmature(armature, clock))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool DBCCArmatureNode::initWithDBCCArmature(DBCCArmature *armature, WorldClock *clock)
{
    if (armature != nullptr)
    {
        _armature = armature;
        _armature->setArmatureNode(this);
        _clock = clock;
        if (clock)
        {
            clock->add(this);
        }else
        {
            scheduleUpdate();
        }
        addChild(armature->getCCDisplay());
        setCascadeOpacityEnabled(true);
        setCascadeColorEnabled(true);
        return true;
    }

	return false;
}


DBCCArmatureNode::DBCCArmatureNode()
    :_armature(nullptr)
    ,_clock(nullptr)
{
}
DBCCArmatureNode::~DBCCArmatureNode()
{
    if (_clock)
    {
        _clock->remove(this);
        _clock = nullptr;
    } else 
    {
        unscheduleUpdate();
    }

    if (_armature)
    {
        delete _armature;
        _armature = nullptr;
    }
}

cocos2d::Rect DBCCArmatureNode::getBoundingBox() const
{
    auto rect = _armature->getCCBoundingBox();
    return cocos2d::RectApplyTransform(rect, getNodeToParentTransform());
}

cocos2d::Rect DBCCArmatureNode::getInnerBoundingBox() const
{
    return _armature->getCCBoundingBox();
}

void DBCCArmatureNode::update(float dt)
{
    DBASSERT(!_clock, "can not has clock when update!");
    advanceTime(dt);
}

void DBCCArmatureNode::advanceTime(float dt)
{
    retain();
    _armature->advanceTime(dt);
    release();
}

///////////////////////////////////////////////////////////////////////
/////////////// for lua
///////////////////////////////////////////////////////////////////////

#if (DRAGON_BONES_ENABLE_LUA == 1)

void DBCCArmatureNode::registerFrameEventHandler(cocos2d::LUA_FUNCTION func)
{
	_armature->registerFrameEventHandler(func);
}

void DBCCArmatureNode::registerAnimationEventHandler(cocos2d::LUA_FUNCTION func)
{
	_armature->registerAnimationEventHandler(func);
}

void DBCCArmatureNode::unregisterFrameEventHandler()
{
	_armature->unregisterFrameEventHandler();
}

void DBCCArmatureNode::unregisterAnimationEventHandler()
{
	_armature->unregisterAnimationEventHandler();
}

#endif // !DRAGON_BONES_ENABLE_LUA


NAME_SPACE_DRAGON_BONES_END
