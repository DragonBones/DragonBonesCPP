#ifndef ANIMATION_ANIMATION_STATE_H
#define ANIMATION_ANIMATION_STATE_H

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
    enum class FadeState {FADE_BEFORE, FADING, FADE_COMPLETE};

    static std::vector<AnimationState*> _pool;
    static AnimationState* borrowObject();
    static void returnObject(AnimationState *animationState);
    static void clearObjects();
    
public:
    bool additiveBlending;
    bool autoTween;
    bool autoFadeOut;
    bool displayControl;
    bool lastFrameAutoTween;
    float fadeOutTime;
    float weight;
    std::string name;
    
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
    std::string _group;
    FadeState _fadeState;
    
    std::vector<TimelineState*> _timelineStateList;
    std::vector<std::string> _mixingTransforms;
    
    AnimationData *_clip;
    Armature *_armature;
    
public:
    bool getIsComplete() const;
    bool getIsPlaying() const;
    int getCurrentPlayTimes() const;
    int getLayer() const;
    float getTotalTime() const;
    float getCurrentWeight() const;
    const std::string& getGroup() const;
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
    bool getMixingTransform(const std::string &timelineName) const;
    AnimationState* addMixingTransform(const std::string &timelineName, bool recursive = true);
    AnimationState* removeMixingTransform(const std::string &timelineName, bool recursive = true);
    AnimationState* removeAllMixingTransform();
    
private:
    void fadeIn(Armature *armature, AnimationData *clip, float fadeTotalTime, float timeScale, int playTimes, bool pausePlayhead);
    
    bool advanceTime(float passedTime);
    void updateTimelineStates();
    void addTimelineState(const std::string &timelineName);
    void removeTimelineState(TimelineState *timelineState);
    void advanceFadeTime(float passedTime);
    void advanceTimelinesTime(float passedTime);
    void updateMainTimeline(bool isThisComplete);
    void hideBones();
    void clear();
    
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(AnimationState);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // ANIMATION_ANIMATION_STATE_H
