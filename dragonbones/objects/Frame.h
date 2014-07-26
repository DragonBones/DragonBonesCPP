#ifndef __OBJECTS_FRAME_H__
#define __OBJECTS_FRAME_H__

#include "../DragonBones.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class Frame
{
public:
    enum class FrameType {FT_FRAME, FT_TRANSFORM_FRAME};
    
public:
    int position;
    int duration;
    
    FrameType frameType;
    String action;
    String event;
    String sound;
    
public:
    Frame()
    {
        position = 0;
        duration = 0;
        frameType = FrameType::FT_FRAME;
    }
    Frame(const Frame &copyData)
    {
        operator=(copyData);
    }
    Frame &operator=(const Frame &copyData)
    {
        dispose();
        position = copyData.position;
        duration = copyData.duration;
        action = copyData.action;
        event = copyData.event;
        sound = copyData.sound;
        return *this;
    }
    virtual ~Frame()
    {
        dispose();
    }
    virtual void dispose() {}
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __OBJECTS_FRAME_H__
