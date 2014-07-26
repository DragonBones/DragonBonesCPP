#ifndef __ANIMATION_ANIMATION_H__
#define __ANIMATION_ANIMATION_H__

#include "../DragonBones.h"
#include "../objects/AnimationData.h"
#include "../animation/AnimationState.h"
#include "../core/Armature.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class AnimationState;
class Armature;

class Animation
{
    friend class Armature;
    friend class Slot;
    
public:
    static enum class AnimationFadeOutMode {NONE, SAME_LAYER, SAME_GROUP, SAME_LAYER_AND_GROUP, ALL};
    
public:
    bool autoTween;
    
    std::vector<AnimationData *> animationDataList;
    
protected:
    bool _isFading;
    bool _isPlaying;
    float _timeScale;
    
    std::vector<AnimationState *> _animationStateList;
    
    Armature *_armature;
    AnimationState *_lastAnimationState;
    
public:
    virtual bool getIsPlaying() const;
    virtual bool getIsComplete() const;
    //virtual const std::vector<String>& getAnimationList() const;
    virtual AnimationState *getLastAnimationState() const;
    virtual float getTimeScale() const;
    virtual void setTimeScale(float timeScale);
    
public:
    explicit Animation();
    virtual ~Animation();
    
    virtual void dispose();
    
    virtual AnimationState *gotoAndPlay(
        const String &animationName,
        float fadeInTime = -1.f,
        float duration = -1.f,
        int playTimes = -1,
        int layer = 0,
        const String &group = "",
        const AnimationFadeOutMode &fadeOutMode = AnimationFadeOutMode::SAME_LAYER_AND_GROUP,
        bool pauseFadeOut = true,
        bool pauseFadeIn = true
    );
    
    virtual AnimationState *gotoAndStop(
        const String &animationName,
        float time,
        float normalizedTime = -1.f,
        float fadeInTime = 0.f,
        float duration = -1.f,
        int layer = 0,
        const String &group = "",
        const AnimationFadeOutMode &fadeOutMode = AnimationFadeOutMode::ALL
    );
    
    virtual void play();
    virtual void stop();
    virtual void advanceTime(float passedTime);
    
    virtual bool hasAnimation(const String &animationName) const;
    virtual AnimationState *getState(const String &name, int layer = 0) const;
    
protected:
    virtual void addState(AnimationState *animationState);
    virtual void removeState(AnimationState *animationState);
    virtual void updateAnimationStates();
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(Animation);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __ANIMATION_ANIMATION_H__
