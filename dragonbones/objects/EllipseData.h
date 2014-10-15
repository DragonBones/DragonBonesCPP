#ifndef OBJECTS_ELLIPSE_DATA_H
#define OBJECTS_ELLIPSE_DATA_H

#include "../DragonBones.h"
#include "../geoms/Point.h"
#include "../geoms/Transform.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class EllipseData : public IAreaData
{
public:
    float width;
    float height;
    
    Transform transform;
    Point pivot;
    
public:
    EllipseData()
    {
        areaType = IAreaData::AreaType::AT_ELLIPSE;
        width = 0.f;
        height = 0.f;
    }
    EllipseData(const EllipseData &copyData)
    {
        operator=(copyData);
    }
    EllipseData& operator=(const EllipseData &copyData)
    {
        IAreaData::operator=(copyData);
        dispose();
        width = copyData.width;
        height = copyData.height;
        transform = copyData.transform;
        pivot = copyData.pivot;
        return *this;
    }
    virtual ~EllipseData()
    {
        dispose();
    }
    virtual void dispose() {}
};
NAME_SPACE_DRAGON_BONES_END
#endif  // OBJECTS_ELLIPSE_DATA_H
