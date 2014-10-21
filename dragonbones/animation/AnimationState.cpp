#include <cstdlib>
#include "AnimationState.h"

NAME_SPACE_DRAGON_BONES_BEGIN
std::vector<AnimationState*> AnimationState::_pool;

AnimationState* AnimationState::borrowObject()
{
    if (_pool.empty())
    {
        return new AnimationState();
    }
    
    AnimationState *animationState = _pool.back();
    _pool.pop_back();
    return animationState;
}

void AnimationState::returnObject(AnimationState *animationState)
{
    auto iterator = std::find(_pool.cbegin(), _pool.cend(), animationState);
    
    if (iterator == _pool.end())
    {
        _pool.push_back(animationState);
    }
    
    animationState->clear();
}

void AnimationState::clearObjects()
{
    for (size_t i = 0, l = _pool.size(); i < l; ++i)
    {
        _pool[i]->clear();
        delete _pool[i];
    }
    
    _pool.clear();
}

bool AnimationState::getIsComplete() const
{
    return _isComplete;
}

bool AnimationState::getIsPlaying() const
{
    return (_isPlaying && !_isComplete);
}

int AnimationState::getCurrentPlayTimes() const
{
    return _currentPlayTimes < 0 ? 0 : _currentPlayTimes;
}

int AnimationState::getLayer() const
{
    return _layer;
}

float AnimationState::getTotalTime() const
{
    return _totalTime * 0.001f;
}

float AnimationState::getCurrentWeight() const
{
    return _fadeWeight * weight;
}

const std::string& AnimationState::getGroup() const
{
    return _group;
}

const AnimationData* AnimationState::getClip() const
{
    return _clip;
}

AnimationState* AnimationState::setAdditiveBlending(bool value)
{
    additiveBlending = value;
    return this;
}

AnimationState* AnimationState::setAutoFadeOut(bool value, float fadeOutTime_)
{
    autoFadeOut = value;
    
    if (fadeOutTime_ >= 0)
    {
        fadeOutTime = fadeOutTime_;
    }
    
    return this;
}

AnimationState* AnimationState::setWeight(float value)
{
    weight = value;
    return this;
}

AnimationState* AnimationState::setFrameTween(bool autoTween_, bool lastFrameAutoTween_)
{
    autoTween = autoTween_;
    lastFrameAutoTween = lastFrameAutoTween_;
    return this;
}

int AnimationState::getPlayTimes() const
{
    return _playTimes;
}
AnimationState* AnimationState::setPlayTimes(int playTimes)
{
    _playTimes = playTimes;
    
    if (round(_totalTime * 0.001f * _clip->frameRate) < 2)
    {
        _playTimes = playTimes < 0 ? -1 : 1;
    }
    else
    {
        _playTimes = playTimes < 0 ? -playTimes : playTimes;
    }
    
    autoFadeOut = playTimes < 0 ? true : false;
    return this;
}

float AnimationState::getCurrentTime() const
{
    return _currentTime < 0 ? 0.f : _currentTime * 0.001f;
}
AnimationState* AnimationState::setCurrentTime(float currentTime)
{
    if (currentTime < 0 || currentTime != currentTime)
    {
        currentTime = 0.f;
    }
    
    _time = currentTime;
    _currentTime = static_cast<int>(_time * 1000.f);
    return this;
}

float AnimationState::getTimeScale() const
{
    return _timeScale;
}
AnimationState* AnimationState::setTimeScale(float timeScale)
{
    if (timeScale != timeScale)
    {
        timeScale = 1.f;
    }
    
    _timeScale = timeScale;
    return this;
}

AnimationState::AnimationState()
{
    _clip = nullptr;
    _armature = nullptr;
}
AnimationState::~AnimationState()
{
    clear();
}

void AnimationState::fadeIn(Armature *armature, AnimationData *clip, float fadeTotalTime, float timeScale, int playTimes, bool pausePlayhead)
{
    _armature = armature;
    _clip = clip;
    _pausePlayheadInFade = pausePlayhead;
    _totalTime = _clip->duration;
    autoTween = _clip->autoTween;
    name = _clip->name;
    setTimeScale(timeScale);
    setPlayTimes(playTimes);
    // reset
    _isComplete = false;
    _currentFrameIndex = -1;
    _currentPlayTimes = -1;
    
    if (round(_totalTime * 0.001f * _clip->frameRate) < 2)
    {
        _currentTime = _totalTime;
    }
    else
    {
        _currentTime = -1;
    }
    
    _time = 0.f;
    _mixingTransforms.clear();
    // fade start
    _isFadeOut = false;
    _fadeWeight = 0.f;
    _fadeTotalWeight = 1.f;
    _fadeCurrentTime = 0.f;
    _fadeBeginTime = _fadeCurrentTime;
    _fadeTotalTime = fadeTotalTime * _timeScale;
    _fadeState = FadeState::FADE_BEFORE;
    // default
    _isPlaying = true;
    displayControl = true;
    lastFrameAutoTween = true;
    additiveBlending = false;
    weight = 1.f;
    fadeOutTime = fadeTotalTime;
    updateTimelineStates();
}

AnimationState* AnimationState::fadeOut(float fadeTotalTime, bool pausePlayhead)
{
    if (!(fadeTotalTime >= 0))
    {
        fadeTotalTime = 0.f;
    }
    
    _pausePlayheadInFade = pausePlayhead;
    
    if (_isFadeOut)
    {
        if (fadeTotalTime > _fadeTotalTime / _timeScale - (_fadeCurrentTime - _fadeBeginTime))
        {
            return this;
        }
    }
    else
    {
        for (size_t i = 0, l = _timelineStateList.size(); i < l; ++i)
        {
            _timelineStateList[i]->fadeOut();
        }
    }
    
    // fade start
    _isFadeOut = true;
    _fadeTotalWeight = _fadeWeight;
    _fadeState = FadeState::FADE_BEFORE;
    _fadeBeginTime = _fadeCurrentTime;
    _fadeTotalTime = _fadeTotalWeight >= 0 ? fadeTotalTime * _timeScale : 0.f;
    // default
    displayControl = false;
    return this;
}

AnimationState* AnimationState::play()
{
    _isPlaying = true;
    return this;
}

AnimationState* AnimationState::stop()
{
    _isPlaying = false;
    return this;
}

bool AnimationState::getMixingTransform(const std::string &timelineName) const
{
    return std::find(_mixingTransforms.cbegin(), _mixingTransforms.cend(), timelineName) != _mixingTransforms.cend();
}

AnimationState* AnimationState::addMixingTransform(const std::string &timelineName, bool recursive)
{
    if (recursive)
    {
        Bone *currentBone = nullptr;
        
        // From root to leaf
        for (size_t i = _armature->getBones().size(); i--;)
        {
            Bone *bone = _armature->getBones()[i];
            const std::string &boneName = bone->name;
            
            if (boneName == timelineName)
            {
                currentBone = bone;
            }
            
            if (
                currentBone &&
                (currentBone == bone || currentBone->contains(bone)) &&
                _clip->getTimeline(boneName) &&
                std::find(_mixingTransforms.cbegin(), _mixingTransforms.cend(), boneName) == _mixingTransforms.cend()
            )
            {
                _mixingTransforms.push_back(boneName);
            }
        }
    }
    else if (
        _clip->getTimeline(timelineName) &&
        std::find(_mixingTransforms.cbegin(), _mixingTransforms.cend(), timelineName) == _mixingTransforms.cend()
    )
    {
        _mixingTransforms.push_back(timelineName);
    }
    
    updateTimelineStates();
    return this;
}

AnimationState* AnimationState::removeMixingTransform(const std::string &timelineName, bool recursive)
{
    if (recursive)
    {
        Bone *currentBone = nullptr;
        
        // From root to leaf
        for (size_t i = _armature->getBones().size(); i--;)
        {
            Bone *bone = _armature->getBones()[i];
            
            if (bone->name == timelineName)
            {
                currentBone = bone;
            }
            
            if (currentBone && (currentBone == bone || currentBone->contains(bone)))
            {
                auto iterator = std::find(_mixingTransforms.begin(), _mixingTransforms.end(), bone->name);
                
                if (iterator != _mixingTransforms.end())
                {
                    _mixingTransforms.erase(iterator);
                }
            }
        }
    }
    else
    {
        auto iterator = std::find(_mixingTransforms.begin(), _mixingTransforms.end(), timelineName);
        
        if (iterator != _mixingTransforms.end())
        {
            _mixingTransforms.erase(iterator);
        }
    }
    
    updateTimelineStates();
    return this;
}

AnimationState* AnimationState::removeAllMixingTransform()
{
    _mixingTransforms.clear();
    updateTimelineStates();
    return this;
}

bool AnimationState::advanceTime(float passedTime)
{
    passedTime *= _timeScale;
    advanceFadeTime(passedTime);
    
    if (_fadeWeight)
    {
        advanceTimelinesTime(passedTime);
    }
    
    return _isFadeOut && _fadeState == FadeState::FADE_COMPLETE;
}

void AnimationState::updateTimelineStates()
{
    for (size_t i = _timelineStateList.size(); i--;)
    {
        TimelineState *timelineState = _timelineStateList[i];
        
        if (!_armature->getBone(timelineState->name))
        {
            removeTimelineState(timelineState);
        }
    }
    
    if (_mixingTransforms.empty())
    {
        for (size_t i = 0, l = _clip->timelineList.size(); i < l; ++i)
        {
            addTimelineState(_clip->timelineList[i]->name);
        }
    }
    else
    {
        for (size_t i = _timelineStateList.size(); i--;)
        {
            TimelineState *timelineState = _timelineStateList[i];
            auto iterator = std::find(_mixingTransforms.cbegin(), _mixingTransforms.cend(), timelineState->name);
            
            if (iterator == _mixingTransforms.cend())
            {
                removeTimelineState(timelineState);
            }
        }
        
        for (size_t i = 0, l = _mixingTransforms.size(); i < l; ++i)
        {
            addTimelineState(_mixingTransforms[i]);
        }
    }
}

void AnimationState::addTimelineState(const std::string &timelineName)
{
    Bone *bone = _armature->getBone(timelineName);
    
    if (bone)
    {
        for (size_t i = 0, l = _timelineStateList.size(); i < l; ++i)
        {
            if (_timelineStateList[i]->name == timelineName)
            {
                return;
            }
        }
        
        TimelineState *timelineState = TimelineState::borrowObject();
        timelineState->fadeIn(bone, this, _clip->getTimeline(timelineName));
        _timelineStateList.push_back(timelineState);
    }
}

void AnimationState::removeTimelineState(TimelineState *timelineState)
{
    auto iterator = std::find(_timelineStateList.begin(), _timelineStateList.end(), timelineState);
    
    if (iterator != _timelineStateList.end())
    {
        TimelineState::returnObject(timelineState);
        _timelineStateList.erase(iterator);
    }
}

void AnimationState::advanceFadeTime(float passedTime)
{
    bool fadeStartFlg = false;
    bool fadeCompleteFlg = false;
    
    if (_fadeBeginTime >= 0)
    {
        FadeState fadeState = _fadeState;
        _fadeCurrentTime += passedTime < 0 ? -passedTime : passedTime;
        
        if (_fadeCurrentTime >= _fadeBeginTime + _fadeTotalTime)
        {
            // fade complete
            if (_fadeWeight == 1 || _fadeWeight == 0)
            {
                fadeState = FadeState::FADE_COMPLETE;
                
                if (_pausePlayheadInFade)
                {
                    _pausePlayheadInFade = false;
                    _currentTime = -1;
                }
            }
            
            _fadeWeight = _isFadeOut ? 0.f : 1.f;
        }
        else if (_fadeCurrentTime >= _fadeBeginTime)
        {
            // fading
            fadeState = FadeState::FADING;
            _fadeWeight = (_fadeCurrentTime - _fadeBeginTime) / _fadeTotalTime * _fadeTotalWeight;
            
            if (_isFadeOut)
            {
                _fadeWeight = _fadeTotalWeight - _fadeWeight;
            }
        }
        else
        {
            // fade before
            fadeState = FadeState::FADE_BEFORE;
            _fadeWeight = _isFadeOut ? 1.f : 0.f;
        }
        
        if (_fadeState != fadeState)
        {
            // _fadeState == FadeState::FADE_BEFORE && (fadeState == FadeState::FADING || fadeState == FadeState::FADE_COMPLETE)
            if (_fadeState == FadeState::FADE_BEFORE)
            {
                fadeStartFlg = true;
            }
            
            // (_fadeState == FadeState::FADE_BEFORE || _fadeState == FadeState::FADING) && fadeState == FadeState::FADE_COMPLETE
            if (fadeState == FadeState::FADE_COMPLETE)
            {
                fadeCompleteFlg = true;
            }
            
            _fadeState = fadeState;
        }
    }
    
    if (fadeStartFlg)
    {
        EventData::EventType eventDataType;
        
        if (_isFadeOut)
        {
            eventDataType = EventData::EventType::FADE_OUT;
        }
        else
        {
            hideBones();
            eventDataType = EventData::EventType::FADE_IN;
        }
        
        if (_armature->_eventDispatcher->hasEvent(eventDataType))
        {
            EventData *eventData = EventData::borrowObject(eventDataType);
            eventData->armature = _armature;
            eventData->animationState = this;
            _armature->_eventDataList.push_back(eventData);
        }
    }
    
    if (fadeCompleteFlg)
    {
        EventData::EventType eventDataType;
        
        if (_isFadeOut)
        {
            eventDataType = EventData::EventType::FADE_OUT_COMPLETE;
        }
        else
        {
            eventDataType = EventData::EventType::FADE_IN_COMPLETE;
        }
        
        if (_armature->_eventDispatcher->hasEvent(eventDataType))
        {
            EventData *eventData = EventData::borrowObject(eventDataType);
            eventData->armature = _armature;
            eventData->animationState = this;
            _armature->_eventDataList.push_back(eventData);
        }
    }
}

void AnimationState::advanceTimelinesTime(float passedTime)
{
    if (_isPlaying && !_pausePlayheadInFade)
    {
        _time += passedTime;
    }
    
    bool startFlg = false;
    bool completeFlg = false;
    bool loopCompleteFlg = false;
    bool isThisComplete = false;
    int currentPlayTimes = 0;
    int currentTime = (int)(_time * 1000.f);
    
    if (_playTimes == 0)
    {
        isThisComplete = false;
        currentPlayTimes = (int)(ceil(abs(currentTime) / (float)(_totalTime)));
        currentTime -= (int)(floor(currentTime / (float)(_totalTime))) * _totalTime;
        
        if (currentTime < 0)
        {
            currentTime += _totalTime;
        }
    }
    else
    {
        int totalTimes = _playTimes * _totalTime;
        
        if (currentTime >= totalTimes)
        {
            currentTime = totalTimes;
            isThisComplete = true;
        }
        else if (currentTime <= -totalTimes)
        {
            currentTime = -totalTimes;
            isThisComplete = true;
        }
        else
        {
            isThisComplete = false;
        }
        
        if (currentTime < 0)
        {
            currentTime += totalTimes;
        }
        
        currentPlayTimes = (int)(ceil(currentTime / (float)(_totalTime)));
        currentTime -= (int)(floor(currentTime / (float)(_totalTime))) * _totalTime;
        
        if (isThisComplete)
        {
            currentTime = _totalTime;
        }
    }
    
    if (currentPlayTimes == 0)
    {
        currentPlayTimes = 1;
    }
    
    // update timeline
    _isComplete = isThisComplete;
    float progress = _time * 1000.f / (float)(_totalTime);
    
    for (size_t i = 0, l = _timelineStateList.size(); i < l; ++i)
    {
        _timelineStateList[i]->update(progress);
        _isComplete = _timelineStateList[i]->_isComplete && _isComplete;
    }
    
    // update main timeline
    if (_currentTime != currentTime)
    {
        if (_currentPlayTimes != currentPlayTimes)    // check loop complete
        {
            if (_currentPlayTimes > 0 && currentPlayTimes > 1)
            {
                loopCompleteFlg = true;
            }
            
            _currentPlayTimes = currentPlayTimes;
        }
        
        if (_currentTime < 0 && !_pausePlayheadInFade)    // check start
        {
            startFlg = true;
        }
        
        if (_isComplete)    // check complete
        {
            completeFlg = true;
        }
        
        _currentTime = currentTime;
        updateMainTimeline(isThisComplete);
    }
    
    if (startFlg)
    {
        if (_armature->_eventDispatcher->hasEvent(EventData::EventType::START))
        {
            EventData *eventData = EventData::borrowObject(EventData::EventType::START);
            eventData->armature = _armature;
            eventData->animationState = this;
            _armature->_eventDataList.push_back(eventData);
        }
    }
    
    if (completeFlg)
    {
        if (_armature->_eventDispatcher->hasEvent(EventData::EventType::COMPLETE))
        {
            EventData *eventData = EventData::borrowObject(EventData::EventType::COMPLETE);
            eventData->armature = _armature;
            eventData->animationState = this;
            _armature->_eventDataList.push_back(eventData);
        }
        
        if (autoFadeOut)
        {
            fadeOut(fadeOutTime, true);
        }
    }
    else if (loopCompleteFlg)
    {
        if (_armature->_eventDispatcher->hasEvent(EventData::EventType::LOOP_COMPLETE))
        {
            EventData *eventData = EventData::borrowObject(EventData::EventType::LOOP_COMPLETE);
            eventData->armature = _armature;
            eventData->animationState = this;
            _armature->_eventDataList.push_back(eventData);
        }
    }
}

void AnimationState::updateMainTimeline(bool isThisComplete)
{
    if (!_clip->frameList.empty())
    {
        Frame *prevFrame = nullptr;
        Frame *currentFrame = nullptr;
        
        for (size_t i = 0, l = _clip->frameList.size(); i < l; ++i)
        {
            if (_currentFrameIndex < 0)
            {
                _currentFrameIndex = 0;
            }
            else if (_currentTime < _currentFramePosition || _currentTime >= _currentFramePosition + _currentFrameDuration)
            {
                ++_currentFrameIndex;
                
                if (_currentFrameIndex >= (int)(l))
                {
                    if (isThisComplete)
                    {
                        --_currentFrameIndex;
                        break;
                    }
                    else
                    {
                        _currentFrameIndex = 0;
                    }
                }
            }
            else
            {
                break;
            }
            
            currentFrame = _clip->frameList[_currentFrameIndex];
            
            if (prevFrame)
            {
                _armature->arriveAtFrame(prevFrame, this, true);
            }
            
            _currentFrameDuration = currentFrame->duration;
            _currentFramePosition = currentFrame->position;
            prevFrame = currentFrame;
        }
        
        if (currentFrame)
        {
            _armature->arriveAtFrame(currentFrame, this, false);
        }
    }
}

void AnimationState::hideBones()
{
    for (size_t i = 0, l = _clip->hideTimelineList.size(); i < l; ++i)
    {
        Bone *bone = _armature->getBone(_clip->hideTimelineList[i]);
        
        if (bone)
        {
            bone->hideSlots();
        }
    }
}

void AnimationState::clear()
{
    // reverse delete
    for (size_t i = _timelineStateList.size(); i--;)
    {
        TimelineState::returnObject(_timelineStateList[i]);
    }
    
    _timelineStateList.clear();
    _mixingTransforms.clear();
    _armature = nullptr;
    _clip = nullptr;
}
NAME_SPACE_DRAGON_BONES_END