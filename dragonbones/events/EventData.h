#ifndef __EVENTS_EVENT_DATA_H__
#define __EVENTS_EVENT_DATA_H__

#include "../DragonBones.h"

NAME_SPACE_DRAGON_BONES_BEGIN

class Armature;
class Bone;
class AnimationState;

class EventData
{
public:
    static enum class EventDataType
    {
        Z_ORDER_UPDATED,
        ANIMATION_FRAME_EVENT,
        BONE_FRAME_EVENT,
        SOUND,
        FADE_IN, FADE_OUT, START, COMPLETE, LOOP_COMPLETE, FADE_IN_COMPLETE, FADE_OUT_COMPLETE
    };
    
    static String getStringType(const EventDataType &type)
    {
        switch (type)
        {
            case EventDataType::Z_ORDER_UPDATED:
                return "zOrderUpdate";
            case EventDataType::ANIMATION_FRAME_EVENT:
                return "animationFrameEvent";
            case EventDataType::BONE_FRAME_EVENT:
                return "boneFrameEvent";
            case EventDataType::SOUND:
                return "sound";
            case EventDataType::FADE_IN:
                return "fadeIn";
            case EventDataType::FADE_OUT:
                return "fadeOut";
            case EventDataType::START:
                return "start";
            case EventDataType::COMPLETE:
                return "complete";
            case EventDataType::LOOP_COMPLETE:
                return "loopComplete";
            case EventDataType::FADE_IN_COMPLETE:
                return "fadeInComplete";
            case EventDataType::FADE_OUT_COMPLETE:
                return "fadeOutComplete";
            default:
                // throw
                break;
        }
        return "";
    }
    
public:
    String frameLabel;
    String sound;
    
    Armature *armature;
    Bone *bone;
    AnimationState *animationState;
    
private:
    EventDataType _type;
    
public:
    const EventDataType &getType() const
    {
        return _type;
    }
    
public:
    EventData(const EventDataType &type, Armature *armatureTarget)
    {
        _type = type;
        armature = armatureTarget;
        bone = nullptr;
        animationState = nullptr;
    }
    virtual ~EventData()
    {
        dispose();
    }
    void dispose()
    {
        armature = nullptr;
        bone = nullptr;
        animationState = nullptr;
    }
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(EventData);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __EVENTS_EVENT_DATA_H__
