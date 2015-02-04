#include "DBCCArmature.h"
#include "DBCCEventDispatcher.h"
#include "DBCCArmatureNode.h"

#if (DRAGON_BONES_ENABLE_LUA == 1)
#include "DBCCLuaUtils.h"
#include "CCLuaEngine.h"
#endif // DRAGON_BONES_ENABLE_LUA

NAME_SPACE_DRAGON_BONES_BEGIN

cocos2d::Node* DBCCArmature::getCCDisplay() const
{
    return static_cast<cocos2d::Node*>(_display);
}

cocos2d::EventDispatcher* DBCCArmature::getCCEventDispatcher() const
{
    return static_cast<DBCCEventDispatcher*>(_eventDispatcher)->eventDispatcher;
}

DBCCArmature::DBCCArmature(ArmatureData *armatureData, Animation *animation, IEventDispatcher *eventDispatcher, cocos2d::Node *display)
    : Armature(armatureData, animation, eventDispatcher, display)
    ,_armatureNode(nullptr)
#if (DRAGON_BONES_ENABLE_LUA == 1)
    ,_frameEventHandler(0)
    ,_animationEventHandler(0)
#endif // DRAGON_BONES_ENABLE_LUA
{
}
DBCCArmature::~DBCCArmature()
{
#if (DRAGON_BONES_ENABLE_LUA == 1)
    unregisterFrameEventHandler();
    unregisterAnimationEventHandler();
#endif // DRAGON_BONES_ENABLE_LUA

    dispose();
}
void DBCCArmature::dispose()
{
    _delayDispose = true;
    if(!_animation || _lockDispose)
    {
        return;
    }

    if (_display)
    {
        getCCDisplay()->cleanup();
        getCCDisplay()->release();
    }

    Armature::dispose();
}

Rectangle DBCCArmature::getBoundingBox()
{
    auto r = getCCBoundingBox();
    return Rectangle(r.origin.x, r.origin.y, r.size.width, r.size.height);
}

cocos2d::Rect DBCCArmature::getCCBoundingBox()
{
    float minx = 0, miny = 0, maxx = 0, maxy = 0;
    bool first = true;
    for (const auto slot : _slotList)
    {
        if (!slot->getVisible() || !slot->isShowDisplay()) { continue; }
        auto r = slot->getBoundingBox();
        if (first)
        {
            first = false;
            minx = r.x;
            miny = r.y;
            maxx = r.x + r.width;
            maxy = r.y + r.height;
        }
        else
        {
            minx = r.x < minx ? r.x : minx;
            miny = r.y < miny ? r.y : miny;
            maxx = r.x + r.width > maxx ? r.x + r.width : maxx;
            maxy = r.y + r.height > maxy ? r.y + r.height : maxy;
        }
    }
    cocos2d::Rect rect(minx, miny, maxx - minx, maxy - miny);
    return cocos2d::RectApplyTransform(rect, getCCDisplay()->getNodeToParentTransform());
}

DBCCSlot* DBCCArmature::getCCSlot(const std::string &slotName) const
{
    Slot *slot = getSlot(slotName);
    return slot ? static_cast<DBCCSlot*>(slot) : nullptr;
}

void DBCCArmature::sortSlotsByZOrder()
{
    std::sort(_slotList.begin() , _slotList.end() , sortSlot);

    int nShowCount = 0;
    int nDisplayChildrenCount = static_cast<cocos2d::Node*>(getCCDisplay())->getChildrenCount();

    for (size_t i = 0, l = _slotList.size(); i < l; ++i)
    {
        Slot *slot = _slotList[i];
        if (slot->isShowDisplay())
        {
            cocos2d::Node* slotDisplayNode = static_cast<cocos2d::Node*>(slot->getDisplay());
            if (slotDisplayNode)
            {
                slotDisplayNode->setLocalZOrder(nDisplayChildrenCount + nShowCount);
            }
            nShowCount += 1;
        }
    }
    _slotsZOrderChanged = false;
}

DBCCArmatureNode* DBCCArmature::getArmatureNode() const
{
    return _armatureNode;
}
void DBCCArmature::setArmatureNode(DBCCArmatureNode *armatureNode)
{
    _armatureNode = armatureNode;
}



///////////////////////////////////////////////////////////////////////
/////////////// for lua
///////////////////////////////////////////////////////////////////////

#if (DRAGON_BONES_ENABLE_LUA == 1)

void DBCCArmature::registerFrameEventHandler(cocos2d::LUA_FUNCTION func)
{
    unregisterFrameEventHandler();
    _frameEventHandler = func;

    auto dispatcher = getCCEventDispatcher();

    auto f = [this](cocos2d::EventCustom *event)
    {
        auto eventData = (EventData*)(event->getUserData());
        DBCCLuaUtils::pushEventData(eventData, _armatureNode);
        auto stack = cocos2d::LuaEngine::getInstance()->getLuaStack();
        stack->executeFunctionByHandler(_frameEventHandler, 1);
    };

    dispatcher->addCustomEventListener(EventData::ANIMATION_FRAME_EVENT, f);
    dispatcher->addCustomEventListener(EventData::BONE_FRAME_EVENT, f);
}

void DBCCArmature::registerAnimationEventHandler(cocos2d::LUA_FUNCTION func)
{
    unregisterAnimationEventHandler();
    _animationEventHandler = func;

    auto dispatcher = getCCEventDispatcher();

    auto f = [this](cocos2d::EventCustom *event)
    {
        auto eventData = (EventData*)(event->getUserData());
        DBCCLuaUtils::pushEventData(eventData, _armatureNode);
        auto stack = cocos2d::LuaEngine::getInstance()->getLuaStack();
        stack->executeFunctionByHandler(_animationEventHandler, 1);
    };

    dispatcher->addCustomEventListener(EventData::COMPLETE, f);
    dispatcher->addCustomEventListener(EventData::LOOP_COMPLETE, f);
}

void DBCCArmature::unregisterFrameEventHandler()
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

void DBCCArmature::unregisterAnimationEventHandler()
{
    if (_animationEventHandler != 0)
    {
        auto dispatcher = getCCEventDispatcher();
        dispatcher->removeCustomEventListeners(EventData::COMPLETE);
        dispatcher->removeCustomEventListeners(EventData::LOOP_COMPLETE);
        cocos2d::LuaEngine::getInstance()->removeScriptHandler(_animationEventHandler);
        _animationEventHandler = 0;
    }
}

#endif // !DRAGON_BONES_ENABLE_LUA


NAME_SPACE_DRAGON_BONES_END