#ifndef DRAGONBONES_BASE_TIMELINE_STATE_H
#define DRAGONBONES_BASE_TIMELINE_STATE_H

#include "../geom/Transform.h"
#include "../core/BaseObject.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* @private
*/
class TimelineState : public BaseObject
{
    ABSTRACT_CLASS(TimelineState)

protected:
    enum class TweenState
    {
        None = 0,
        Once = 1,
        Always = 2
    };

public:
    int playState; // -1: start, 0: play, 1: complete;
    int currentPlayTimes;
    float currentTime;

protected:
    TweenState _tweenState;
    unsigned _frameRate;
    unsigned _frameValueOffset;
    unsigned _frameCount;
    unsigned _frameOffset;
    int _frameIndex;
    float _frameRateR;
    float _position;
    float _duration;
    float _timeScale;
    float _timeOffset;
    DragonBonesData* _dragonBonesData;
    AnimationData* _animationData;
    TimelineData* _timelineData;
    Armature* _armature;
    AnimationState* _animationState;
    TimelineState* _actionTimeline;
    int16_t* _frameArray;
    int16_t* _frameIntArray;
    float* _frameFloatArray;
    uint16_t* _timelineArray;
    std::vector<unsigned>* _frameIndices;

protected:
    virtual void _onClear() override;
    virtual void _onArriveAtFrame() = 0;
    virtual void _onUpdateFrame() = 0;
    bool _setCurrentTime(float passedTime);

public:
    virtual void init(Armature* armature, AnimationState* animationState, TimelineData* timelineData);
    virtual void fadeOut() {}
    virtual void update(float passedTime);
};
/**
* @private
*/
class TweenTimelineState : public TimelineState
{
    ABSTRACT_CLASS(TweenTimelineState)

private:
    inline static float _getEasingValue(TweenType tweenType, float progress, float easing)
    {
        auto value = progress;
        switch (tweenType)
        {
            case TweenType::QuadIn:
                value = std::pow(progress, 2.0f);
                break;

            case TweenType::QuadOut:
                value = 1.0f - std::pow(1.0f - progress, 2.0f);
                break;

            case TweenType::QuadInOut:
                value = 0.5f * (1.0f - std::cos(progress * Transform::PI));
                break;
            default:
                break;
        }

        return (value - progress) * easing + progress;
    }

    inline static float _getEasingCurveValue(float progress, int16_t* samples, unsigned count, unsigned offset)
    {
        if (progress <= 0.0f) 
        {
            return 0.0f;
        }
        else if (progress >= 1.0f) 
        {
            return 1.0f;
        }

        const auto segmentCount = count + 1; // + 2 - 1
        const auto valueIndex = (unsigned)(progress * segmentCount);
        const auto fromValue = valueIndex == 0 ? 0.0f : samples[offset + valueIndex - 1];
        const auto toValue = (valueIndex == segmentCount - 1) ? 10000.0f : samples[offset + valueIndex];

        return (fromValue + (toValue - fromValue) * (progress * segmentCount - valueIndex)) * 0.0001f;
    }

protected:
    TweenType _tweenType;
    unsigned _curveCount;
    float _framePosition;
    float _frameDurationR;
    float _tweenProgress;
    float _tweenEasing;

protected:
    virtual void _onClear() override;
    virtual void _onArriveAtFrame() override;
    virtual void _onUpdateFrame() override;
};
/**
* @private
*/
class BoneTimelineState : public TweenTimelineState
{
    ABSTRACT_CLASS(BoneTimelineState)

public:
    Bone* bone;
    BonePose* bonePose;

protected:
    virtual void _onClear() override;
};
/**
* @private
*/
class SlotTimelineState : public TweenTimelineState
{
    ABSTRACT_CLASS(SlotTimelineState)

public:
    Slot* slot;

protected:
    virtual void _onClear() override;
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_BASE_TIMELINE_STATE_H
