#ifndef __ANIMATION_ANIMATION_STATE_H__
#define __ANIMATION_ANIMATION_STATE_H__

#include "../DragonBones.h"
#include "../objects/AnimationData.h"
#include "../core/Armature.h"
#include "TimelineState.h"

NAME_SPACE_DRAGON_BONES_BEGIN

class TimelineState;

class AnimationState
{
    friend class Animation;
    friend class TimelineState;
    
private:
    static std::vector<AnimationState*> _pool;
    static AnimationState* borrowObject();
    static void returnObject(AnimationState *animationState);
    static void clearObjects();
    
    static enum class FadeState {FADE_BEFORE, FADING, FADE_COMPLETE};
    
public:
    bool additiveBlending;
    bool autoTween;
    bool autoFadeOut;
    bool displayControl;
    bool lastFrameAutoTween;
    float fadeOutTime;
    float weight;
    String name;
    
private:
    bool _isPlaying;
    bool _isComplete;
    bool _isFadeOut;
    bool _pausePlayheadInFade;
    int _currentPlayTimes;
    int _layer;
    int _playTimes;
    int _currentTime;
    int _currentFrameIndex;
    int _currentFramePosition;
    int _currentFrameDuration;
    int _totalTime;
    float _time;
    float _timeScale;
    float _fadeWeight;
    float _fadeTotalWeight;
    float _fadeCurrentTime;
    float _fadeTotalTime;
    float _fadeBeginTime;
    String _group;
    FadeState _fadeState;
    
    std::vector<TimelineState*> _timelineStateList;
    std::vector<String> _mixingTransforms;
    
    AnimationData *_clip;
    Armature *_armature;
    
public:
    bool getIsComplete() const;
    bool getIsPlaying() const;
    int getCurrentPlayTimes() const;
    int getLayer() const;
    float getTotalTime() const;
    float getCurrentWeight() const;
    const String& getGroup() const;
    const AnimationData* getClip() const;
    
    AnimationState* setAdditiveBlending(bool value);
    AnimationState* setAutoFadeOut(bool value, float fadeOutTime = -1);
    AnimationState* setWeight(float value);
    AnimationState* setFrameTween(bool autoTween_, bool lastFrameAutoTween_);
    
    int getPlayTimes() const;
    AnimationState* setPlayTimes(int playTimes);
    float getCurrentTime() const;
    AnimationState* setCurrentTime(float currentTime);
    float getTimeScale() const;
    AnimationState* setTimeScale(float timeScale);
    
public:
    AnimationState();
    virtual ~AnimationState();
    
    AnimationState* fadeOut(float fadeTotalTime, bool pausePlayhead);
    AnimationState* play();
    AnimationState* stop();
    bool getMixingTransform(const String &timelineName) const;
    AnimationState* addMixingTransform(const String &timelineName, bool recursive = true);
    AnimationState* removeMixingTransform(const String &timelineName, bool recursive = true);
    AnimationState* removeAllMixingTransform();
    
private:
    void fadeIn(Armature *armature, AnimationData *clip, float fadeTotalTime, float timeScale, int playTimes, bool pausePlayhead);
    
    bool advanceTime(float passedTime);
    void updateTimelineStates();
    void addTimelineState(const String &timelineName);
    void removeTimelineState(TimelineState *timelineState);
    void advanceFadeTime(float passedTime);
    void advanceTimelinesTime(float passedTime);
    void updateMainTimeline(bool isThisComplete);
    void hideBones();
    void clear();
    
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(AnimationState);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __ANIMATION_ANIMATION_STATE_H__
