#include "DBCCArmatureNode.h"
#include "DBCCEventDispatcher.h"

#if (DRAGON_BONES_ENABLE_LUA == 1)
#include "CCLuaEngine.h"
#endif // DRAGON_BONES_ENABLE_LUA


NAME_SPACE_DRAGON_BONES_BEGIN

DBCCArmatureNode* DBCCArmatureNode::create(DBCCArmature *armature)
{
    DBCCArmatureNode *ret = new DBCCArmatureNode();
    
    if (ret && ret->initWithDBCCArmature(armature))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}

bool DBCCArmatureNode::initWithDBCCArmature(DBCCArmature *armature)
{
    if (armature != nullptr)
    {
        _armature = armature;
        addChild(armature->getCCDisplay());
        scheduleUpdate();
        setCascadeOpacityEnabled(true);
        setCascadeColorEnabled(true);
        return true;
    }

	return false;
}


DBCCArmatureNode::DBCCArmatureNode()
	:_armature(nullptr)
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

    if (_armature)
    {
        _armature->dispose();
        _armature = nullptr;
    }
}

cocos2d::Rect DBCCArmatureNode::getBoundingBox() const
{
    float minx = 0, miny = 0, maxx = 0, maxy = 0;
    bool first = true;
    
    for (const auto slot : _armature->getSlots())
    {
        if (!slot->getVisible() || !slot->isShowDisplay()) { continue; }
        
        auto node = static_cast<cocos2d::Node*>(slot->getDisplay());
        
        auto r = node->getBoundingBox();
        
        if (first)
        {
            minx = r.getMinX();
            miny = r.getMinY();
            maxx = r.getMaxX();
            maxy = r.getMaxY();
            first = false;
        }
        else
        {
            minx = r.getMinX() < minx ? r.getMinX() : minx;
            miny = r.getMinY() < miny ? r.getMinY() : miny;
            maxx = r.getMaxX() > maxx ? r.getMaxX() : maxx;
            maxy = r.getMaxY() > maxy ? r.getMaxY() : maxy;
        }
    }

    auto display = _armature->getCCDisplay();
    cocos2d::Rect rect(minx, miny, maxx - minx, maxy - miny);
    return cocos2d::RectApplyTransform(rect, display->getNodeToParentTransform());
}

void DBCCArmatureNode::update(float dt)
{
	retain();
	getArmature()->advanceTime(dt);
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
		auto eventData = (dragonBones::EventData*)(event->getUserData());
		auto type = (int) eventData->getType();
		auto movementId = eventData->animationState->name;
		auto frameLabel = eventData->frameLabel;

		auto stack = cocos2d::LuaEngine::getInstance()->getLuaStack();
		stack->pushObject(this, "db.DBCCArmatureNode");
		stack->pushInt(type);
        stack->pushString(movementId.c_str(), movementId.size());
        stack->pushString(frameLabel.c_str(), frameLabel.size());
		stack->executeFunctionByHandler(_frameEventHandler, 4);
	};

	dispatcher->addCustomEventListener(dragonBones::EventData::ANIMATION_FRAME_EVENT, f);
}

void DBCCArmatureNode::registerMovementEventHandler(cocos2d::LUA_FUNCTION func)
{
	unregisterMovementEventHandler();
	_movementEventHandler = func;

	auto dispatcher = getCCEventDispatcher();

	auto f = [this](cocos2d::EventCustom *event)
	{
		auto eventData = (dragonBones::EventData*)(event->getUserData());
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

	dispatcher->addCustomEventListener(dragonBones::EventData::COMPLETE, f);
	dispatcher->addCustomEventListener(dragonBones::EventData::LOOP_COMPLETE, f);
}

void DBCCArmatureNode::unregisterFrameEventHandler()
{
	if (_frameEventHandler != 0)
	{
		cocos2d::LuaEngine::getInstance()->removeScriptHandler(_frameEventHandler);
		_frameEventHandler = 0;
	}
}

void DBCCArmatureNode::unregisterMovementEventHandler()
{
	if (_movementEventHandler != 0)
	{
		cocos2d::LuaEngine::getInstance()->removeScriptHandler(_movementEventHandler);
		_movementEventHandler = 0;
	}
}

#endif // !DRAGON_BONES_ENABLE_LUA


NAME_SPACE_DRAGON_BONES_END