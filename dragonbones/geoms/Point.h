#ifndef __GEOMS_POINT_H__
#define __GEOMS_POINT_H__

#include "../DragonBones.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class Point
{
public:
    float x;
    float y;
    
public:
    Point()
    {
        x = 0.f;
        y = 0.f;
    }
    Point(const Point &copyData)
    {
        operator=(copyData);
    }
    void operator=(const Point &copyData)
    {
        x = copyData.x;
        y = copyData.y;
    }
    virtual ~Point() {}
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __GEOMS_POINT_H__
