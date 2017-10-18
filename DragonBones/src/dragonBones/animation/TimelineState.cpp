#include "TimelineState.h"
#include "WorldClock.h"
#include "Animation.h"
#include "../model/DragonBonesData.h"
#include "../model/UserData.h"
#include "../model/ArmatureData.h"
#include "../model/DisplayData.h"
#include "../model/AnimationData.h"
#include "../events/EventObject.h"
#include "../events/IEventDispatcher.h"
#include "../armature/Armature.h"
#include "../armature/Bone.h"
#include "../armature/Slot.h"
#include "AnimationState.h"

DRAGONBONES_NAMESPACE_BEGIN

void ActionTimelineState::_onCrossFrame(unsigned frameIndex) const
{
    const auto eventDispatcher = _armature->getProxy();

    if (_animationState->actionEnabled)
    {
        const auto frameOffset = _animationData->frameOffset + _timelineArray[_timelineData->offset + (unsigned)BinaryOffset::TimelineFrameOffset + frameIndex];
        const unsigned actionCount = _frameArray[frameOffset + 1];
        const auto& actions = _animationData->parent->actions; // May be the animaton data not belong to this armature data.
        for (std::size_t i = 0; i < actionCount; ++i)
        {
            const auto actionIndex = _frameArray[frameOffset + 2 + i];
            const auto action = actions[actionIndex];
            if (action->type == ActionType::Play)
            {
                if (action->slot != nullptr) 
                {
                    const auto slot = _armature->getSlot(action->slot->name);
                    if (slot != nullptr)
                    {
                        const auto childArmature = slot->getChildArmature();
                        if (childArmature != nullptr) 
                        {
                            childArmature->_bufferAction(action, true);
                        }
                    }
                }
                else if (action->bone != nullptr) 
                {
                    for (const auto slot : _armature->getSlots())
                    {
                        const auto childArmature = slot->getChildArmature();
                        if (childArmature != nullptr && slot->getParent()->boneData == action->bone) 
                        {
                            childArmature->_bufferAction(action, true);
                        }
                    }
                }
                else 
                {
                    _armature->_bufferAction(action, true);
                }
            }
            else
            {
                const auto eventType = action->type == ActionType::Frame ? EventObject::FRAME_EVENT : EventObject::SOUND_EVENT;
                if (action->type == ActionType::Sound || eventDispatcher->hasEvent(eventType))
                {
                    const auto eventObject = BaseObject::borrowObject<EventObject>();
                    eventObject->time = (float)_frameArray[frameOffset] / _frameRate;
                    eventObject->type = eventType;
                    eventObject->name = action->name;
                    eventObject->data = action->data;
                    eventObject->armature = _armature;
                    eventObject->animationState = _animationState;

                    if (action->bone != nullptr)
                    {
                        eventObject->bone = _armature->getBone(action->bone->name);
                    }

                    if (action->slot != nullptr)
                    {
                        eventObject->slot = _armature->getSlot(action->slot->name);
                    }

                    _armature->_dragonBones->bufferEvent(eventObject);
                }
            }
        }
    }
}

void ActionTimelineState::update(float passedTime)
{
    const auto prevState = playState;
    auto prevPlayTimes = currentPlayTimes;
    auto prevTime = currentTime;

    if (playState <= 0 && _setCurrentTime(passedTime)) 
    {
        const auto eventDispatcher = _armature->getProxy();
        if (prevState < 0) 
        {
            if (playState != prevState)
            {
                if (_animationState->displayControl && _animationState->resetToPose) // Reset zorder to pose.
                {
                    _armature->_sortZOrder(nullptr, 0);
                }

                prevPlayTimes = currentPlayTimes;

                if (eventDispatcher->hasEvent(EventObject::START))
                {
                    const auto eventObject = BaseObject::borrowObject<EventObject>();
                    eventObject->type = EventObject::START;
                    eventObject->armature = _armature;
                    eventObject->animationState = _animationState;
                    _armature->_dragonBones->bufferEvent(eventObject);
                }
            }
            else 
            {
                return;
            }
        }

        const auto isReverse = _animationState->timeScale < 0.0f;
        EventObject* loopCompleteEvent = nullptr;
        EventObject* completeEvent = nullptr;
        if (currentPlayTimes != prevPlayTimes) 
        {
            if (eventDispatcher->hasEvent(EventObject::LOOP_COMPLETE))
            {
                loopCompleteEvent = BaseObject::borrowObject<EventObject>();
                loopCompleteEvent->type = EventObject::LOOP_COMPLETE;
                loopCompleteEvent->armature = _armature;
                loopCompleteEvent->animationState = _animationState;
            }

            if (playState > 0) 
            {
                if (eventDispatcher->hasEvent(EventObject::COMPLETE))
                {
                    completeEvent = BaseObject::borrowObject<EventObject>();
                    completeEvent->type = EventObject::COMPLETE;
                    completeEvent->armature = _armature;
                    completeEvent->animationState = _animationState;
                }
            }
        }

        if (_frameCount > 1) 
        {
            const auto timelineData = _timelineData;
            const auto timelineFrameIndex = (unsigned)(currentTime * _frameRate); // uint
            const auto frameIndex = (*_frameIndices)[timelineData->frameIndicesOffset + timelineFrameIndex];
            if ((unsigned)_frameIndex != frameIndex) // Arrive at frame.  
            {
                auto crossedFrameIndex = _frameIndex;
                _frameIndex = frameIndex;
                if (_timelineArray != nullptr)
                {
                    _frameOffset = _animationData->frameOffset + _timelineArray[timelineData->offset + (unsigned)BinaryOffset::TimelineFrameOffset + _frameIndex];
                    if (isReverse) 
                    {
                        if (crossedFrameIndex < 0) 
                        {
                            const auto prevFrameIndex = (unsigned)(prevTime * _frameRate);
                            crossedFrameIndex = (*_frameIndices)[timelineData->frameIndicesOffset + prevFrameIndex];
                            if (currentPlayTimes == prevPlayTimes) // Start.
                            {
                                if ((unsigned)crossedFrameIndex == frameIndex) // Uncrossed.
                                {
                                    crossedFrameIndex = -1;
                                }
                            }
                        }

                        while (crossedFrameIndex >= 0) 
                        {
                            const auto frameOffset = _animationData->frameOffset + _timelineArray[timelineData->offset + (unsigned)BinaryOffset::TimelineFrameOffset + crossedFrameIndex];
                            const auto framePosition = (float)_frameArray[frameOffset] / _frameRate;
                            if (
                                _position <= framePosition &&
                                framePosition <= _position + _duration
                            )  // Support interval play.
                            {
                                _onCrossFrame(crossedFrameIndex);
                            }

                            if (loopCompleteEvent != nullptr && crossedFrameIndex == 0) // Add loop complete event after first frame.
                            { 
                                _armature->_dragonBones->bufferEvent(loopCompleteEvent);
                                loopCompleteEvent = nullptr;
                            }

                            if (crossedFrameIndex > 0) 
                            {
                                crossedFrameIndex--;
                            }
                            else 
                            {
                                crossedFrameIndex = _frameCount - 1;
                            }

                            if ((unsigned)crossedFrameIndex ==  frameIndex)
                            {
                                break;
                            }
                        }
                    }
                    else 
                    {
                        if (crossedFrameIndex < 0)
                        {
                            const auto prevFrameIndex = (unsigned)(prevTime * _frameRate);
                            crossedFrameIndex = (*_frameIndices)[timelineData->frameIndicesOffset + prevFrameIndex];
                            const auto frameOffset = _animationData->frameOffset + _timelineArray[timelineData->offset + (unsigned)BinaryOffset::TimelineFrameOffset + crossedFrameIndex];
                            const auto framePosition = (float)_frameArray[frameOffset] / _frameRate;
                            if (currentPlayTimes == prevPlayTimes) // Start.
                            { 
                                if (prevTime <= framePosition) // Crossed.
                                {
                                    if (crossedFrameIndex > 0) 
                                    {
                                        crossedFrameIndex--;
                                    }
                                    else 
                                    {
                                        crossedFrameIndex = _frameCount - 1;
                                    }
                                }
                                else if ((unsigned)crossedFrameIndex == frameIndex) // Uncrossed.
                                { 
                                    crossedFrameIndex = -1;
                                }
                            }
                        }

                        while (crossedFrameIndex >= 0) 
                        {
                            if ((unsigned)crossedFrameIndex < _frameCount - 1) 
                            {
                                crossedFrameIndex++;
                            }
                            else 
                            {
                                crossedFrameIndex = 0;
                            }

                            const auto frameOffset = _animationData->frameOffset + _timelineArray[timelineData->offset + (unsigned)BinaryOffset::TimelineFrameOffset + crossedFrameIndex];
                            const auto framePosition = (float)_frameArray[frameOffset] / _frameRate;
                            if (
                                _position <= framePosition &&
                                framePosition <= _position + _duration
                            ) // Support interval play.
                            {
                                _onCrossFrame(crossedFrameIndex);
                            }

                            if (loopCompleteEvent != nullptr && crossedFrameIndex == 0) // Add loop complete event before first frame.
                            {
                                _armature->_dragonBones->bufferEvent(loopCompleteEvent);
                                loopCompleteEvent = nullptr;
                            }

                            if ((unsigned)crossedFrameIndex == frameIndex)
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
        else if (_frameIndex < 0) 
        {
            _frameIndex = 0;
            if (_timelineData != nullptr)
            {
                _frameOffset = _animationData->frameOffset + _timelineArray[_timelineData->offset + (unsigned)BinaryOffset::TimelineFrameOffset];
                // Arrive at frame.
                const auto framePosition = (float)_frameArray[_frameOffset] / _frameRate;
                if (currentPlayTimes == prevPlayTimes) // Start.
                {
                    if (prevTime <= framePosition) 
                    {
                        _onCrossFrame(_frameIndex);
                    }
                }
                else if (_position <= framePosition) // Loop complete.
                {
                    if (!isReverse && loopCompleteEvent != nullptr) // Add loop complete event before first frame.
                    {
                        _armature->_dragonBones->bufferEvent(loopCompleteEvent);
                        loopCompleteEvent = nullptr;
                    }

                    _onCrossFrame(_frameIndex);
                }
            }
        }

        if (loopCompleteEvent != nullptr)
        {
            _armature->_dragonBones->bufferEvent(loopCompleteEvent);
        }

        if (completeEvent != nullptr)
        {
            _armature->_dragonBones->bufferEvent(completeEvent);
        }
    }
}

void ActionTimelineState::setCurrentTime(float value)
{
    _setCurrentTime(value);
    _frameIndex = -1;
}

void ZOrderTimelineState::_onArriveAtFrame()
{
    if (playState >= 0) 
    {
        const auto count = _frameArray[_frameOffset + 1];
        if (count > 0) 
        {
            _armature->_sortZOrder(_frameArray, _frameOffset + 2);
        }
        else {
            _armature->_sortZOrder(nullptr, 0);
        }
    }
}

void BoneAllTimelineState::_onArriveAtFrame()
{
    BoneTimelineState::_onArriveAtFrame();

    if (_timelineData != nullptr) 
    {
        const auto frameFloatArray = _frameFloatArray;
        auto valueOffset = _animationData->frameFloatOffset + _frameValueOffset + _frameIndex * 6; // ...(timeline value offset)|xxxxxx|xxxxxx|(Value offset)xxxxx|(Next offset)xxxxx|xxxxxx|xxxxxx|...

        bonePose->current.x = frameFloatArray[valueOffset++];
        bonePose->current.y = frameFloatArray[valueOffset++];
        bonePose->current.rotation = frameFloatArray[valueOffset++];
        bonePose->current.skew = frameFloatArray[valueOffset++];
        bonePose->current.scaleX = frameFloatArray[valueOffset++];
        bonePose->current.scaleY = frameFloatArray[valueOffset++];

        if (_tweenState == TweenState::Always) 
        {
            if ((unsigned)_frameIndex == _frameCount - 1)
            {
                valueOffset = _animationData->frameFloatOffset + _frameValueOffset;
            }

            bonePose->delta.x = frameFloatArray[valueOffset++] - bonePose->current.x;
            bonePose->delta.y = frameFloatArray[valueOffset++] - bonePose->current.y;
            bonePose->delta.rotation = frameFloatArray[valueOffset++] - bonePose->current.rotation;
            bonePose->delta.skew = frameFloatArray[valueOffset++] - bonePose->current.skew;
            bonePose->delta.scaleX = frameFloatArray[valueOffset++] - bonePose->current.scaleX;
            bonePose->delta.scaleY = frameFloatArray[valueOffset++] - bonePose->current.scaleY;
        }
    }
    else
    {
        bonePose->current.x = 0.0f;
        bonePose->current.y = 0.0f;
        bonePose->current.rotation = 0.0f;
        bonePose->current.skew = 0.0f;
        bonePose->current.scaleX = 1.0f;
        bonePose->current.scaleY = 1.0f;
    }
}

void BoneAllTimelineState::_onUpdateFrame()
{
    BoneTimelineState::_onUpdateFrame();

    bone->_transformDirty = true;
    if (_tweenState != TweenState::Always) 
    {
        _tweenState = TweenState::None;
    }

    bonePose->result.x = bonePose->current.x + bonePose->delta.x * _tweenProgress;
    bonePose->result.y = bonePose->current.y + bonePose->delta.y * _tweenProgress;
    bonePose->result.rotation = bonePose->current.rotation + bonePose->delta.rotation * _tweenProgress;
    bonePose->result.skew = bonePose->current.skew + bonePose->delta.skew * _tweenProgress;
    bonePose->result.scaleX = bonePose->current.scaleX + bonePose->delta.scaleX * _tweenProgress;
    bonePose->result.scaleY = bonePose->current.scaleY + bonePose->delta.scaleY * _tweenProgress;
}

void BoneAllTimelineState::fadeOut()
{
    bonePose->result.rotation = Transform::normalizeRadian(bonePose->result.rotation);
    bonePose->result.skew = Transform::normalizeRadian(bonePose->result.skew);
}

void BoneTranslateTimelineState::_onArriveAtFrame()
{
    BoneTimelineState::_onArriveAtFrame();

    if (_timelineData != nullptr)
    {
        const auto frameFloatArray = _frameFloatArray;
        auto valueOffset = _animationData->frameFloatOffset + _frameValueOffset + _frameIndex * 2;

        bonePose->current.x = frameFloatArray[valueOffset++];
        bonePose->current.y = frameFloatArray[valueOffset++];

        if (_tweenState == TweenState::Always)
        {
            if ((unsigned)_frameIndex == _frameCount - 1)
            {
                valueOffset = _animationData->frameFloatOffset + _frameValueOffset;
            }

            bonePose->delta.x = frameFloatArray[valueOffset++] - bonePose->current.x;
            bonePose->delta.y = frameFloatArray[valueOffset++] - bonePose->current.y;
        }
    }
    else
    {
        bonePose->current.x = 0.0f;
        bonePose->current.y = 0.0f;
    }
}

void BoneTranslateTimelineState::_onUpdateFrame()
{
    BoneTimelineState::_onUpdateFrame();

    bone->_transformDirty = true;
    if (_tweenState != TweenState::Always)
    {
        _tweenState = TweenState::None;
    }

    bonePose->result.x = bonePose->current.x + bonePose->delta.x * _tweenProgress;
    bonePose->result.y = bonePose->current.y + bonePose->delta.y * _tweenProgress;
}

void BoneRotateTimelineState::_onArriveAtFrame()
{
    BoneTimelineState::_onArriveAtFrame();

    if (_timelineData != nullptr)
    {
        const auto frameFloatArray = _frameFloatArray;
        auto valueOffset = _animationData->frameFloatOffset + _frameValueOffset + _frameIndex * 2;

        bonePose->current.rotation = frameFloatArray[valueOffset++];
        bonePose->current.skew = frameFloatArray[valueOffset++];

        if (_tweenState == TweenState::Always)
        {
            if ((unsigned)_frameIndex == _frameCount - 1)
            {
                valueOffset = _animationData->frameFloatOffset + _frameValueOffset;
            }

            bonePose->delta.rotation = frameFloatArray[valueOffset++] - bonePose->current.rotation;
            bonePose->delta.skew = frameFloatArray[valueOffset++] - bonePose->current.skew;
        }
    }
    else
    {
        bonePose->current.rotation = 0.0f;
        bonePose->current.skew = 0.0f;
    }
}

void BoneRotateTimelineState::_onUpdateFrame()
{
    BoneTimelineState::_onUpdateFrame();

    bone->_transformDirty = true;
    if (_tweenState != TweenState::Always)
    {
        _tweenState = TweenState::None;
    }

    bonePose->result.rotation = bonePose->current.rotation + bonePose->delta.rotation * _tweenProgress;
    bonePose->result.skew = bonePose->current.skew + bonePose->delta.skew * _tweenProgress;
}

void BoneRotateTimelineState::fadeOut()
{
    bonePose->result.rotation = Transform::normalizeRadian(bonePose->result.rotation);
    bonePose->result.skew = Transform::normalizeRadian(bonePose->result.skew);
}

void BoneScaleTimelineState::_onArriveAtFrame()
{
    BoneTimelineState::_onArriveAtFrame();

    if (_timelineData != nullptr)
    {
        const auto frameFloatArray = _frameFloatArray;
        auto valueOffset = _animationData->frameFloatOffset + _frameValueOffset + _frameIndex * 2;
        bonePose->current.scaleX = frameFloatArray[valueOffset++];
        bonePose->current.scaleY = frameFloatArray[valueOffset++];

        if (_tweenState == TweenState::Always)
        {
            if ((unsigned)_frameIndex == _frameCount - 1)
            {
                valueOffset = _animationData->frameFloatOffset + _frameValueOffset;
            }

            bonePose->delta.scaleX = frameFloatArray[valueOffset++] - bonePose->current.scaleX;
            bonePose->delta.scaleY = frameFloatArray[valueOffset++] - bonePose->current.scaleY;
        }
    }
    else
    {
        bonePose->current.scaleX = 1.0f;
        bonePose->current.scaleY = 1.0f;
    }
}

void BoneScaleTimelineState::_onUpdateFrame()
{
    BoneTimelineState::_onUpdateFrame();

    bone->_transformDirty = true;
    if (_tweenState != TweenState::Always)
    {
        _tweenState = TweenState::None;
    }

    bonePose->result.scaleX = bonePose->current.scaleX + bonePose->delta.scaleX * _tweenProgress;
    bonePose->result.scaleY = bonePose->current.scaleY + bonePose->delta.scaleY * _tweenProgress;
}

void SlotDislayIndexTimelineState::_onArriveAtFrame()
{
    if (playState >= 0) 
    {
        const auto displayIndex = _timelineData != nullptr ? _frameArray[_frameOffset + 1] : slot->slotData->displayIndex;
        if (slot->getDisplayIndex() != displayIndex) 
        {
            slot->_setDisplayIndex(displayIndex, true);
        }
    }
}

void SlotColorTimelineState::_onClear()
{
    SlotTimelineState::_onClear();

    _dirty = false;
}

void SlotColorTimelineState::_onArriveAtFrame()
{
    SlotTimelineState::_onArriveAtFrame();

    if (_timelineData != nullptr) 
    {
        const auto intArray = _dragonBonesData->intArray;
        const auto frameIntArray = _frameIntArray;
        const auto valueOffset = _animationData->frameIntOffset + _frameValueOffset + _frameIndex * 1; // ...(timeline value offset)|x|x|(Value offset)|(Next offset)|x|x|...
        unsigned colorOffset = frameIntArray[valueOffset];

        _current[0] = intArray[colorOffset++];
        _current[1] = intArray[colorOffset++];
        _current[2] = intArray[colorOffset++];
        _current[3] = intArray[colorOffset++];
        _current[4] = intArray[colorOffset++];
        _current[5] = intArray[colorOffset++];
        _current[6] = intArray[colorOffset++];
        _current[7] = intArray[colorOffset++];

        if (_tweenState == TweenState::Always) 
        {
            if ((unsigned)_frameIndex == _frameCount - 1)
            {
                colorOffset = frameIntArray[_animationData->frameIntOffset + _frameValueOffset];
            }
            else 
            {
                colorOffset = frameIntArray[valueOffset + 1 * 1];
            }

            _delta[0] = intArray[colorOffset++] - _current[0];
            _delta[1] = intArray[colorOffset++] - _current[1];
            _delta[2] = intArray[colorOffset++] - _current[2];
            _delta[3] = intArray[colorOffset++] - _current[3];
            _delta[4] = intArray[colorOffset++] - _current[4];
            _delta[5] = intArray[colorOffset++] - _current[5];
            _delta[6] = intArray[colorOffset++] - _current[6];
            _delta[7] = intArray[colorOffset++] - _current[7];
        }
    }
    else 
    {
        const auto color = slot->slotData->color;

        _current[0] = color->alphaMultiplier * 100.0f;
        _current[1] = color->redMultiplier * 100.0f;
        _current[2] = color->greenMultiplier * 100.0f;
        _current[3] = color->blueMultiplier * 100.0f;
        _current[4] = color->alphaOffset;
        _current[5] = color->redOffset;
        _current[6] = color->greenOffset;
        _current[7] = color->blueOffset;
    }
}

void SlotColorTimelineState::_onUpdateFrame()
{
    SlotTimelineState::_onUpdateFrame();

    _dirty = true;
    if (_tweenState != TweenState::Always) 
    {
        _tweenState = TweenState::None;
    }

    _result[0] = (_current[0] + _delta[0] * _tweenProgress) * 0.01f;
    _result[1] = (_current[1] + _delta[1] * _tweenProgress) * 0.01f;
    _result[2] = (_current[2] + _delta[2] * _tweenProgress) * 0.01f;
    _result[3] = (_current[3] + _delta[3] * _tweenProgress) * 0.01f;
    _result[4] = _current[4] + _delta[4] * _tweenProgress;
    _result[5] = _current[5] + _delta[5] * _tweenProgress;
    _result[6] = _current[6] + _delta[6] * _tweenProgress;
    _result[7] = _current[7] + _delta[7] * _tweenProgress;
}

void SlotColorTimelineState::fadeOut()
{
    _tweenState = TweenState::None;
    _dirty = false;
}

void SlotColorTimelineState::update(float passedTime)
{
    SlotTimelineState::update(passedTime);

    // Fade animation.
    if (_tweenState != TweenState::None || _dirty) 
    {
        auto& result = slot->_colorTransform;

        if (_animationState->_fadeState != 0 || _animationState->_subFadeState != 0)
        {
            if (
                result.alphaMultiplier != _result[0] ||
                result.redMultiplier != _result[1] ||
                result.greenMultiplier != _result[2] ||
                result.blueMultiplier != _result[3] ||
                result.alphaOffset != _result[4] ||
                result.redOffset != _result[5] ||
                result.greenOffset != _result[6] ||
                result.blueOffset != _result[7]
            ) 
            {
                const auto fadeProgress = pow(_animationState->_fadeProgress, 2);

                result.alphaMultiplier += (_result[0] - result.alphaMultiplier) * fadeProgress;
                result.redMultiplier += (_result[1] - result.redMultiplier) * fadeProgress;
                result.greenMultiplier += (_result[2] - result.greenMultiplier) * fadeProgress;
                result.blueMultiplier += (_result[3] - result.blueMultiplier) * fadeProgress;
                result.alphaOffset += (_result[4] - result.alphaOffset) * fadeProgress;
                result.redOffset += (_result[5] - result.redOffset) * fadeProgress;
                result.greenOffset += (_result[6] - result.greenOffset) * fadeProgress;
                result.blueOffset += (_result[7] - result.blueOffset) * fadeProgress;

                slot->_colorDirty = true;
            }
        }
        else if (_dirty) 
        {
            _dirty = false;
            if (
                result.alphaMultiplier != _result[0] ||
                result.redMultiplier != _result[1] ||
                result.greenMultiplier != _result[2] ||
                result.blueMultiplier != _result[3] ||
                result.alphaOffset != _result[4] ||
                result.redOffset != _result[5] ||
                result.greenOffset != _result[6] ||
                result.blueOffset != _result[7]
            ) 
            {
                result.alphaMultiplier = _result[0];
                result.redMultiplier = _result[1];
                result.greenMultiplier = _result[2];
                result.blueMultiplier = _result[3];
                result.alphaOffset = _result[4];
                result.redOffset = _result[5];
                result.greenOffset = _result[6];
                result.blueOffset = _result[7];

                slot->_colorDirty = true;
            }
        }
    }
}

void SlotFFDTimelineState::_onClear()
{
    SlotTimelineState::_onClear();

    meshOffset = 0;

    _dirty = false;
    _frameFloatOffset = 0;
    _ffdCount = 0;
    _valueCount = 0;
    _valueOffset = 0;
    _current.clear();
    _delta.clear();
    _result.clear();
}

void SlotFFDTimelineState::_onArriveAtFrame()
{
    SlotTimelineState::_onArriveAtFrame();

    if (_timelineData != nullptr) 
    {
        const auto isTween = _tweenState == TweenState::Always;
        const auto frameFloatArray = _frameFloatArray;
        const auto valueOffset = _animationData->frameFloatOffset + _frameValueOffset + _frameIndex * _valueCount;

        if (isTween) 
        {
            auto nextValueOffset = valueOffset + _valueCount;
            if ((unsigned)_frameIndex == _frameCount - 1)
            {
                nextValueOffset = _animationData->frameFloatOffset + _frameValueOffset;
            }

            for (std::size_t i = 0; i < _valueCount; ++i)
            {
                _delta[i] = frameFloatArray[nextValueOffset + i] - (_current[i] = frameFloatArray[valueOffset + i]);
            }
        }
        else 
        {
            for (std::size_t i = 0; i < _valueCount; ++i)
            {
                _current[i] = frameFloatArray[valueOffset + i];
            }
        }
    }
    else 
    {
        for (std::size_t i = 0; i < _valueCount; ++i)
        {
            _current[i] = 0.0f;
        }
    }
}

void SlotFFDTimelineState::_onUpdateFrame()
{
    SlotTimelineState::_onUpdateFrame();

    _dirty = true;
    if (_tweenState != TweenState::Always) 
    {
        _tweenState = TweenState::None;
    }

    for (std::size_t i = 0; i < _valueCount; ++i)
    {
        _result[i] = _current[i] + _delta[i] * _tweenProgress;
    }
}

void SlotFFDTimelineState::init(Armature* armature, AnimationState* animationState, TimelineData* timelineData)
{
    SlotTimelineState::init(armature, animationState, timelineData);

    if (_timelineData != nullptr) 
    {
        const auto frameIntOffset = _animationData->frameIntOffset + _timelineArray[_timelineData->offset + (unsigned)BinaryOffset::TimelineFrameValueCount];
        meshOffset = _frameIntArray[frameIntOffset + (unsigned)BinaryOffset::FFDTimelineMeshOffset];
        _ffdCount = _frameIntArray[frameIntOffset + (unsigned)BinaryOffset::FFDTimelineFFDCount];
        _valueCount = _frameIntArray[frameIntOffset + (unsigned)BinaryOffset::FFDTimelineValueCount];
        _valueOffset = _frameIntArray[frameIntOffset + (unsigned)BinaryOffset::FFDTimelineValueOffset];
        _frameFloatOffset = _frameIntArray[frameIntOffset + (unsigned)BinaryOffset::FFDTimelineFloatOffset] + _animationData->frameFloatOffset;
    }
    else 
    {
        _valueCount = 0;
    }

    _current.resize(_valueCount);
    _delta.resize(_valueCount, 0.0f);
    _result.resize(_valueCount);
}

void SlotFFDTimelineState::fadeOut()
{
    _tweenState = TweenState::None;
    _dirty = false;
}

void SlotFFDTimelineState::update(float passedTime)
{
    if (slot->_meshData == nullptr || slot->_meshData->offset != meshOffset) 
    {
        return;
    }

    SlotTimelineState::update(passedTime);

    if (_tweenState != TweenState::None || _dirty) 
    {
        auto& result = slot->_ffdVertices;
        if (_timelineData != nullptr)
        {
            if (_animationState->_fadeState != 0 || _animationState->_subFadeState != 0)
            {
                const auto fadeProgress = pow(_animationState->_fadeProgress, 2);
                for (std::size_t i = 0; i < _ffdCount; ++i) 
                {
                    if (i < _valueOffset) 
                    {
                        result[i] += (_frameFloatArray[_frameFloatOffset + i] - result[i]) * fadeProgress;
                    }
                    else if (i < _valueOffset + _valueCount)
                    {
                        result[i] += (_result[i - _valueOffset] - result[i]) * fadeProgress;
                    }
                    else 
                    {
                        result[i] += (_frameFloatArray[_frameFloatOffset + i - _valueCount] - result[i]) * fadeProgress;
                    }
                }

                slot->_meshDirty = true;
            }
            else if (_dirty) 
            {
                _dirty = false;

                for (std::size_t i = 0; i < _ffdCount; ++i) 
                {
                    if (i < _valueOffset) 
                    {
                        result[i] = _frameFloatArray[_frameFloatOffset + i];
                    }
                    else if (i < _valueOffset + _valueCount)
                    {
                        result[i] = _result[i - _valueOffset];
                    }
                    else 
                    {
                        result[i] = _frameFloatArray[_frameFloatOffset + i - _valueCount];
                    }
                }

                slot->_meshDirty = true;
            }
        }
        else 
        {
            _ffdCount = result.size(); //
            if (_animationState->_fadeState != 0 || _animationState->_subFadeState != 0) 
            {
                const auto fadeProgress = pow(_animationState->_fadeProgress, 4);
                for (std::size_t i = 0; i < _ffdCount; ++i) {
                    result[i] += (0.0f - result[i]) * fadeProgress;
                }

                slot->_meshDirty = true;
            }
            else if (_dirty) 
            {
                _dirty = false;

                for (std::size_t i = 0; i < _ffdCount; ++i) 
                {
                    result[i] = 0.0f;
                }

                slot->_meshDirty = true;
            }
        }
    }
}

DRAGONBONES_NAMESPACE_END
