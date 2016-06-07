#ifndef DRAGONBONES_ARMATURE_DISPLAY_CONTAINER_H
#define DRAGONBONES_ARMATURE_DISPLAY_CONTAINER_H

#include "../core/DragonBones.h"
#include "../events/IEventDispatcher.h"

DRAGONBONES_NAMESPACE_BEGIN

class Armature;
class Animation;

class IArmatureDisplayContainer : public IEventDispatcher
{
public:
    IArmatureDisplayContainer() {};
    virtual ~IArmatureDisplayContainer() {};

    virtual void dispose() = 0;
    virtual void advanceTimeSelf(bool on) = 0;
    virtual Armature& getArmature() const = 0;
    virtual Animation& getAnimation() const = 0;
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES__ARMATURE_DISPLAY_CONTAINER_H