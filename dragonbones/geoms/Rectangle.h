#ifndef GEOMS_RECTANGLE_H
#define GEOMS_RECTANGLE_H

#include "../DragonBones.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class Rectangle
{
public:
    float x;
    float y;
    float width;
    float height;
    
public:
    Rectangle()
        : x(0)
        , y(0)
        , width(0)
        , height(0)
    {}
    Rectangle(float x, float y, float w, float h)
        : x(x)
        , y(y)
        , width(w)
        , height(h)
    {}
    Rectangle(const Rectangle &copyData)
    {
        operator=(copyData);
    }
    Rectangle& operator=(const Rectangle &copyData)
    {
        x = copyData.x;
        y = copyData.y;
        width = copyData.width;
        height = copyData.height;
        return *this;
    }
    virtual ~Rectangle() {}
};
NAME_SPACE_DRAGON_BONES_END
#endif  // GEOMS_RECTANGLE_H
