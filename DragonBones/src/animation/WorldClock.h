#ifndef DRAGONBONES_WORLD_CLOCK_H
#define DRAGONBONES_WORLD_CLOCK_H

#include "../core/DragonBones.h"
#include "IAnimateble.h"

NAMESPACE_DRAGONBONES_BEGIN
class WorldClock final : public IAnimateble
{
public:
    static WorldClock clock;

public:
    float time;
    float timeScale;

private:
    std::vector<IAnimateble*> _animatebles;

public:
    WorldClock();
    ~WorldClock();

    virtual void advanceTime(float passedTime) override;
    bool contains(const IAnimateble* value) const;
    void add(IAnimateble* value);
    void remove(IAnimateble* value);
    void clear();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(WorldClock);
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_WORLD_CLOCK_H