#ifndef DRAGONBONES_ANIMATION_H
#define DRAGONBONES_ANIMATION_H

#include "../core/BaseObject.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
 * 动画控制器，用来播放动画数据，管理动画状态。
 * @see dragonBones.AnimationData
 * @see dragonBones.AnimationState
 * @version DragonBones 3.0
 * @language zh_CN
 */
class Animation final : public BaseObject
{
    BIND_CLASS_TYPE_B(Animation);

public:
    /**
     * 播放速度。 [0: 停止播放, (0~1): 慢速播放, 1: 正常播放, (1~N): 快速播放]
     * @default 1.0
     * @version DragonBones 3.0
     * @language zh_CN
     */
    float timeScale;

public:
    /**
     * @private
     */
    bool _timelineDirty;
    
private:
    bool _animationDirty;
    float _inheritTimeScale;
    std::vector<std::string> _animationNames;
    std::vector<AnimationState*> _animationStates;
    std::map<std::string, AnimationData*> _animations;
    Armature* _armature;
    AnimationConfig* _animationConfig;
    AnimationState* _lastAnimationState;

public:
    Animation() :
        _animationConfig(nullptr)
    { 
        _onClear();
    }
    ~Animation()
    {
        _onClear();
    }

private:
    void _fadeOut(AnimationConfig* animationConfig);

protected:
    virtual void _onClear() override;

public:
    /**
     * @private
     */
    void init(Armature* armature);
    /**
     * @private
     */
    void advanceTime(float passedTime);
    /**
     * 清除所有动画状态。
     * @see dragonBones.AnimationState
     * @version DragonBones 4.5
     * @language zh_CN
     */
    void reset();
    /**
     * 暂停播放动画。
     * @param animationName 动画状态的名称，如果未设置，则暂停所有动画状态。
     * @see dragonBones.AnimationState
     * @version DragonBones 3.0
     * @language zh_CN
     */
    void stop(const std::string& animationName);
    /**
     * 通过动画配置来播放动画。
     * @param animationConfig 动画配置。
     * @returns 对应的动画状态。
     * @see dragonBones.AnimationConfig
     * @see dragonBones.AnimationState
     * @version DragonBones 5.0
     * @beta
     * @language zh_CN
     */
    AnimationState* playConfig(AnimationConfig* animationConfig);
    /**
     * 播放动画。
     * @param animationName 动画数据名称，如果未设置，则播放默认动画，或将暂停状态切换为播放状态，或重新播放上一个正在播放的动画。
     * @param playTimes 播放次数。 [-1: 使用动画数据默认值, 0: 无限循环播放, [1~N]: 循环播放 N 次]
     * @returns 对应的动画状态。
     * @see dragonBones.AnimationState
     * @version DragonBones 3.0
     * @language zh_CN
     */
    AnimationState* play(const std::string& animationName = "", int playTimes = -1);
    /**
     * 淡入播放动画。
     * @param animationName 动画数据名称。
     * @param playTimes 播放次数。 [-1: 使用动画数据默认值, 0: 无限循环播放, [1~N]: 循环播放 N 次]
     * @param fadeInTime 淡入时间。 [-1: 使用动画数据默认值, [0~N]: 淡入时间] (以秒为单位)
     * @param layer 混合图层，图层高会优先获取混合权重。
     * @param group 混合组，用于动画状态编组，方便控制淡出。
     * @param fadeOutMode 淡出模式。
     * @param resetToPose
     * @returns 对应的动画状态。
     * @see dragonBones.AnimationFadeOutMode
     * @see dragonBones.AnimationState
     * @version DragonBones 4.5
     * @language zh_CN
     */
#ifdef EGRET_WASM
    AnimationState* fadeIn(
        const std::string& animationName, float fadeInTime = -1.f, int playTimes = -1,
        int layer = 0, const std::string& group = "", int fadeOutMode = 3/*AnimationFadeOutMode fadeOutMode = AnimationFadeOutMode::SameLayerAndGroup*/
    );
#else
    AnimationState* fadeIn(
        const std::string& animationName, float fadeInTime = -1.f, int playTimes = -1,
        int layer = 0, const std::string& group = "", AnimationFadeOutMode fadeOutMode = AnimationFadeOutMode::SameLayerAndGroup
    );
#endif // EGRET_WASM
    /**
     * 从指定时间开始播放动画。
     * @param animationName 动画数据的名称。
     * @param time 开始时间。 (以秒为单位)
     * @param playTimes 播放次数。 [-1: 使用动画数据默认值, 0: 无限循环播放, [1~N]: 循环播放 N 次]
     * @returns 对应的动画状态。
     * @see dragonBones.AnimationState
     * @version DragonBones 4.5
     * @language zh_CN
     */
    AnimationState* gotoAndPlayByTime(const std::string& animationName, float time = 0.f, int playTimes = -1);
    /**
     * 从指定帧开始播放动画。
     * @param animationName 动画数据的名称。
     * @param frame 帧。
     * @param playTimes 播放次数。 [-1: 使用动画数据默认值, 0: 无限循环播放, [1~N]: 循环播放 N 次]
     * @returns 对应的动画状态。
     * @see dragonBones.AnimationState
     * @version DragonBones 4.5
     * @language zh_CN
     */
    AnimationState* gotoAndPlayByFrame(const std::string& animationName, unsigned frame = 0, int playTimes = -1);
    /**
     * 从指定进度开始播放动画。
     * @param animationName 动画数据的名称。
     * @param progress 进度。 [0~1]
     * @param playTimes 播放次数。 [-1: 使用动画数据默认值, 0: 无限循环播放, [1~N]: 循环播放 N 次]
     * @returns 对应的动画状态。
     * @see dragonBones.AnimationState
     * @version DragonBones 4.5
     * @language zh_CN
     */
    AnimationState* gotoAndPlayByProgress(const std::string& animationName, float progress = 0.f, int playTimes = -1);
    /**
     * 将动画停止到指定的时间。
     * @param animationName 动画数据的名称。
     * @param time 时间。 (以秒为单位)
     * @returns 对应的动画状态。
     * @see dragonBones.AnimationState
     * @version DragonBones 4.5
     * @language zh_CN
     */
    AnimationState* gotoAndStopByTime(const std::string& animationName, float time = 0.f);
    /**
     * 将动画停止到指定的帧。
     * @param animationName 动画数据的名称。
     * @param frame 帧。
     * @returns 对应的动画状态。
     * @see dragonBones.AnimationState
     * @version DragonBones 4.5
     * @language zh_CN
     */
    AnimationState* gotoAndStopByFrame(const std::string& animationName, unsigned frame = 0);
    /**
     * 将动画停止到指定的进度。
     * @param animationName 动画数据的名称。
     * @param progress 进度。 [0 ~ 1]
     * @returns 对应的动画状态。
     * @see dragonBones.AnimationState
     * @version DragonBones 4.5
     * @language zh_CN
     */
    AnimationState* gotoAndStopByProgress(const std::string& animationName, float progress = 0.f);
    /**
     * 获取动画状态。
     * @param animationName 动画状态的名称。
     * @see dragonBones.AnimationState
     * @version DragonBones 3.0
     * @language zh_CN
     */
    AnimationState* getState(const std::string& animationName) const;
    /**
     * 是否包含动画数据。
     * @param animationName 动画数据的名称。
     * @see dragonBones.AnimationData
     * @version DragonBones 3.0
     * @language zh_CN
     */
    bool hasAnimation(const std::string& animationName) const;
    /**
    * 获取所有的动画状态。
    * @see dragonBones.AnimationState
    * @version DragonBones 5.1
    * @language zh_CN
    */
    inline const std::vector<AnimationState*>& getStates() const
    {
        return _animationStates;
    }
    /**
     * 动画是否处于播放状态。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    bool isPlaying() const;
    /**
     * 所有动画状态是否均已播放完毕。
     * @see dragonBones.AnimationState
     * @version DragonBones 3.0
     * @language zh_CN
     */
    bool isCompleted() const;
    /**
     * 上一个正在播放的动画状态名称。
     * @see #lastAnimationState
     * @version DragonBones 3.0
     * @language zh_CN
     */
    const std::string& getLastAnimationName() const;
    /**
     * 所有动画数据名称。
     * @see #animations
     * @version DragonBones 4.5
     * @language zh_CN
     */
    inline const std::vector<std::string>& getAnimationNames() const
    {
        return _animationNames;
    }
    /**
     * 所有动画数据。
     * @see dragonBones.AnimationData
     * @version DragonBones 4.5
     * @language zh_CN
     */
    inline const std::map<std::string, AnimationData*>& getAnimations() const
    {
        return _animations;
    }
    void setAnimations(const std::map<std::string, AnimationData*>& value);
    /**
     * 一个可以快速使用的动画配置实例。
     * @see dragonBones.AnimationConfig
     * @version DragonBones 5.0
     * @language zh_CN
     */
    AnimationConfig* getAnimationConfig() const;
    /**
    * 上一个正在播放的动画状态名称。
    * @see #lastAnimationState
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline AnimationState* getLastAnimationState() const
    {
        return _lastAnimationState;
    }
};

DRAGONBONES_NAMESPACE_END
#endif  // DRAGONBONES_ANIMATION_H
