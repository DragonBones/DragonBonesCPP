#ifndef DRAGONBONES_POINT_H
#define DRAGONBONES_POINT_H

#include "../core/DragonBones.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
 * @private
 */
class Point
{
public:
    static Point helpPointA;
    static Point helpPointB;
    static Point helpPointC;
    static Point helpPointD;

public:
    float x;
    float y;

    Point():
        x(0.0f),
        y(0.0f)
    {}
    Point(const Point& value)
    {
        operator=(value);
    }
    ~Point() {}

    inline void operator=(const Point& value)
    {
        x = value.x;
        y = value.y;
    }

    inline void clear()
    {
        x = y = 0.0f;
    }

public: // For WebAssembly.
    static Point* getHelpPointA() { return &helpPointA; }
    static Point* getHelpPointB() { return &helpPointB; }
    static Point* getHelpPointC() { return &helpPointC; }
    static Point* getHelpPointD() { return &helpPointD; }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_POINT_H
