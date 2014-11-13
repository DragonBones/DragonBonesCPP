#include "WorldClock.h"

NAME_SPACE_DRAGON_BONES_BEGIN

WorldClock WorldClock::clock;

WorldClock* WorldClock::getInstance()
{
    return &clock;
}

float WorldClock::getTime() const
{
    return _time;
}

float WorldClock::getTimeScale() const
{
    return _timeScale;
}
void WorldClock::setTimeScale(float timeScale)
{
    if (timeScale < 0 || timeScale != timeScale)
    {
        timeScale = 1.f;
    }
    
    _timeScale = timeScale;
}

WorldClock::WorldClock(float time, float timeScale)
    : _dirty(false)
    , _isPlaying(true)
{
    // _time = (time < 0 || time != time) ? getTimer() * 0.001f : time;
    _time = 0;
    setTimeScale(timeScale);
}
WorldClock::~WorldClock()
{
    dispose();
}
void WorldClock::dispose()
{
    _animatableList.clear();
}

bool WorldClock::contains(const IAnimatable *animatable) const
{
    auto iterator = std::find(_animatableList.cbegin(), _animatableList.cend(), animatable);
    return iterator != _animatableList.cend();
}

void WorldClock::add(IAnimatable *animatable)
{
    if (animatable && !contains(animatable))
    {
        _animatableList.push_back(animatable);
    }
}

void WorldClock::remove(IAnimatable *animatable)
{
    if (!animatable) { return; }
    
    auto iterator = std::find(_animatableList.begin(), _animatableList.end(), animatable);
    
    if (iterator != _animatableList.end())
    {
        _animatableList[iterator - _animatableList.begin()] = nullptr;
        _dirty = true;
    }
}

void WorldClock::removeAll()
{
    _animatableList.clear();
}

void WorldClock::play()
{
    _isPlaying = true;
}

void WorldClock::stop()
{
    _isPlaying = false;
}

void WorldClock::advanceTime(float passedTime)
{
    if (!_isPlaying)
    {
        return;
    }
    
    if (passedTime < 0 || passedTime != passedTime)
    {
        /*
        passedTime = getTimer() * 0.001f - _time;
        if (passedTime < 0)
        {
            passedTime = 0.f;
        }
        */
        passedTime = 0.f;
    }
    
    passedTime *= _timeScale;
    _time += passedTime;
    
    if (_animatableList.empty())
    {
        return;
    }
    
    for (size_t i = 0, l = _animatableList.size(); i < l; ++i)
    {
        if (_animatableList[i])
        {
            _animatableList[i]->advanceTime(passedTime);
        }
    }
    
    if (_dirty)
    {
        size_t curIdx = 0;
        
        for (size_t i = 0, l = _animatableList.size(); i < l; ++i)
        {
            if (_animatableList[i])
            {
                if (curIdx != i)
                {
                    _animatableList[curIdx] = _animatableList[i];
                    _animatableList[i] = nullptr;
                }
                
                curIdx++;
            }
        }
        
        _animatableList.resize(curIdx);
        _dirty = false;
    }
}
NAME_SPACE_DRAGON_BONES_END