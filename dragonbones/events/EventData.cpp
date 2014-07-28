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

EventData::EventDataType EventData::getType() const
{
    return _type;
}

const String &EventData::getStringType() const
{
    switch (_type)
    {
        case EventDataType::Z_ORDER_UPDATED:
            return Z_ORDER_UPDATED;
        case EventDataType::ANIMATION_FRAME_EVENT:
            return ANIMATION_FRAME_EVENT;
        case EventDataType::BONE_FRAME_EVENT:
            return BONE_FRAME_EVENT;
        case EventDataType::SOUND:
            return SOUND;
        case EventDataType::FADE_IN:
            return FADE_IN;
        case EventDataType::FADE_OUT:
            return FADE_OUT;
        case EventDataType::START:
            return START;
        case EventDataType::COMPLETE:
            return COMPLETE;
        case EventDataType::LOOP_COMPLETE:
            return LOOP_COMPLETE;
        case EventDataType::FADE_IN_COMPLETE:
            return FADE_IN_COMPLETE;
        case EventDataType::FADE_OUT_COMPLETE:
            return FADE_OUT_COMPLETE;
        default:
            break;
    }
    // throw
    return _ERROR;
}

EventData::EventData(EventDataType type, Armature *armatureTarget)
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