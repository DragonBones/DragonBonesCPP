#ifndef __EVENTS_I_EVENT_DISPATCHER_H__
#define __EVENTS_I_EVENT_DISPATCHER_H__

#include "../DragonBones.h"
#include "EventData.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class IEventDispatcher
{
public:
    IEventDispatcher() {}
    virtual ~IEventDispatcher() {};
    virtual void dispose() = 0;
    
    virtual void dispatchEvent(EventData *eventData) = 0;
    virtual bool hasEvent(EventData::EventDataType eventDataType) const = 0;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(IEventDispatcher);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __EVENTS_I_EVENT_DISPATCHER_H__
