#ifndef DRAGONBONES_ANIMATION_STATE_H
#define DRAGONBONES_ANIMATION_STATE_H

#include "../core/BaseObject.h"
#include "../geom/Transform.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* @private
*/
class BonePose : public BaseObject
{
    BIND_CLASS_TYPE_A(BonePose);

public:
    Transform current;
    Transform delta;
    Transform result;

protected:
    virtual void _onClear() override;
};
/**
 * 动画状态，播放动画时产生，可以对每个播放的动画进行更细致的控制和调节。
 * @see dragonBones.Animation
 * @see dragonBones.AnimationData
 * @version DragonBones 3.0
 * @language zh_CN
 */
class AnimationState : public BaseObject
{
    BIND_CLASS_TYPE_B(AnimationState);

public:
    /**
     * 是否将骨架的骨骼和插槽重置为绑定姿势（如果骨骼和插槽在这个动画状态中没有动画）。
     * @version DragonBones 5.1
     * @language zh_CN
     */
    bool resetToPose;
    /**
     * 是否以增加的方式混合。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    bool displayControl;
    /**
     * 是否对插槽的显示对象有控制权。
     * @see dragonBones.Slot#displayController
     * @version DragonBones 3.0
     * @language zh_CN
     */
    bool additiveBlending;
    /**
     * 是否能触发行为。
     * @version DragonBones 5.0
     * @language zh_CN
     */
    bool actionEnabled;
    /**
     * 混合图层。
     * @version DragonBones 3.0
     * @readonly
     * @language zh_CN
     */
    unsigned layer;
    /**
     * 播放次数。 [0: 无限循环播放, [1~N]: 循环播放 N 次]
     * @version DragonBones 3.0
     * @language zh_CN
     */
    unsigned playTimes;
    /**
     * 播放速度。 [(-N~0): 倒转播放, 0: 停止播放, (0~1): 慢速播放, 1: 正常播放, (1~N): 快速播放]
     * @version DragonBones 3.0
     * @language zh_CN
     */
    float timeScale;
    /**
     * 混合权重。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    float weight;
    /**
     * 自动淡出时间。 [-1: 不自动淡出, [0~N]: 淡出时间] (以秒为单位)
     * 当设置一个大于等于 0 的值，动画状态将会在播放完成后自动淡出。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    float autoFadeOutTime;
    /**
     * @private
     */
    float fadeTotalTime;
    /**
     * 动画名称。
     * @version DragonBones 3.0
     * @readonly
     * @language zh_CN
     */
    std::string name;
    /**
     * 混合组。
     * @version DragonBones 3.0
     * @readonly
     * @language zh_CN
     */
    std::string group;
    /**
     * 动画数据。
     * @see dragonBones.AnimationData
     * @version DragonBones 3.0
     * @readonly
     * @language zh_CN
     */
    AnimationData* animationData;

public:
    int _playheadState;
    int _fadeState;
    int _subFadeState;
    float _position;
    float _duration;
    float _fadeProgress;
    ActionTimelineState* _actionTimeline;

private:
    bool _timelineDirty;
    float _fadeTime;
    float _time;
    float _weightResult;
    std::vector<std::string> _boneMask;
    std::vector<BoneTimelineState*> _boneTimelines;
    std::vector<SlotTimelineState*> _slotTimelines;
    std::map<std::string, BonePose*> _bonePoses;
    Armature* _armature;
    ZOrderTimelineState* _zOrderTimeline;

public:
    AnimationState() :
        _actionTimeline(nullptr),
        _zOrderTimeline(nullptr)
    {
        _onClear();
    }
    virtual ~AnimationState()
    {
        _onClear();
    }

protected:
    virtual void _onClear() override;

private:
    bool _isDisabled(const Slot& slot) const;
    void _advanceFadeTime(float passedTime);
    void _blendBoneTimline(BoneTimelineState* timeline) const;

public:
    /**
     * @private
     */
    void init(Armature* armature, AnimationData* animationData, AnimationConfig* animationConfig);
    /**
     * @private
     */
    void updateTimelines();
    /**
     * @private
     */
    void advanceTime(float passedTime, float cacheFrameRate);
    /**
     * 继续播放。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    void play();
    /**
     * 暂停播放。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    void stop();
    /**
     * 淡出动画。
     * @param fadeOutTime 淡出时间。 (以秒为单位)
     * @param pausePlayhead 淡出时是否暂停动画。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    void fadeOut(float fadeOutTime, bool pausePlayhead = true);
    /**
     * 是否包含骨骼遮罩。
     * @param name 指定的骨骼名称。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    bool containsBoneMask(const std::string& name) const;
    /**
     * 添加骨骼遮罩。
     * @param name 指定的骨骼名称。
     * @param recursive 是否为该骨骼的子骨骼添加遮罩。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    void addBoneMask(const std::string& name, bool recursive = true);
    /**
     * 删除骨骼遮罩。
     * @param name 指定的骨骼名称。
     * @param recursive 是否删除该骨骼的子骨骼遮罩。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    void removeBoneMask(const std::string& name, bool recursive = true);
    /**
     * 删除所有骨骼遮罩。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    void removeAllBoneMask();
    /**
     * 是否正在淡入。
     * @version DragonBones 5.1
     * @language zh_CN
     */
    inline bool isFadeIn() const 
    {
        return _fadeState < 0;
    }
    /**
     * 是否正在淡出。
     * @version DragonBones 5.1
     * @language zh_CN
     */
    inline bool isFadeOut() const
    {
        return _fadeState > 0;
    }
    /**
     * 是否淡入完毕。
     * @version DragonBones 5.1
     * @language zh_CN
     */
    inline bool isFadeComplete() const
    {
        return _fadeState == 0;
    }
    /**
     * 是否正在播放。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    bool isPlaying() const;
    /**
     * 是否播放完毕。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    bool isCompleted() const;
    /**
     * 当前播放次数。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    unsigned getCurrentPlayTimes() const;
    /**
     * 总时间。 (以秒为单位)
     * @version DragonBones 3.0
     * @language zh_CN
     */
    inline float getTotalTime() const
    {
        return _duration;
    }
    /**
     * 当前播放的时间。 (以秒为单位)
     * @version DragonBones 3.0
     * @language zh_CN
     */
    float getCurrentTime() const;
    void setCurrentTime(float value);
    /**
    * 动画名称。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline const std::string& getName() const
    {
        return name;
    }
};

DRAGONBONES_NAMESPACE_END
#endif  // DRAGONBONES_ANIMATION_STATE_H
