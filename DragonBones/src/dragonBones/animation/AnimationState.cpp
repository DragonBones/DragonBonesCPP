#include "AnimationState.h"
#include "WorldClock.h"
#include "../model/DisplayData.h"
#include "../model/AnimationConfig.h"
#include "../model/AnimationData.h"
#include "../armature/Armature.h"
#include "../armature/Bone.h"
#include "../armature/Slot.h"
#include "../armature/Constraint.h"
#include "../event/EventObject.h"
#include "../event/IEventDispatcher.h"
#include "TimelineState.h"

DRAGONBONES_NAMESPACE_BEGIN

void AnimationState::_onClear()
{
    for (const auto timeline : _boneTimelines)
    {
        timeline->returnToPool();
    }

    for (const auto timeline : _slotTimelines)
    {
        timeline->returnToPool();
    }

    for (const auto timeline : _constraintTimelines)
    {
        timeline->returnToPool();
    }

    for (const auto& pair : _bonePoses)
    {
        pair.second->returnToPool();
    }

    if (_actionTimeline !=  nullptr) {
        _actionTimeline->returnToPool();
    }

    if (_zOrderTimeline != nullptr) {
        _zOrderTimeline->returnToPool();
    }

    actionEnabled = false;
    additiveBlending = false;
    displayControl = false;
    resetToPose = false;
    playTimes = 1;
    layer = 0;
    timeScale = 1.0f;
    weight = 1.0f;
    autoFadeOutTime = 0.0f;
    fadeTotalTime = 0.0f;
    name = "";
    group = "";

    _timelineDirty = true;
    _playheadState = 0;
    _fadeState = -1;
    _subFadeState = -1;
    _position = 0.0f;
    _duration = 0.0f;
    _fadeTime = 0.0f;
    _time = 0.0f;
    _fadeProgress = 0.0f;
    _weightResult = 0.0f;
    _boneMask.clear();
    _boneTimelines.clear();
    _slotTimelines.clear();
    _constraintTimelines.clear();
    _poseTimelines.clear();
    _bonePoses.clear();
    _animationData = nullptr;
    _armature = nullptr;
    _actionTimeline = nullptr;
    _zOrderTimeline = nullptr;
}

void AnimationState::_updateTimelines()
{
    { // Update bone timelines.
        std::map<std::string, std::vector<BoneTimelineState*>> boneTimelines;
        for (const auto timeline : _boneTimelines) // Create bone timelines map.
        {
            boneTimelines[timeline->bone->getName()].push_back(timeline);
        }

        for (const auto bone : _armature->getBones())
        {
            const auto& timelineName = bone->getName();
            if (!containsBoneMask(timelineName))
            {
                continue;
            }

            const auto timelineDatas = _animationData->getBoneTimelines(timelineName);
            const auto iterator = boneTimelines.find(timelineName);

            if (iterator != boneTimelines.end()) // Remove bone timeline from map.
            {
                boneTimelines.erase(iterator);
            }
            else // Create new bone timeline.
            {
                const auto bonePose = _bonePoses.find(timelineName) != _bonePoses.end() ? _bonePoses[timelineName] : (_bonePoses[timelineName] = BaseObject::borrowObject<BonePose>());

                if (timelineDatas != nullptr)
                {
                    for (const auto timelineData : *timelineDatas)
                    {
                        switch (timelineData->type)
                        {
                        case TimelineType::BoneAll:
                        {
                            const auto timeline = BaseObject::borrowObject<BoneAllTimelineState>();
                            timeline->bone = bone;
                            timeline->bonePose = bonePose;
                            timeline->init(_armature, this, timelineData);
                            _boneTimelines.push_back(timeline);
                            break;
                        }

                        case TimelineType::BoneTranslate:
                        {
                            const auto timeline = BaseObject::borrowObject<BoneTranslateTimelineState>();
                            timeline->bone = bone;
                            timeline->bonePose = bonePose;
                            timeline->init(_armature, this, timelineData);
                            _boneTimelines.push_back(timeline);
                            break;
                        }

                        case TimelineType::BoneRotate:
                        {
                            const auto timeline = BaseObject::borrowObject<BoneRotateTimelineState>();
                            timeline->bone = bone;
                            timeline->bonePose = bonePose;
                            timeline->init(_armature, this, timelineData);
                            _boneTimelines.push_back(timeline);
                            break;
                        }

                        case TimelineType::BoneScale:
                        {
                            const auto timeline = BaseObject::borrowObject<BoneScaleTimelineState>();
                            timeline->bone = bone;
                            timeline->bonePose = bonePose;
                            timeline->init(_armature, this, timelineData);
                            _boneTimelines.push_back(timeline);
                            break;
                        }

                        default:
                            break;
                        }
                    }
                }
                else if (resetToPose) // Pose timeline.
                {
                    const auto timeline = BaseObject::borrowObject<BoneAllTimelineState>();
                    timeline->bone = bone;
                    timeline->bonePose = bonePose;
                    timeline->init(_armature, this, nullptr);
                    _boneTimelines.push_back(timeline);
                    _poseTimelines.push_back(std::make_pair(timeline, BaseTimelineType::Bone));
                }
            }
        }

        for (const auto& pair : boneTimelines) // Remove bone timelines.
        {
            for (const auto timeline : pair.second)
            {
                _boneTimelines.erase(std::find(_boneTimelines.begin(), _boneTimelines.end(), timeline));
                timeline->returnToPool();
            }
        }
    }

    { // Update slot timelines.
        std::map<std::string, std::vector<SlotTimelineState*>> slotTimelines;
        std::vector<unsigned> ffdFlags;
        for (const auto timeline : _slotTimelines) // Create slot timelines map.
        {
            slotTimelines[timeline->slot->getName()].push_back(timeline);
        }

        for (const auto slot : _armature->getSlots())
        {
            const auto& boneName = slot->getParent()->getName();
            if (!containsBoneMask(boneName))
            {
                continue;
            }

            const auto& timelineName = slot->getName();
            const auto timelineDatas = _animationData->getSlotTimelines(timelineName);
            const auto iterator = slotTimelines.find(timelineName);

            if (iterator != slotTimelines.end()) // Remove slot timeline from map.
            {
                slotTimelines.erase(iterator);
            }
            else // Create new slot timeline.
            {
                auto displayIndexFlag = false;
                auto colorFlag = false;
                ffdFlags.clear();

                if (timelineDatas != nullptr)
                {
                    for (const auto timelineData : *timelineDatas)
                    {
                        switch (timelineData->type)
                        {
                        case TimelineType::SlotDisplay:
                        {
                            const auto timeline = BaseObject::borrowObject<SlotDislayTimelineState>();
                            timeline->slot = slot;
                            timeline->init(_armature, this, timelineData);
                            _slotTimelines.push_back(timeline);
                            displayIndexFlag = true;
                            break;
                        }

                        case TimelineType::SlotColor:
                        {
                            const auto timeline = BaseObject::borrowObject<SlotColorTimelineState>();
                            timeline->slot = slot;
                            timeline->init(_armature, this, timelineData);
                            _slotTimelines.push_back(timeline);
                            colorFlag = true;
                            break;
                        }

                        case TimelineType::SlotFFD:
                        {
                            const auto timeline = BaseObject::borrowObject<SlotFFDTimelineState>();
                            timeline->slot = slot;
                            timeline->init(_armature, this, timelineData);
                            _slotTimelines.push_back(timeline);
                            ffdFlags.push_back(timeline->meshOffset);
                            break;
                        }

                        default:
                            break;
                        }
                    }
                }

                if (resetToPose) // Pose timeline.
                {
                    if (!displayIndexFlag)
                    {
                        const auto timeline = BaseObject::borrowObject<SlotDislayTimelineState>();
                        timeline->slot = slot;
                        timeline->init(_armature, this, nullptr);
                        _slotTimelines.push_back(timeline);
                        _poseTimelines.push_back(std::make_pair(timeline, BaseTimelineType::Slot));
                    }

                    if (!colorFlag)
                    {
                        const auto timeline = BaseObject::borrowObject<SlotColorTimelineState>();
                        timeline->slot = slot;
                        timeline->init(_armature, this, nullptr);
                        _slotTimelines.push_back(timeline);
                        _poseTimelines.push_back(std::make_pair(timeline, BaseTimelineType::Slot));
                    }

                    if (slot->getRawDisplayDatas() != nullptr)
                    {
                        for (const auto displayData : *(slot->getRawDisplayDatas()))
                        {
                            if (displayData != nullptr && displayData->type == DisplayType::Mesh)
                            {
                                const auto meshOffset = static_cast<MeshDisplayData*>(displayData)->offset;
                                if (std::find(ffdFlags.cbegin(), ffdFlags.cend(), meshOffset) == ffdFlags.cend())
                                {
                                    const auto timeline = BaseObject::borrowObject<SlotFFDTimelineState>();
                                    timeline->meshOffset = meshOffset;
                                    timeline->slot = slot;
                                    timeline->init(_armature, this, nullptr);
                                    _slotTimelines.push_back(timeline);
                                    _poseTimelines.push_back(std::make_pair(timeline, BaseTimelineType::Slot));
                                }
                            }
                        }
                    }
                }
            }
        }

        for (const auto& pair : slotTimelines) // Remove slot timelines.
        {
            for (const auto timeline : pair.second)
            {
                _slotTimelines.erase(std::find(_slotTimelines.begin(), _slotTimelines.end(), timeline));
                timeline->returnToPool();
            }
        }
    }

    { // Update constraint timelines.
        std::map<std::string, std::vector<ConstraintTimelineState*>> constraintTimelines;
        for (const auto timeline : _constraintTimelines) // Create constraint timelines map.
        {
            constraintTimelines[timeline->constraint->getName()].push_back(timeline);
        }

        for (const auto constraint : _armature->_constraints) 
        {
            const auto& timelineName = constraint->getName();
            const auto timelineDatas = _animationData->getConstraintTimelines(timelineName);
            const auto iterator = constraintTimelines.find(timelineName);

            if (iterator != constraintTimelines.end()) // Remove constraint timeline from map.
            {
                constraintTimelines.erase(iterator);
            }
            else  // Create new constraint timeline.
            {
                if (timelineDatas != nullptr) 
                {
                    for (const auto timelineData : *timelineDatas) 
                    {
                        switch (timelineData->type) 
                        {
                            case TimelineType::IKConstraint:
                            {
                                const auto timeline = BaseObject::borrowObject<IKConstraintTimelineState>();
                                timeline->constraint = constraint;
                                timeline->init(_armature, this, timelineData);
                                _constraintTimelines.push_back(timeline);
                                break;
                            }

                            default:
                                break;
                        }
                    }
                }
                else if (resetToPose) // Pose timeline.
                {
                    const auto timeline = BaseObject::borrowObject<IKConstraintTimelineState>();
                    timeline->constraint = constraint;
                    timeline->init(_armature, this, nullptr);
                    _constraintTimelines.push_back(timeline);
                    _poseTimelines.push_back(std::make_pair(timeline, BaseTimelineType::Constraint));
                }
            }
        }
    }
}

void AnimationState::_advanceFadeTime(float passedTime)
{
    const auto isFadeOut = _fadeState > 0;

    if (_subFadeState < 0)
    {
        _subFadeState = 0;

        const auto eventType = isFadeOut ? EventObject::FADE_OUT : EventObject::FADE_IN;
        if (_armature->getProxy()->hasDBEventListener(eventType))
        {
            const auto eventObject = BaseObject::borrowObject<EventObject>();
            eventObject->type = eventType;
            eventObject->armature = _armature;
            eventObject->animationState = this;
            _armature->_dragonBones->bufferEvent(eventObject);
        }
    }

    if (passedTime < 0.0f)
    {
        passedTime = -passedTime;
    }

    _fadeTime += passedTime;

    if (_fadeTime >= fadeTotalTime)
    {
        _subFadeState = 1;
        _fadeProgress = isFadeOut ? 0.0f : 1.0f;
    }
    else if (_fadeTime > 0.0f)
    {
        _fadeProgress = isFadeOut ? (1.0f - _fadeTime / fadeTotalTime) : (_fadeTime / fadeTotalTime);
    }
    else
    {
        _fadeProgress = isFadeOut ? 1.0f : 0.0f;
    }

    if (_subFadeState > 0)
    {
        if (!isFadeOut)
        {
            _playheadState |= 1; // x1
            _fadeState = 0;
        }

        const auto eventType = isFadeOut ? EventObject::FADE_OUT_COMPLETE : EventObject::FADE_IN_COMPLETE;
        if (_armature->getProxy()->hasDBEventListener(eventType))
        {
            const auto eventObject = BaseObject::borrowObject<EventObject>();
            eventObject->type = eventType;
            eventObject->armature = _armature;
            eventObject->animationState = this;
            _armature->_dragonBones->bufferEvent(eventObject);
        }
    }
}

void AnimationState::_blendBoneTimline(BoneTimelineState * timeline) const
{
    auto boneWeight = _weightResult > 0.0f ? _weightResult : -_weightResult;
    const auto bone = timeline->bone;
    const auto bonePose = &(timeline->bonePose->result);
    const auto animationPose = &(bone->animationPose);

    if (!bone->_blendDirty) 
    {
        bone->_blendDirty = true;
        bone->_blendLayer = layer;
        bone->_blendLayerWeight = boneWeight;
        bone->_blendLeftWeight = 1.0f;
        //
        animationPose->x = bonePose->x * boneWeight;
        animationPose->y = bonePose->y * boneWeight;
        animationPose->rotation = bonePose->rotation * boneWeight;
        animationPose->skew = bonePose->skew * boneWeight;
        animationPose->scaleX = (bonePose->scaleX - 1.0f) * boneWeight + 1.0f;
        animationPose->scaleY = (bonePose->scaleY - 1.0f) * boneWeight + 1.0f;
    }
    else 
    {
        boneWeight *= bone->_blendLeftWeight;
        bone->_blendLayerWeight += boneWeight;
        //
        animationPose->x += bonePose->x * boneWeight;
        animationPose->y += bonePose->y * boneWeight;
        animationPose->rotation += bonePose->rotation * boneWeight;
        animationPose->skew += bonePose->skew * boneWeight;
        animationPose->scaleX += (bonePose->scaleX - 1.0f) * boneWeight;
        animationPose->scaleY += (bonePose->scaleY - 1.0f) * boneWeight;
    }

    if (_fadeState != 0 || _subFadeState != 0)
    {
        bone->_transformDirty = true;
    }
}

void AnimationState::init(Armature* parmature, AnimationData* panimationData, AnimationConfig* animationConfig)
{
    if (_armature != nullptr) {
        return;
    }

    _armature = parmature;
    _animationData = panimationData;
    //
    resetToPose = animationConfig->resetToPose;
    additiveBlending = animationConfig->additiveBlending;
    displayControl = animationConfig->displayControl;
    actionEnabled = animationConfig->actionEnabled;
    layer = animationConfig->layer;
    playTimes = animationConfig->playTimes;
    timeScale = animationConfig->timeScale;
    fadeTotalTime = animationConfig->fadeInTime;
    autoFadeOutTime = animationConfig->autoFadeOutTime;
    weight = animationConfig->weight;
    name = !animationConfig->name.empty() ? animationConfig->name : animationConfig->animation;
    group = animationConfig->group;

    if (animationConfig->pauseFadeIn) 
    {
        _playheadState = 2; // 10
    }
    else 
    {
        _playheadState = 3; // 11
    }

    if (animationConfig->duration < 0.0f) 
    {
        _position = 0.0f;
        _duration = _animationData->duration;

        if (animationConfig->position != 0.0f) 
        {
            if (timeScale >= 0.0f) 
            {
                _time = animationConfig->position;
            }
            else 
            {
                _time = animationConfig->position - _duration;
            }
        }
        else 
        {
            _time = 0.0f;
        }
    }
    else 
    {
        _position = animationConfig->position;
        _duration = animationConfig->duration;
        _time = 0.0f;
    }

    if (timeScale < 0.0f && _time == 0.0f) 
    {
        _time = -0.000001f; // Can not cross last frame event.
    }

    if (fadeTotalTime <= 0.0f)
    {
        _fadeProgress = 0.999999f;
    }

    if (!animationConfig->boneMask.empty())
    {
        _boneMask.resize(animationConfig->boneMask.size());
        for (std::size_t i = 0, l = _boneMask.size(); i < l; ++i) {
            _boneMask[i] = animationConfig->boneMask[i];
        }
    }

    _actionTimeline = BaseObject::borrowObject<ActionTimelineState>();
    _actionTimeline->init(_armature, this, _animationData->actionTimeline); //
    _actionTimeline->currentTime = _time;
    if (_actionTimeline->currentTime < 0.0f)
    {
        _actionTimeline->currentTime = _duration - _actionTimeline->currentTime;
    }

    if (_animationData->zOrderTimeline != nullptr)
    {
        _zOrderTimeline = BaseObject::borrowObject<ZOrderTimelineState>();
        _zOrderTimeline->init(_armature, this, _animationData->zOrderTimeline);
    }
}

void AnimationState::advanceTime(float passedTime, float cacheFrameRate)
{
    // Update fade time.
    if (_fadeState != 0 || _subFadeState != 0) 
    {
        _advanceFadeTime(passedTime);
    }

    // Update time.
    if (_playheadState == 3) // 11
    {
        if (timeScale != 1.0f) 
        {
            passedTime *= timeScale;
        }

        _time += passedTime;
    }

    if (_timelineDirty) 
    {
        _timelineDirty = false;
        _updateTimelines();
    }

    if (weight == 0.0f) 
    {
        return;
    }

    const auto isCacheEnabled = _fadeState == 0 && cacheFrameRate > 0.0f;
    auto isUpdateTimeline = true;
    auto isUpdateBoneTimeline = true;
    auto time = _time;
    _weightResult = weight * _fadeProgress;

    if (_actionTimeline->playState <= 0)
    {
        _actionTimeline->update(time); // Update main timeline.
    }

    if (isCacheEnabled) // Cache time internval.
    {
        const auto internval = cacheFrameRate * 2.0f;
        _actionTimeline->currentTime = (unsigned)(_actionTimeline->currentTime * internval) / internval;
    }

    if (_zOrderTimeline != nullptr && _zOrderTimeline->playState <= 0) // Update zOrder timeline.
    {
        _zOrderTimeline->update(time);
    }

    if (isCacheEnabled) // Update cache.
    {
        const auto cacheFrameIndex = (unsigned)(_actionTimeline->currentTime * cacheFrameRate); // uint
        if ((unsigned)_armature->_cacheFrameIndex == cacheFrameIndex) // Same cache.
        { 
            isUpdateTimeline = false;
            isUpdateBoneTimeline = false;
        }
        else 
        {
            _armature->_cacheFrameIndex = cacheFrameIndex;
            if (_animationData->cachedFrames[cacheFrameIndex]) // Cached.
            { 
                isUpdateBoneTimeline = false;
            }
            else // Cache.
            { 
                _animationData->cachedFrames[cacheFrameIndex] = true;
            }
        }
    }

    if (isUpdateTimeline) 
    {
        if (isUpdateBoneTimeline) // Update bone timelines.
        {
            Bone* bone = nullptr;
            BoneTimelineState* prevTimeline = nullptr;

            for (std::size_t i = 0, l = _boneTimelines.size(); i < l; ++i) 
            {
                const auto timeline = _boneTimelines[i];
                if (bone != timeline->bone) // Blend bone pose.
                { 
                    if (bone != nullptr) 
                    {
                        _blendBoneTimline(prevTimeline);
                        if (bone->_blendDirty)
                        {
                            if (bone->_blendLeftWeight > 0.0f) 
                            {
                                if ((unsigned)bone->_blendLayer != layer)
                                {
                                    if (bone->_blendLayerWeight >= bone->_blendLeftWeight) {
                                        bone->_blendLeftWeight = 0.0f;
                                        bone = nullptr;
                                    }
                                    else 
                                    {
                                        bone->_blendLayer = layer;
                                        bone->_blendLeftWeight -= bone->_blendLayerWeight;
                                        bone->_blendLayerWeight = 0.0f;
                                    }
                                }
                            }
                            else 
                            {
                                bone = nullptr;
                            }
                        }
                    }

                    bone = timeline->bone;
                }

                if (bone != nullptr)
                {
                    if (timeline->playState <= 0)
                    {
                        timeline->update(time);
                    }

                    if (i == l - 1)
                    {
                        _blendBoneTimline(timeline);
                    }
                    else 
                    {
                        prevTimeline = timeline;
                    }
                }
            }
        }

        if (displayControl) 
        {
            for (std::size_t i = 0, l = _slotTimelines.size(); i < l; ++i)
            {
                const auto timeline = _slotTimelines[i];
                const auto& displayController = timeline->slot->displayController;

                if (
                    displayController.empty() ||
                    displayController == name ||
                    displayController == group
                    )
                {
                    if (timeline->playState <= 0)
                    {
                        timeline->update(time);
                    }
                }
            }
        }

        for (std::size_t i = 0, l = _constraintTimelines.size(); i < l; ++i)
        {
            const auto timeline = _constraintTimelines[i];
            if (timeline->playState <= 0) 
            {
                timeline->update(time);
            }
        }
    }

    if (_fadeState == 0) 
    {
        if (_subFadeState > 0) 
        {
            _subFadeState = 0;

            if (!_poseTimelines.empty()) 
            {
                for (const auto& pair : _poseTimelines) 
                {
                    const auto timeline = pair.first;
                    if (pair.second == BaseTimelineType::Bone)
                    { 
                        _boneTimelines.erase(std::find(_boneTimelines.begin(), _boneTimelines.end(), timeline));
                    }
                    else if (pair.second == BaseTimelineType::Slot)
                    {
                        _slotTimelines.erase(std::find(_slotTimelines.begin(), _slotTimelines.end(), timeline));
                    }
                    else if (pair.second == BaseTimelineType::Constraint)
                    {
                        _constraintTimelines.erase(std::find(_constraintTimelines.begin(), _constraintTimelines.end(), timeline));
                    }

                    timeline->returnToPool();
                }

                _poseTimelines.clear();
            }
        }

        if (_actionTimeline->playState > 0)
        {
            if (autoFadeOutTime >= 0.0f) // Auto fade out.
            {
                fadeOut(autoFadeOutTime);
            }
        }
    }
}

void AnimationState::play()
{
    _playheadState = 3; // 11
}

void AnimationState::stop()
{
    _playheadState &= 1; // 0x
}

void AnimationState::fadeOut(float fadeOutTime, bool pausePlayhead)
{
    if (fadeOutTime < 0.0f)
    {
        fadeOutTime = 0.0f;
    }

    if (pausePlayhead) 
    {
        _playheadState &= 2; // x0
    }

    if (_fadeState > 0)
    {
        if (fadeOutTime > fadeTotalTime - _fadeTime) // If the animation is already in fade out, the new fade out will be ignored.
        {
            return;
        }
    }
    else
    {
        _fadeState = 1;
        _subFadeState = -1;

        if (fadeOutTime <= 0.0f || _fadeProgress <= 0.0f)
        {
            _fadeProgress = 0.000001f; // Modify fade progress to different value.
        }

        for (const auto timeline : _boneTimelines)
        {
            timeline->fadeOut();
        }

        for (const auto timeline : _slotTimelines)
        {
            timeline->fadeOut();
        }
    }

    displayControl = false; //
    fadeTotalTime = _fadeProgress > 0.000001f ? fadeOutTime / _fadeProgress : 0.0f;
    _fadeTime = fadeTotalTime * (1.0f - _fadeProgress);
}

bool AnimationState::containsBoneMask(const std::string& name) const
{
    return _boneMask.empty() || std::find(_boneMask.cbegin(), _boneMask.cend(), name) != _boneMask.cend();
}

void AnimationState::addBoneMask(const std::string& name, bool recursive)
{
    const auto currentBone = _armature->getBone(name);
    if (currentBone == nullptr)
    {
        return;
    }

    if (std::find(_boneMask.cbegin(), _boneMask.cend(), name) == _boneMask.cend())
    {
        _boneMask.push_back(name);
    }

    if (recursive) // Add recursive mixing.
    {
        for (const auto bone : _armature->getBones())
        {
            if (std::find(_boneMask.cbegin(), _boneMask.cend(), bone->getName()) == _boneMask.cend() && currentBone->contains(bone))
            {
                _boneMask.push_back(bone->getName());
            }
        }
    }

    _timelineDirty = true;
}

void AnimationState::removeBoneMask(const std::string& name, bool recursive)
{
    auto iterator = std::find(_boneMask.begin(), _boneMask.end(), name);
    if (iterator != _boneMask.cend()) // Remove mixing.
    {
        _boneMask.erase(iterator);
    }

    if (recursive)
    {
        const auto currentBone = _armature->getBone(name);
        if (currentBone != nullptr)
        {
            const auto& bones = _armature->getBones();
            if (!_boneMask.empty()) // Remove recursive mixing.
            {
                for (const auto bone : bones)
                {
                    auto iterator = std::find(_boneMask.begin(), _boneMask.end(), bone->getName());
                    if (iterator != _boneMask.end() && currentBone->contains(bone))
                    {
                        _boneMask.erase(iterator);
                    }
                }
            }
            else // Add unrecursive mixing.
            {
                for (const auto bone : bones)
                {
                    if (bone == currentBone) 
                    {
                        continue;
                    }

                    if (!currentBone->contains(bone)) 
                    {
                        _boneMask.push_back(bone->getName());
                    }
                }
            }
        }
    }

    _timelineDirty = true;
}

void AnimationState::removeAllBoneMask()
{
    _boneMask.clear();
    _timelineDirty = true;
}

bool AnimationState::isPlaying() const
{
    return (_playheadState & 2) != 0 && _actionTimeline->playState <= 0;
}

bool AnimationState::isCompleted() const
{
    return _actionTimeline->playState > 0;
}

unsigned AnimationState::getCurrentPlayTimes() const
{
    return _actionTimeline->currentPlayTimes;
}

float AnimationState::getCurrentTime() const
{
    return _actionTimeline->currentTime;
}

void AnimationState::setCurrentTime(float value)
{
    const auto currentPlayTimes = _actionTimeline->currentPlayTimes - (_actionTimeline->playState > 0 ? 1 : 0);
    if (value < 0.0f || _duration < value) 
    {
        value = fmod(value, _duration) + currentPlayTimes * _duration;
        if (value < 0.0f) {
            value += _duration;
        }
    }

    if (_time == value) {
        return;
    }

    _time = value;
    _actionTimeline->setCurrentTime(_time);

    if (_zOrderTimeline != nullptr) {
        _zOrderTimeline->playState = -1;
    }

    for (const auto timeline : _boneTimelines)
    {
        timeline->playState = -1;
    }

    for (const auto timeline : _slotTimelines)
    {
        timeline->playState = -1;
    }
}

void BonePose::_onClear()
{
    current.identity();
    delta.identity();
    result.identity();
}

DRAGONBONES_NAMESPACE_END
