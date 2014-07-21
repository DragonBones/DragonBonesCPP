#include "WorldClock.h"

NAME_SPACE_DRAGON_BONES_BEGIN
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
{
    _time = (time < 0 || time != time) ? std::system("time") * 0.001f : time;
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
    auto iterator = std::find(_animatableList.begin(), _animatableList.end(), animatable);
    
    if (iterator != _animatableList.end())
    {
        _animatableList.erase(iterator);
    }
}

void WorldClock::removeAll()
{
    _animatableList.clear();
}

void WorldClock::advanceTime(float passedTime)
{
    if (passedTime < 0 || passedTime != passedTime)
    {
        passedTime = std::system("time") * 0.001f - _time;
        
        if (passedTime < 0)
        {
            passedTime = 0.f;
        }
    }
    
    passedTime *= _timeScale;
    _time += passedTime;
    
    if (_animatableList.empty())
    {
        return;
    }
    
    size_t currentIndex = 0;
    size_t i = 0;
    
    for (size_t l = _animatableList.size(); i < l; ++i)
    {
        IAnimatable *animatable = _animatableList[i];
        
        if (animatable)
        {
            if (currentIndex != i)
            {
                _animatableList[currentIndex] = animatable;
                _animatableList[i] = nullptr;
            }
            
            animatable->advanceTime(passedTime);
            ++ currentIndex;
        }
    }
    
    if (currentIndex != i)
    {
        for (size_t l = _animatableList.size(); i < l;)
        {
            _animatableList[currentIndex++] = _animatableList[i++];
        }
        
        _animatableList.reserve(currentIndex);
    }
}
NAME_SPACE_DRAGON_BONES_END