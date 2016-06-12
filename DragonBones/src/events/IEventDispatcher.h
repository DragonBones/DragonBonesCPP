#ifndef DRAGONBONES_I_EVENT_DISPATCHER_H
#define DRAGONBONES_I_EVENT_DISPATCHER_H

#include "../core/DragonBones.h"

DRAGONBONES_NAMESPACE_BEGIN

class EventObject;

class IEventDispatcher 
{
protected:
    IEventDispatcher() {};
    virtual ~IEventDispatcher() {};

public:
    /** @private */
    virtual void _onClear() = 0;
    /** @private */
    virtual void _dispatchEvent(EventObject* value) = 0;

    virtual bool hasEvent(const std::string& type) const = 0;
    virtual void addEvent(const std::string& type) = 0;
    virtual void removeEvent(const std::string& type) = 0;
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_I_EVENT_DISPATCHER_H