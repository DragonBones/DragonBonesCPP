#ifndef DBCC_EVENT_DISPATCHER_H
#define DBCC_EVENT_DISPATCHER_H

#include "DragonBonesHeaders.h"
#include "cocos2d.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class DBCCEventDispatcher : public IEventDispatcher
{
public:
    cocos2d::EventDispatcher *eventDispatcher;
    
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
        if (eventDispatcher)
        {
            eventDispatcher->removeAllEventListeners();
            eventDispatcher->setEnabled(false);
            delete eventDispatcher;
            eventDispatcher = nullptr;
        }
    }
    
    virtual void dispatchEvent(EventData *eventData) override
    {
        if (eventDispatcher)
        {
            eventDispatcher->dispatchCustomEvent(eventData->getStringType(), eventData);
        }
    }
    
    virtual bool hasEvent(EventData::EventType eventDataType) const override
    {
        return eventDispatcher != nullptr;
    }
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCEventDispatcher);
};
NAME_SPACE_DRAGON_BONES_END

#endif  // DBCC_EVENT_DISPATCHER_H