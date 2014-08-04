#include "EventData.h"

NAME_SPACE_DRAGON_BONES_BEGIN

const String EventData::Z_ORDER_UPDATED = "zorderUpdate";
const String EventData::ANIMATION_FRAME_EVENT = "animationFrameEvent";
const String EventData::BONE_FRAME_EVENT = "boneFrameEvent";
const String EventData::SOUND = "sound";
const String EventData::FADE_IN = "fadeIn";
const String EventData::FADE_OUT = "fadeOut";
const String EventData::START = "start";
const String EventData::COMPLETE = "complete";
const String EventData::LOOP_COMPLETE = "loopComplete";
const String EventData::FADE_IN_COMPLETE = "fadeInComplete";
const String EventData::FADE_OUT_COMPLETE = "fadeOutComplete";

const String EventData::_ERROR = "error";

const String &EventData::typeToString(EventData::EventType eventType)
{
    switch (eventType)
    {
        case EventType::Z_ORDER_UPDATED:
            return Z_ORDER_UPDATED;
            
        case EventType::ANIMATION_FRAME_EVENT:
            return ANIMATION_FRAME_EVENT;
            
        case EventType::BONE_FRAME_EVENT:
            return BONE_FRAME_EVENT;
            
        case EventType::SOUND:
            return SOUND;
            
        case EventType::FADE_IN:
            return FADE_IN;
            
        case EventType::FADE_OUT:
            return FADE_OUT;
            
        case EventType::START:
            return START;
            
        case EventType::COMPLETE:
            return COMPLETE;
            
        case EventType::LOOP_COMPLETE:
            return LOOP_COMPLETE;
            
        case EventType::FADE_IN_COMPLETE:
            return FADE_IN_COMPLETE;
            
        case EventType::FADE_OUT_COMPLETE:
            return FADE_OUT_COMPLETE;
            
        default:
            break;
    }
    
    // throw
    return _ERROR;
}

EventData::EventType EventData::getType() const
{
    return _type;
}

const String &EventData::getStringType() const
{
    return typeToString(_type);
}

EventData::EventData(EventType type, Armature *armatureTarget)
{
    _type = type;
    armature = armatureTarget;
    bone = nullptr;
    animationState = nullptr;
}
EventData::~EventData()
{
    dispose();
}
void EventData::dispose()
{
    armature = nullptr;
    bone = nullptr;
    animationState = nullptr;
}

NAME_SPACE_DRAGON_BONES_END