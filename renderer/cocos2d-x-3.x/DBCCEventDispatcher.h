#ifndef __DBCC_EVENT_DISPATCHER_H__
#define __DBCC_EVENT_DISPATCHER_H__

#include "../dragonBones/DragonBonesHeaders.h"

#include "CCEventDispatcher.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class DBCCEventDispatcher : public IEventDispatcher
{
public:
    cocos2d::EventDispatcher eventDispatcher;
    
public:
    DBCCEventDispatcher()
    {
    }
    virtual ~DBCCEventDispatcher()
    {
        dispose();
    }
    virtual void dispose() override
    {
        eventDispatcher.removeAllEventListeners();
    }
    
    virtual void dispatchEvent(EventData *eventData) override
    {
        eventDispatcher.dispatchCustomEvent(EventData::getStringType(eventData->getType()), eventData);
    }
    
    virtual bool hasEvent(EventData::EventDataType eventDataType) const override
    {
        return true;
    }
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCEventDispatcher);
};
NAME_SPACE_DRAGON_BONES_END

#endif  // __DBCC_EVENT_DISPATCHER_H__