#ifndef DRAGONBONES_ANIMATEBLE_H
#define DRAGONBONES_ANIMATEBLE_H

#include "../core/DragonBones.h"

NAMESPACE_DRAGONBONES_BEGIN

class IAnimateble
{
public:
    IAnimateble() {}
    virtual ~IAnimateble() {}

    virtual void advanceTime(float passedTime) = 0;
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_ANIMATEBLE_H