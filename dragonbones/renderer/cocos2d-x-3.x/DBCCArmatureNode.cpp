#include "dbccMacro.h"
#include "DBCCArmatureNode.h"
#include "DBCCEventDispatcher.h"


#if (DRAGON_BONES_ENABLE_LUA == 1)
#include "CCLuaEngine.h"
#endif // DRAGON_BONES_ENABLE_LUA


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
	,_frameEventHandler(0)
	,_movementEventHandler(0)
{
}
DBCCArmatureNode::~DBCCArmatureNode()
{
#if (DRAGON_BONES_ENABLE_LUA == 1)
	unregisterFrameEventHandler();
	unregisterMovementEventHandler();
#endif // DRAGON_BONES_ENABLE_LUA

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
    unregisterFrameEventHandler();
    _frameEventHandler = func;

    auto dispatcher = getCCEventDispatcher();

    auto f = [this](cocos2d::EventCustom *event)
    {
        auto eventData = (EventData*)(event->getUserData());
        auto type = (int) eventData->getType();
        auto movementId = eventData->animationState->name;
        auto frameLabel = eventData->frameLabel;

        auto stack = cocos2d::LuaEngine::getInstance()->getLuaStack();
        stack->pushObject(this, "db.DBCCArmatureNode");
        stack->pushInt(type);
        stack->pushString(movementId.c_str(), movementId.size());
        stack->pushString(frameLabel.c_str(), frameLabel.size());
        if (eventData->getType() == EventData::EventType::BONE_FRAME_EVENT)
        {
            stack->pushString(eventData->bone->name.c_str(), eventData->bone->name.size());
            stack->executeFunctionByHandler(_frameEventHandler, 5);
        } else 
        {
            stack->executeFunctionByHandler(_frameEventHandler, 4);
        }
    };

    dispatcher->addCustomEventListener(EventData::ANIMATION_FRAME_EVENT, f);
    dispatcher->addCustomEventListener(EventData::BONE_FRAME_EVENT, f);
}

void DBCCArmatureNode::registerMovementEventHandler(cocos2d::LUA_FUNCTION func)
{
    unregisterMovementEventHandler();
    _movementEventHandler = func;

    auto dispatcher = getCCEventDispatcher();

    auto f = [this](cocos2d::EventCustom *event)
    {
        auto eventData = (EventData*)(event->getUserData());
        auto type = (int) eventData->getType();
        auto movementId = eventData->animationState->name;
        auto lastState = eventData->armature->getAnimation()->getLastAnimationState();

        auto stack = cocos2d::LuaEngine::getInstance()->getLuaStack();
        stack->pushObject(this, "db.DBCCArmatureNode");
        stack->pushInt(type);
        stack->pushString(movementId.c_str(), movementId.size());
        stack->pushBoolean(lastState == eventData->animationState);

        stack->executeFunctionByHandler(_movementEventHandler, 4);
    };

    dispatcher->addCustomEventListener(EventData::COMPLETE, f);
    dispatcher->addCustomEventListener(EventData::LOOP_COMPLETE, f);
}

void DBCCArmatureNode::unregisterFrameEventHandler()
{
    if (_frameEventHandler != 0)
    {
        auto dispatcher = getCCEventDispatcher();
        dispatcher->removeCustomEventListeners(EventData::ANIMATION_FRAME_EVENT);
        dispatcher->removeCustomEventListeners(EventData::BONE_FRAME_EVENT);
        cocos2d::LuaEngine::getInstance()->removeScriptHandler(_frameEventHandler);
        _frameEventHandler = 0;
    }
}

void DBCCArmatureNode::unregisterMovementEventHandler()
{
    if (_movementEventHandler != 0)
    {
        auto dispatcher = getCCEventDispatcher();
        dispatcher->removeCustomEventListeners(EventData::COMPLETE);
        dispatcher->removeCustomEventListeners(EventData::LOOP_COMPLETE);
        cocos2d::LuaEngine::getInstance()->removeScriptHandler(_movementEventHandler);
        _movementEventHandler = 0;
    }
}

#endif // !DRAGON_BONES_ENABLE_LUA


NAME_SPACE_DRAGON_BONES_END
