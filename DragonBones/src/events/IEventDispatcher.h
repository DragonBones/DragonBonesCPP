#ifndef DRAGONBONES_I_EVENT_DISPATCHER_H
#define DRAGONBONES_I_EVENT_DISPATCHER_H

#include "../core/DragonBones.h"

NAMESPACE_DRAGONBONES_BEGIN

class EventObject;

class IEventDispatcher 
{
public:
    IEventDispatcher() {};
    virtual ~IEventDispatcher() {};

    virtual void _dispatchEvent(EventObject* value) = 0;
    virtual bool hasEvent(const std::string& type) const = 0;
    virtual void addEvent(const std::string& type) = 0;
    virtual void removeEvent(const std::string& type) = 0;
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_I_EVENT_DISPATCHER_H