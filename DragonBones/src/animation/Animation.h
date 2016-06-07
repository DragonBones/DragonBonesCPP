#ifndef DRAGONBONES_ANIMATION_H
#define DRAGONBONES_ANIMATION_H

#include "../core/BaseObject.h"
#include "../model/AnimationData.h"

DRAGONBONES_NAMESPACE_BEGIN

enum class AnimationFadeOutMode { None, SameLayer, SameGroup, SameLayerAndGroup, All };

class Armature;
class AnimationState;

class Animation final : public BaseObject
{
    BIND_CLASS_TYPE(Animation);

private:
    static bool _sortAnimationState(AnimationState* a, AnimationState* b);

public:
    float timeScale;

public:
    bool _timelineStateDirty;
    Armature* _armature;
    
private:
    bool _isPlaying;
    AnimationState* _lastAnimationState;
    std::map<std::string, AnimationData*> _animations;
    std::vector<std::string> _animationNames;
    std::vector<AnimationState*> _animationStates;

public:
    Animation();
    ~Animation();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(Animation);

protected:
    void _onClear() override;
    void _fadeOut(float fadeOutTime, int layer, const std::string& group, AnimationFadeOutMode fadeOutMode, bool pauseFadeOut);

public:
    void _updateFFDTimelineStates();
    void _advanceTime(float passedTime);

public:
    void reset();
    void stop();
    AnimationState* play(const std::string& animationName = "", int playTimes = -1);
    AnimationState* fadeIn(
        const std::string& animationName, int playTimes = -1, float fadeInTime = -1.f,
        int layer = 0, const std::string& group = "", AnimationFadeOutMode fadeOutMode = AnimationFadeOutMode::SameLayerAndGroup,
        bool additiveBlending = false,
        bool pauseFadeOut = true, bool pauseFadeIn = true
    );
    AnimationState* gotoAndPlayWithTime(const std::string& animationName, float time = 0.f, int playTimes = -1);
    AnimationState* gotoAndPlayWithFrame(const std::string& animationName, unsigned frame = 0, int playTimes = -1);
    AnimationState* gotoAndPlayWithProgress(const std::string& animationName, float progress = 0.f, int playTimes = -1);
    AnimationState* gotoAndStopWithTime(const std::string& animationName, float time = 0.f);
    AnimationState* gotoAndStopWithFrame(const std::string& animationName, unsigned frame = 0);
    AnimationState* gotoAndStopWithProgress(const std::string& animationName, float progress = 0.f);
    bool hasAnimation(const std::string& animationName);
    AnimationState* getState(const std::string& animationName);
    bool getIsPlaying() const;
    bool getIsCompleted() const;
    const std::string& lastAnimationName() const;
    AnimationState* lastAnimationState() const;
    const std::map<std::string, AnimationData*>& getAnimations() const;
    void setAnimations(const std::map<std::string, AnimationData*>& value);


    /*virtual AnimationState* gotoAndPlay(
        const std::string &animationName,
        float fadeInTime = -1.f,
        float duration = -1.f,
        int playTimes = -1,
        int layer = 0,
        const std::string &group = "",
        AnimationFadeOutMode fadeOutMode = AnimationFadeOutMode::SAME_LAYER_AND_GROUP,
        bool pauseFadeOut = true,
        bool pauseFadeIn = true
    );
    
    virtual AnimationState* gotoAndStop(
        const std::string &animationName,
        float time,
        float normalizedTime = -1.f,
        float fadeInTime = 0.f,
        float duration = -1.f,
        int layer = 0,
        const std::string &group = "",
        AnimationFadeOutMode fadeOutMode = AnimationFadeOutMode::ALL
    );*/
};

DRAGONBONES_NAMESPACE_END
#endif  // DRAGONBONES_ANIMATION_H