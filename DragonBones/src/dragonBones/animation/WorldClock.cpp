#include "WorldClock.h"

DRAGONBONES_NAMESPACE_BEGIN

WorldClock WorldClock::clock;

void WorldClock::advanceTime(float passedTime)
{
    if (passedTime < 0.0f || passedTime != passedTime)
    {
        passedTime = 0.0f;
    }

    if (timeScale != 1.0f)
    {
        passedTime *= timeScale;
    }

    if (passedTime < 0.0f)
    {
        time -= passedTime;
    }
    else
    {
        time += passedTime;
    }

    if (passedTime == 0.0f)
    {
        return;
    }

    std::size_t i = 0, r = 0, l = _animatebles.size();
    for (; i < l; ++i)
    {
        const auto animatable = _animatebles[i];
        if (animatable != nullptr)
        {
            if (r > 0)
            {
                _animatebles[i - r] = animatable;
                _animatebles[i] = nullptr;
            }

            animatable->advanceTime(passedTime);
        }
        else
        {
            r++;
        }
    }

    if (r > 0)
    {
        l = _animatebles.size();
        for (; i < l; ++i)
        {
            const auto animateble = _animatebles[i];
            if (animateble != nullptr)
            {
                _animatebles[i - r] = animateble;
            }
            else
            {
                r++;
            }
        }

        _animatebles.resize(l - r);
    }
}

bool WorldClock::contains(const IAnimatable* value) const
{
    return std::find(_animatebles.cbegin(), _animatebles.cend(), value) != _animatebles.cend();
}

void WorldClock::add(IAnimatable* value)
{
    if (std::find(_animatebles.begin(), _animatebles.end(), value) == _animatebles.end())
    {
        _animatebles.push_back(value);
        value->setClock(this);
    }
}

void WorldClock::remove(IAnimatable* value)
{
    const auto iterator = std::find(_animatebles.begin(), _animatebles.end(), value);
    if (iterator != _animatebles.end())
    {
        *iterator = nullptr;
        value->setClock(nullptr);
    }
}

void WorldClock::clear()
{
    for (const auto animatable : _animatebles)
    {
        if (animatable != nullptr)
        {
            animatable->setClock(nullptr);
        }
    }
}

void WorldClock::setClock(WorldClock* value)
{
    if (_clock == value)
    {
        return;
    }

    if (_clock != nullptr)
    {
        _clock->remove(this);
    }

    _clock = value;

    if (_clock != nullptr)
    {
        _clock->add(this);
    }
}

DRAGONBONES_NAMESPACE_END
