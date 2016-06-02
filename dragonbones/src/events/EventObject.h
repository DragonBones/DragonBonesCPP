#ifndef DRAGONBONES_EVENT_OBJECT_H
#define DRAGONBONES_EVENT_OBJECT_H

#include "../core/BaseObject.h"

NAMESPACE_DRAGONBONES_BEGIN

class Armature;
class Bone;
class Slot;
class AnimationState;

class EventObject final : public BaseObject
{
    BIND_CLASS_TYPE(EventObject);

public:
    std::string type;
    std::string name;
    //void* data;
    void* userData;
    Armature* armature;
    Bone* bone;
    Slot* slot;
    AnimationState* animationState;

public:
    EventObject()
    {
        _onClear();
    }
    ~EventObject()
    {
        _onClear();
    }

protected:
    void _onClear() override
    {
        type.clear();
        name.clear();
        //data = null;
        userData = nullptr;
        armature = nullptr;
        bone = nullptr;
        slot = nullptr;
        animationState = nullptr;
    }

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(EventObject);
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_EVENT_OBJECT_H
