#ifndef DRAGONBONES_ANIMATION_CONFIG_H
#define DRAGONBONES_ANIMATION_CONFIG_H

#include "../core/BaseObject.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* 动画配置，描述播放一个动画所需要的全部信息。
* @see dragonBones.AnimationState
* @version DragonBones 5.0
* @beta
* @language zh_CN
*/
class AnimationConfig : public BaseObject
{
    BIND_CLASS_TYPE_A(AnimationConfig);

public:
    /**
    * 是否暂停淡出的动画。
    * @default true
    * @version DragonBones 5.0
    * @language zh_CN
    */
    bool pauseFadeOut;
    /**
    * 淡出模式。
    * @default dragonBones.AnimationFadeOutMode.All
    * @see dragonBones.AnimationFadeOutMode
    * @version DragonBones 5.0
    * @language zh_CN
    */
    AnimationFadeOutMode fadeOutMode;
    /**
    * 淡出缓动方式。
    * @default TweenType.Line
    * @see dragonBones.TweenType
    * @version DragonBones 5.0
    * @language zh_CN
    */
    TweenType fadeOutTweenType;
    /**
    * 淡出时间。 [-1: 与淡入时间同步, [0~N]: 淡出时间] (以秒为单位)
    * @default -1
    * @version DragonBones 5.0
    * @language zh_CN
    */
    float fadeOutTime;
    /**
    * 否能触发行为。
    * @default true
    * @version DragonBones 5.0
    * @language zh_CN
    */
    bool actionEnabled;
    /**
    * 是否以增加的方式混合。
    * @default false
    * @version DragonBones 5.0
    * @language zh_CN
    */
    bool additiveBlending;
    /**
    * 是否对插槽的显示对象有控制权。
    * @default true
    * @version DragonBones 5.0
    * @language zh_CN
    */
    bool displayControl;
    /**
    * 是否暂停淡入的动画，直到淡入过程结束。
    * @default true
    * @version DragonBones 5.0
    * @language zh_CN
    */
    bool pauseFadeIn;
    /**
    * 是否将没有动画的对象重置为初始值。
    * @default true
    * @version DragonBones 5.1
    * @language zh_CN
    */
    bool resetToPose;
    /**
    * 淡入缓动方式。
    * @default TweenType.Line
    * @see dragonBones.TweenType
    * @version DragonBones 5.0
    * @language zh_CN
    */
    TweenType fadeInTweenType;
    /**
    * 播放次数。 [-1: 使用动画数据默认值, 0: 无限循环播放, [1~N]: 循环播放 N 次]
    * @default -1
    * @version DragonBones 5.0
    * @language zh_CN
    */
    int playTimes;
    /**
    * 混合图层，图层高会优先获取混合权重。
    * @default 0
    * @version DragonBones 5.0
    * @language zh_CN
    */
    int layer;
    /**
    * 开始时间。 (以秒为单位)
    * @default 0
    * @version DragonBones 5.0
    * @language zh_CN
    */
    float position;
    /**
    * 持续时间。 [-1: 使用动画数据默认值, 0: 动画停止, (0~N]: 持续时间] (以秒为单位)
    * @default -1
    * @version DragonBones 5.0
    * @language zh_CN
    */
    float duration;
    /**
    * 播放速度。 [(-N~0): 倒转播放, 0: 停止播放, (0~1): 慢速播放, 1: 正常播放, (1~N): 快速播放]
    * @default 1
    * @version DragonBones 3.0
    * @language zh_CN
    */
    float timeScale;
    /**
    * 混合权重。
    * @default 1
    * @version DragonBones 5.0
    * @language zh_CN
    */
    float weight;
    /**
    * 淡入时间。 [-1: 使用动画数据默认值, [0~N]: 淡入时间] (以秒为单位)
    * @default -1
    * @version DragonBones 5.0
    * @language zh_CN
    */
    float fadeInTime;
    /**
    * 自动淡出时间。 [-1: 不自动淡出, [0~N]: 淡出时间] (以秒为单位)
    * @default -1
    * @version DragonBones 5.0
    * @language zh_CN
    */
    float autoFadeOutTime;
    /**
    * 动画状态名。
    * @version DragonBones 5.0
    * @language zh_CN
    */
    std::string name;
    /**
    * 动画数据名。
    * @version DragonBones 5.0
    * @language zh_CN
    */
    std::string animation;
    /**
    * 混合组，用于动画状态编组，方便控制淡出。
    * @version DragonBones 5.0
    * @language zh_CN
    */
    std::string group;
    /**
    * 骨骼遮罩。
    * @version DragonBones 5.0
    * @language zh_CN
    */
    std::vector<std::string> boneMask;

protected:
    virtual void _onClear() override;

public:
    void clear();
    void copyFrom(AnimationConfig* value);
    bool containsBoneMask(const std::string& name) const;
    void addBoneMask(Armature* armature, const std::string& name, bool recursive);
    void removeBoneMask(Armature* armature, const std::string& name, bool recursive);

public: // For WebAssembly.
    int getFadeOutMode() const { return (int)fadeOutMode; }
    void setFadeOutMode(int value) { fadeOutMode = (AnimationFadeOutMode)value; }

    int getFadeOutTweenType() const { return (int)fadeOutTweenType; }
    void setFadeOutTweenType(int value) { fadeOutTweenType = (TweenType)value; }

    int getFadeInTweenType() const { return (int)fadeInTweenType; }
    void setFadeInTweenType(int value) { fadeInTweenType = (TweenType)value; }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_ANIMATION_CONFIG_H
