#include "DBCCArmature.h"
#include "DBCCEventDispatcher.h"

NAME_SPACE_DRAGON_BONES_BEGIN

cocos2d::Node *DBCCArmature::getDisplay() const
{
    return static_cast<cocos2d::Node *>(_display);
}

cocos2d::EventDispatcher *DBCCArmature::getEventDispatcher() const
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
            getDisplay()->schedule(cocos2d::SEL_SCHEDULE(&Armature::advanceTime), 0);
        }
        else
        {
            getDisplay()->unschedule(cocos2d::SEL_SCHEDULE(&Armature::advanceTime));
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
        getDisplay()->cleanup();
        getDisplay()->release();
    }
    Armature::dispose();
}

NAME_SPACE_DRAGON_BONES_END