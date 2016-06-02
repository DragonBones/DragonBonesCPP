#ifndef DRAGONBONES_BASE_TIMELINE_STATE_H
#define DRAGONBONES_BASE_TIMELINE_STATE_H

#include "../core/BaseObject.h"
#include "../model/FrameData.h"
#include "../model/TimelineData.h"

NAMESPACE_DRAGONBONES_BEGIN

class Armature;
class AnimationState;

class TimelineState : public BaseObject
{
public:
    bool _isCompleted;
    std::size_t _currentTime;

protected:
    bool _isReverse;
    bool _hasAsynchronyTimeline;
    unsigned _keyFrameCount;
    unsigned _frameCount;
    unsigned _currentPlayTimes;
    std::size_t _position;
    std::size_t _duration;
    std::size_t _clipDutation;
    float _timeScale;
    float _timeOffset;
    float _timeToFrameSccale;
    FrameData* _currentFrame;
    TimelineData* _timeline;
    Armature* _armature;
    AnimationState* _animationState;

public:
    TimelineState();
    virtual ~TimelineState() = 0;

protected:
    virtual void _onClear() override;

    virtual void _onFadeIn() {}
    virtual void _onUpdateFrame(bool isUpdate) {}
    virtual void _onArriveAtFrame(bool isUpdate) {}
    virtual void _onCrossFrame(FrameData* frame) {}
    bool _setCurrentTime(int value);

public:
    void setCurrentTime(std::size_t value);
    void fadeIn(Armature* armature, AnimationState* animationState, TimelineData* timelineData);
    virtual void fadeOut() {}
    virtual void update(int time);
};

class TweenTimelineState : public TimelineState
{
protected:
    static float _getEasingValue(float progress, float easing);
    static float _getCurveEasingValue(float progress, const std::vector<float>& sampling);

protected:
    enum class TweenType
    {
        None = 0,
        Once = 1,
        Always = 2
    };

    float _tweenProgress;
    float _tweenEasing;
    std::vector<float>* _curve;

public:
    TweenTimelineState();
    virtual ~TweenTimelineState() = 0;

protected:
    virtual void _onClear() override;
    virtual void _onArriveAtFrame(bool isUpdate) override;
    virtual void _onUpdateFrame(bool isUpdate) override;
    TweenType _updateExtensionKeyFrame(const ExtensionFrameData& current, const ExtensionFrameData& next, ExtensionFrameData& result);
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_BASE_TIMELINE_STATE_H