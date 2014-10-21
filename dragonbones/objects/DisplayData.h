#ifndef OBJECTS_DISPLAY_DATA_H
#define OBJECTS_DISPLAY_DATA_H

#include "../DragonBones.h"
#include "../geoms/Point.h"
#include "../geoms/Transform.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class DisplayData
{
public:
    std::string name;
    
    DisplayType type;
    Transform transform;
    Point pivot;
    
public:
    DisplayData()
    {
        type = DisplayType::DT_IMAGE;
    }
    DisplayData(const DisplayData &copyData)
    {
        operator=(copyData);
    }
    DisplayData& operator=(const DisplayData &copyData)
    {
        name = copyData.name;
        type = copyData.type;
        transform = copyData.transform;
        pivot = copyData.pivot;
        return *this;
    }
    virtual ~DisplayData() {}
};
NAME_SPACE_DRAGON_BONES_END
#endif  // OBJECTS_DISPLAY_DATA_H
