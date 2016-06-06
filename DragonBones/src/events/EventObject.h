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
    static const char* START;
    static const char* LOOP_COMPLETE;
    static const char* COMPLETE;

    static const char* FADE_IN;
    static const char* FADE_IN_COMPLETE;
    static const char* FADE_OUT;
    static const char* FADE_OUT_COMPLETE;

    static const char* FRAME_EVENT;
    static const char* SOUND_EVENT;

public:
    std::string type;
    std::string name;
    //void* data; // TODO
    void* userData;
    Armature* armature;
    Bone* bone;
    Slot* slot;
    AnimationState* animationState;

public:
    EventObject();
    ~EventObject();

protected:
    void _onClear() override;

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(EventObject);
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_EVENT_OBJECT_H
