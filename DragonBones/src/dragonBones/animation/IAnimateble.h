#ifndef DRAGONBONES_ANIMATEBLE_H
#define DRAGONBONES_ANIMATEBLE_H

#include "../core/DragonBones.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
 * @language zh_CN
 * 播放动画接口。 (Armature 和 WordClock 都实现了该接口)
 * 任何实现了此接口的实例都可以加到 WorldClock 实例中，由 WorldClock 统一更新时间。
 * @see dragonBones.WorldClock
 * @see dragonBones.Armature
 * @version DragonBones 3.0
 */
class IAnimateble
{
    ABSTRACT_CLASS(IAnimateble)

public:
    /**
     * @language zh_CN
     * 更新时间。
     * @param passedTime 前进的时间。 (以秒为单位)
     * @version DragonBones 3.0
     */
    virtual void advanceTime(float passedTime) = 0;
    /**
     * 当前所属的 WordClock 实例。
     * @version DragonBones 5.0
     * @language zh_CN
     */
    virtual WorldClock* getClock() const = 0;
    virtual void setClock(WorldClock* value) = 0;
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_ANIMATEBLE_H
