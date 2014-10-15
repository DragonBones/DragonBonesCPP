#ifndef OBJECTS_TIMELINE_H
#define OBJECTS_TIMELINE_H

#include "../DragonBones.h"
#include "Frame.h"
#include "TransformFrame.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class Timeline
{
public:
    int duration;
    float scale;
    std::vector<Frame*> frameList;
    
public:
    Timeline()
    {
        duration = 0;
        scale = 1.f;
    }
    Timeline(const Timeline &copyData)
    {
        operator=(copyData);
    }
    Timeline& operator=(const Timeline &copyData)
    {
        dispose();
        duration = copyData.duration;
        scale = copyData.scale;
        frameList.reserve(copyData.frameList.size());
        
        for (size_t i = 0, l = frameList.size(); i < l; ++i)
        {
            switch (copyData.frameList[i]->frameType)
            {
                case Frame::FrameType::FT_FRAME:
                    frameList.push_back(new Frame());
                    *(frameList[i]) = *(static_cast<Frame*>(copyData.frameList[i]));
                    break;
                    
                case Frame::FrameType::FT_TRANSFORM_FRAME:
                    frameList.push_back(new TransformFrame());
                    *(frameList[i]) = *(static_cast<TransformFrame*>(copyData.frameList[i]));
                    break;
                    
                default:
                    // throw
                    break;
            }
        }
        
        return *this;
    }
    virtual ~Timeline()
    {
        dispose();
    }
    virtual void dispose()
    {
        for (size_t i = 0, l = frameList.size(); i < l; ++i)
        {
            frameList[i]->dispose();
            delete frameList[i];
        }
        
        frameList.clear();
    }
};
NAME_SPACE_DRAGON_BONES_END
#endif  // OBJECTS_TIMELINE_H
