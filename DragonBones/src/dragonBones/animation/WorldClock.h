#ifndef DRAGONBONES_WORLD_CLOCK_H
#define DRAGONBONES_WORLD_CLOCK_H

#include "../core/DragonBones.h"
#include "IAnimateble.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
 * WorldClock 提供时钟支持，为每个加入到时钟的 IAnimatable 对象更新时间。
 * @see dragonBones.IAnimateble
 * @see dragonBones.Armature
 * @version DragonBones 3.0
 * @language zh_CN
 */
class WorldClock : public IAnimateble
{
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(WorldClock)

public:
    /**
     * 一个可以直接使用的全局 WorldClock 实例.
     * @version DragonBones 3.0
     * @language zh_CN
     */
    static WorldClock clock;

public:
    /**
     * 当前时间。 (以秒为单位)
     * @version DragonBones 3.0
     * @language zh_CN
     */
    float time;
    /**
     * 时间流逝速度，用于控制动画变速播放。 [0: 停止播放, (0~1): 慢速播放, 1: 正常播放, (1~N): 快速播放]
     * @default 1.0
     * @version DragonBones 3.0
     * @language zh_CN
     */
    float timeScale;

private:
    std::vector<IAnimateble*> _animatebles;
    WorldClock* _clock;

public:
    /**
     * 创建一个新的 WorldClock 实例。
     * 通常并不需要单独创建 WorldClock 实例，可以直接使用 WorldClock.clock 静态实例。
     * (创建更多独立的 WorldClock 实例可以更灵活的为需要更新的 IAnimateble 实例分组，用于控制不同组不同的播放速度)
     * @version DragonBones 3.0
     * @language zh_CN
     */
    WorldClock(float ptime = -1.0f) :
        time(ptime),
        timeScale(1.0f),
        _animatebles(),
        _clock(nullptr)
    {
        if (time < 0.0f)
        {
            time = 0.0f;
        }
    }
    virtual ~WorldClock()
    {
        clear();
    }
    /**
     * 为所有的 IAnimatable 实例更新时间。
     * @param passedTime 前进的时间。 (以秒为单位，当设置为 -1 时将自动计算当前帧与上一帧的时间差)
     * @version DragonBones 3.0
     * @language zh_CN
     */
    virtual void advanceTime(float passedTime) override;
    /**
     * 是否包含 IAnimatable 实例
     * @param value IAnimatable 实例。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    bool contains(const IAnimateble* value) const;
    /**
     * 添加 IAnimatable 实例。
     * @param value IAnimatable 实例。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    void add(IAnimateble* value);
    /**
     * 移除 IAnimatable 实例。
     * @param value IAnimatable 实例。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    void remove(IAnimateble* value);
    /**
     * 清除所有的 IAnimatable 实例。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    void clear();
    /**
     * @inheritDoc
     */
    inline virtual WorldClock* getClock() const override
    {
        return _clock;
    }
    virtual void setClock(WorldClock* value) override;

public: // For WebAssembly.
    static WorldClock* getStaticClock() { return &WorldClock::clock; }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_WORLD_CLOCK_H
