#include "DBCCArmature.h"
#include "DBCCEventDispatcher.h"
#include "DBCCSlot.h"

NAME_SPACE_DRAGON_BONES_BEGIN

cocos2d::Node *DBCCArmature::getCCDisplay() const
{
    return static_cast<cocos2d::Node *>(_display);
}

cocos2d::EventDispatcher *DBCCArmature::getCCEventDispatcher() const
{
    return static_cast<DBCCEventDispatcher *>(_eventDispatcher)->eventDispatcher;
}

bool DBCCArmature::getAutoUpdate() const
{
    return _autoUpdate;
}
void DBCCArmature::setAutoUpdate(bool autoUpdate)
{
    if (_autoUpdate == autoUpdate)
    {
        return;
    }
    _autoUpdate = autoUpdate;
    if (_display)
    {
        if (_autoUpdate)
        {
            getCCDisplay()->schedule(cocos2d::SEL_SCHEDULE(&Armature::advanceTime), 0);
        }
        else
        {
            getCCDisplay()->unschedule(cocos2d::SEL_SCHEDULE(&Armature::advanceTime));
        }
    }
}

DBCCArmature::DBCCArmature(ArmatureData *armatureData, Animation *animation, IEventDispatcher *eventDispatcher, cocos2d::Node *display)
    : Armature(armatureData, animation, eventDispatcher, display)
{
    _autoUpdate = false;
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

DBCCSlot *DBCCArmature::getCCSlot(const String &slotName) const
{
    Slot *slot = getSlot(slotName);
    return slot ? static_cast<DBCCSlot *>(slot) : nullptr;
}

NAME_SPACE_DRAGON_BONES_END