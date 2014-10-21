#ifndef OBJECTS_I_AREA_DATA_H
#define OBJECTS_I_AREA_DATA_H

#include "../DragonBones.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class IAreaData
{
public:
    enum class AreaType {AT_ELLIPSE, AT_RECTANGLE};
    
public:
    AreaType areaType;
    std::string name;
    
public:
    IAreaData() {}
    IAreaData(const IAreaData &copyData)
    {
        operator=(copyData);
    }
    IAreaData& operator=(const IAreaData &copyData)
    {
        areaType = copyData.areaType;
        name = copyData.name;
        return *this;
    }
    virtual ~IAreaData()
    {
    }
    virtual void dispose() = 0;
};
NAME_SPACE_DRAGON_BONES_END
#endif  // OBJECTS_I_AREA_DATA_H