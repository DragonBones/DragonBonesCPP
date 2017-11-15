#ifndef DRAGONBONES_IEVENT_DISPATCHER_H
#define DRAGONBONES_IEVENT_DISPATCHER_H

#include "../core/DragonBones.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* 事件接口。
* @version DragonBones 4.5
* @language zh_CN
*/
class IEventDispatcher
{
    ABSTRACT_CLASS(IEventDispatcher)

public:
    /**
    * 是否包含指定类型的事件。
    * @param type 事件类型。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    virtual bool hasDBEventListener(const std::string& type) const = 0;
    /**
    * @private
    */
    virtual void dispatchDBEvent(const std::string& type, EventObject* value) = 0;
    /**
    * 添加事件。
    * @param type 事件类型。
    * @param listener 事件回调。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    virtual void addDBEventListener(const std::string& type, const std::function<void(EventObject*)> & listener) = 0;
    /**
    * 移除事件。
    * @param type 事件类型。
    * @param listener 事件回调。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    virtual void removeDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener) = 0;
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_IEVENT_DISPATCHER_H
