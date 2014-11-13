#ifndef ANIMATION_WORLD_CLOCK_H
#define ANIMATION_WORLD_CLOCK_H

#include "../DragonBones.h"
#include "IAnimatable.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class WorldClock : public IAnimatable
{
public:
    static WorldClock clock;

private:
    bool _dirty;
    bool _isPlaying;
    float _time;
    float _timeScale;



    std::vector<IAnimatable*> _animatableList;

public:
    static WorldClock* getInstance();

    float getTime() const;

    float getTimeScale() const;
    void setTimeScale(float timeScale);

public:
    WorldClock(float time = -1, float timeScale = 1);
    virtual ~WorldClock();

    virtual void dispose();

    virtual bool contains(const IAnimatable *animatable) const;
    virtual void add(IAnimatable *animatable);
    virtual void remove(IAnimatable *animatable);
    virtual void removeAll();
    virtual void play();
    virtual void stop();
    virtual void advanceTime(float passedTime) override;

private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(WorldClock);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // ANIMATION_WORLD_CLOCK_H