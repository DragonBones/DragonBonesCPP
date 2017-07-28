#ifndef DRAGONBONES_RECTANGLE_H
#define DRAGONBONES_RECTANGLE_H

#include "../core/DragonBones.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
 * @private
 */
class Rectangle
{
public:
    float x;
    float y;
    float width;
    float height;

    Rectangle():
        x(0.0f),
        y(0.0f),
        width(0.0f),
        height(0.0f)
    {}
    Rectangle(const Rectangle& value)
    {
        operator=(value);
    }
    ~Rectangle() {}

    inline void operator=(const Rectangle& value)
    {
        x = value.x;
        y = value.y;
        width = value.width;
        height = value.height;
    }

    void clear()
    {
        x = y = 0.0f;
        width = height = 0.0f;
    }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_RECTANGLE_H
