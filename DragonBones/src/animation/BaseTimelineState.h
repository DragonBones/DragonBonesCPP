#ifndef DRAGONBONES_BASE_TIMELINE_STATE_H
#define DRAGONBONES_BASE_TIMELINE_STATE_H

#include "../core/BaseObject.h"
#include "../model/FrameData.h"
#include "../model/TimelineData.h"
#include "../model/AnimationData.h"

NAMESPACE_DRAGONBONES_BEGIN

class Armature;
class AnimationState;

template<class T, class M>
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
    T* _currentFrame;
    M* _timeline;
    Armature* _armature;
    AnimationState* _animationState;

public:
    TimelineState() {}
    virtual ~TimelineState() {}

protected:
    virtual void _onClear() override
    {
        _isCompleted = false;
        _currentTime = 0;

        _isReverse = false;
        _hasAsynchronyTimeline = false;
        _keyFrameCount = 0;
        _frameCount = 0;
        _currentPlayTimes = 0;
        _position = 0;
        _duration = 0;
        _clipDutation = 0;
        _timeScale = 1.f;
        _timeOffset = 0.f;
        _timeToFrameSccale = 0.f;
        _currentFrame = nullptr;
        _timeline = nullptr;
        _armature = nullptr;
        _animationState = nullptr;
    }

    virtual void _onFadeIn() {}
    virtual void _onUpdateFrame(bool isUpdate) {}
    virtual void _onArriveAtFrame(bool isUpdate) {}
    virtual void _onCrossFrame(T* frame) {}
    bool _setCurrentTime(int value)
    {
        if (_hasAsynchronyTimeline)
        {
            const auto playTimes = _animationState->playTimes;
            const auto totalTimes = playTimes * _duration;

            value *= _timeScale;
            if (_timeOffset != 0.f)
            {
                value += _timeOffset * _clipDutation;
            }

            if (playTimes > 0 && (value >= totalTimes || value <= -totalTimes))
            {
                _isCompleted = true;
                _currentPlayTimes = playTimes;

                if (value < 0)
                {
                    value = 0;
                }
                else
                {
                    value = _duration;
                }
            }
            else
            {
                _isCompleted = false;

                if (value < 0)
                {
                    _currentPlayTimes = -value / _duration;
                    value = _duration - (value % _duration);
                }
                else
                {
                    _currentPlayTimes = value / _duration;
                    value %= _duration;
                }

                if (_currentPlayTimes > playTimes)
                {
                    _currentPlayTimes = playTimes;
                }
            }

            value += _position;
        }
        else
        {
            // _isCompleted = _animationState._timeline._isCompleted;
            // _currentPlayTimes = _animationState._timeline._currentPlayTimes;
        }

        if (_currentTime == value)
        {
            return false;
        }

        if (_keyFrameCount == 1 && value > _position && (void*)this != (void*)_animationState->_timeline)
        {
            _isCompleted = true;
        }

        _isReverse = _currentTime > value;
        _currentTime = value;

        return true;
    }

public:
    void setCurrentTime(std::size_t value)
    {
        _setCurrentTime(value);

        switch (_keyFrameCount)
        {
        case 0:
            break;

        case 1:
            _currentFrame = _timeline->frames[0];
            _onArriveAtFrame(false);
            _onUpdateFrame(false);
            break;

        default:
            _currentFrame = _timeline->frames[unsigned(_currentTime * _timeToFrameSccale)]; // floor
            _onArriveAtFrame(false);
            _onUpdateFrame(false);
            break;
        }

        // _currentFrame = null; // TODO For first event frame
    }

    void fadeIn(Armature* armature, AnimationState* animationState, M* timelineData)
    {
        _armature = armature;
        _animationState = animationState;
        _timeline = timelineData;

        const auto isMainTimeline = (void*)this == (void*)_animationState->_timeline;

        _hasAsynchronyTimeline = isMainTimeline || _animationState->getClip()->hasAsynchronyTimeline;
        _keyFrameCount = _timeline->frames.size();
        _frameCount = _animationState->getClip()->frameCount;
        _position = _animationState->_position;
        _duration = _animationState->_duration;
        _clipDutation = _animationState->_clipDutation;
        _timeScale = isMainTimeline ? 1.f : (1.f / _timeline->scale);
        _timeOffset = isMainTimeline ? 0.f : _timeline->offset;
        _timeToFrameSccale = (float)_frameCount / (_clipDutation + 1);

        _onFadeIn();

        setCurrentTime(0);
    }

    virtual void fadeOut() {}

    virtual void update(int time)
    {
        if (!_isCompleted && _setCurrentTime(time) && _keyFrameCount)
        {
            const unsigned currentFrameIndex = _keyFrameCount > 1 ? unsigned(_currentTime * _timeToFrameSccale) : 0;
            const auto currentFrame = _timeline->frames[currentFrameIndex];
            if (_currentFrame != currentFrame)
            {
                if (_keyFrameCount > 1)
                {
                    auto crossedFrame = _currentFrame ? _currentFrame : currentFrame->prev;
                    _currentFrame = currentFrame;

                    if (_isReverse)
                    {
                        while (crossedFrame != currentFrame)
                        {
                            _onCrossFrame(crossedFrame);
                            crossedFrame = crossedFrame->prev;
                        }
                    }
                    else
                    {
                        while (crossedFrame != currentFrame)
                        {
                            crossedFrame = crossedFrame->next;
                            _onCrossFrame(crossedFrame);
                        }
                    }

                    _onArriveAtFrame(true);
                }
                else
                {
                    _currentFrame = currentFrame;
                    _onCrossFrame(_currentFrame);
                    _onArriveAtFrame(true);
                }
            }

            _onUpdateFrame(true);
        }
    }
};

template<class T, class M>
class TweenTimelineState : public TimelineState<T, M>
{
protected:
    static float _getEasingValue(float progress, float easing)
    {
        auto value = 1.f;
        if (easing > 2.f)
        {
            return progress;
        }
        else if (easing > 1.f) // Ease in out
        {
            value = 0.5f * (1.f - std::cos(progress * PI));
            easing -= 1.f;
        }
        else if (easing > 0.f) // Ease out
        {
            value = 1.f - std::pow(1.f - progress, 2);
        }
        else if (easing >= -1.f) // Ease in
        {
            easing *= -1.f;
            value = std::pow(progress, 2);
        }
        else if (easing >= -2.f) // Ease out in
        {
            easing *= -1.f;
            value = std::acos(1.f - progress * 2.f) / PI;
            easing -= 1.f;
        }
        else
        {
            return progress;
        }

        return (value - progress) * easing + progress;
    }

    static float _getCurveEasingValue(float progress, const std::vector<float>& sampling)
    {
        auto x = 0.f;
        auto y = 0.f;

        for (std::size_t i = 0, l = sampling.size(); i < l; i += 2)
        {
            x = sampling[i];
            y = sampling[i + 1];
            if (x >= progress)
            {
                if (i == 0)
                {
                    return y * progress / x;
                }
                else
                {
                    const auto xP = sampling[i - 2];
                    const auto yP = sampling[i - 1]; // i - 2 + 1
                    return yP + (y - yP) * (progress - xP) / (x - xP);
                }
            }
        }

        return y + (1.f - y) * (progress - x) / (1.f - x);
    }

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
    TweenTimelineState() {}
    virtual ~TweenTimelineState() {}

protected:
    virtual void _onClear() override
    {
        TimelineState::_onClear();

        _tweenProgress = 0.f;
        _tweenEasing = NO_TWEEN;
        _curve = nullptr;
    }

    virtual void _onArriveAtFrame(bool isUpdate) override
    {
        _tweenEasing = this->_currentFrame->tweenEasing;
        _curve = this->_currentFrame->curve.empty() ? nullptr : &this->_currentFrame->curve;

        if (
            this->_keyFrameCount == 1 ||
            (
                this->_currentFrame->next == this->_timeline->frames[0] &&
                (_tweenEasing != NO_TWEEN || _curve) &&
                this->_animationState->playTimes > 0 &&
                this->_animationState->getCurrentPlayTimes() == this->_animationState->playTimes - 1
                )
            )
        {
            _tweenEasing = NO_TWEEN;
            _curve = nullptr;
        }
    }

    virtual void _onUpdateFrame(bool isUpdate) override
    {
        if (!this->_currentFrame->duration)
        {
            _tweenProgress = 0.f;
        }
        else if (_tweenEasing != NO_TWEEN)
        {
            _tweenProgress = (float)(this->_currentTime - this->_currentFrame->position + this->_position) / this->_currentFrame->duration;
            if (_tweenEasing != 0.f)
            {
                _tweenProgress = _getEasingValue(_tweenProgress, _tweenEasing);
            }
        }
        else if (_curve)
        {
            _tweenProgress = (float)(this->_currentTime - this->_currentFrame->position + this->_position) / this->_currentFrame->duration;
            _tweenProgress = _getCurveEasingValue(_tweenProgress, *_curve);
        }
        else
        {
            _tweenProgress = 0.f;
        }
    }

    TweenType _updateExtensionKeyFrame(const ExtensionFrameData& current, const ExtensionFrameData& next, ExtensionFrameData& result)
    {
        auto tweenType = TweenType::None;
        if (current.type == next.type)
        {
            for (std::size_t i = 0, l = current.tweens.size(); i < l; ++i)
            {
                const auto tweenDuration = next.tweens[i] - current.tweens[i];
                result.tweens[i] = tweenDuration;

                if (tweenDuration != 0.f)
                {
                    tweenType = TweenType::Always;
                }
            }
        }

        if (tweenType == TweenType::None)
        {
            if (result.type != current.type)
            {
                tweenType = TweenType::Once;
                result.type = current.type;
            }

            if (result.tweens.size() != current.tweens.size())
            {
                tweenType = TweenType::Once;
                result.tweens.resize(current.tweens.size(), 0.f);
            }

            if (result.keys.size() != current.keys.size())
            {
                tweenType = TweenType::Once;
                result.keys.resize(current.keys.size(), 0.f);
            }

            for (std::size_t i = 0, l = current.keys.size(); i < l; ++i)
            {
                const auto key = current.keys[i];
                if (result.keys[i] != key)
                {
                    tweenType = TweenType::Once;
                    result.keys[i] = key;
                }
            }
        }

        return tweenType;
    }
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_BASE_TIMELINE_STATE_H