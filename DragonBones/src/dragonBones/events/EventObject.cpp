#include "EventObject.h"

DRAGONBONES_NAMESPACE_BEGIN

const char* EventObject::START = "start";
const char* EventObject::LOOP_COMPLETE = "loopComplete";
const char* EventObject::COMPLETE = "complete";
const char* EventObject::FADE_IN = "fadeIn";
const char* EventObject::FADE_IN_COMPLETE = "fadeInComplete";
const char* EventObject::FADE_OUT = "fadeOut";
const char* EventObject::FADE_OUT_COMPLETE = "fadeOutComplete";
const char* EventObject::FRAME_EVENT = "frameEvent";
const char* EventObject::SOUND_EVENT = "soundEvent";

void EventObject::_onClear()
{
    time = 0.0f;
    type = "";
    name = "";
    data = nullptr;
    armature = nullptr;
    bone = nullptr;
    slot = nullptr;
    animationState = nullptr;
}

void EventObject::copyFrom(const EventObject& value)
{
    time = value.time;
    type = value.type;
    name = value.name;
    data = value.data;
    armature = value.armature;
    bone = value.bone;
    slot = value.slot;
    animationState = value.animationState;
}

DRAGONBONES_NAMESPACE_END
