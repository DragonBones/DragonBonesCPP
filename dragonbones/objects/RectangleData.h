#ifndef OBJECTS_RECTANGLE_DATA_H
#define OBJECTS_RECTANGLE_DATA_H

#include "../DragonBones.h"
#include "../geoms/Point.h"
#include "../geoms/Transform.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class RectangleData : public IAreaData
{
public:
    float width;
    float height;
    
    Transform transform;
    Point pivot;
    
public:
    RectangleData()
    {
        areaType = IAreaData::AreaType::AT_RECTANGLE;
        width = 0.f;
        height = 0.f;
    }
    RectangleData(const RectangleData &copyData)
    {
        operator=(copyData);
    }
    RectangleData& operator=(const RectangleData &copyData)
    {
        IAreaData::operator=(copyData);
        width = copyData.width;
        height = copyData.height;
        transform = copyData.transform;
        pivot = copyData.pivot;
        return *this;
    }
    virtual ~RectangleData()
    {
        dispose();
    }
    virtual void dispose() {}
};
NAME_SPACE_DRAGON_BONES_END
#endif  // OBJECTS_RECTANGLE_DATA_H
