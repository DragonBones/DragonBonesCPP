#include "DBCCArmature.h"
#include "DBCCEventDispatcher.h"

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
{
}
DBCCArmature::~DBCCArmature()
{
    dispose();
}
void DBCCArmature::dispose()
{
    if (_display)
    {
        getCCDisplay()->cleanup();
        getCCDisplay()->release();
    }
    
    Armature::dispose();
}

DBCCSlot* DBCCArmature::getCCSlot(const std::string &slotName) const
{
    Slot *slot = getSlot(slotName);
    return slot ? static_cast<DBCCSlot*>(slot) : nullptr;
}
NAME_SPACE_DRAGON_BONES_END