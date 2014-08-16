#ifndef __OBJECTS_TRANSFORM_TIMELINE_H__
#define __OBJECTS_TRANSFORM_TIMELINE_H__

#include "../DragonBones.h"
#include "../geoms/Point.h"
#include "../geoms/Transform.h"
#include "Timeline.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class TransformTimeline : public Timeline
{
public:
    bool transformed;
    float offset;
    
    String name;
    Transform originTransform;
    Point originPivot;
    
public:
    TransformTimeline()
    {
        offset = 0.f;
        transformed = false;
    }
    TransformTimeline(const TransformTimeline &copyData)
    {
        operator=(copyData);
    }
    TransformTimeline& operator=(const TransformTimeline &copyData)
    {
        Timeline::operator=(copyData);
        //_dispose();
        transformed = copyData.transformed;
        offset = copyData.offset;
        name = copyData.name;
        originTransform = copyData.originTransform;
        originPivot = copyData.originPivot;
        return *this;
    }
    virtual ~TransformTimeline()
    {
        dispose();
    }
    
    virtual void dispose()
    {
        Timeline::dispose();
        //_dispose();
    }
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __OBJECTS_TRANSFORM_TIMELINE_H__
