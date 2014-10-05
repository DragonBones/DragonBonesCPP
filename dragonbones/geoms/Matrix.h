#ifndef GEOMS_MATRIX_H
#define GEOMS_MATRIX_H

#include "../DragonBones.h"
#include "Point.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class Matrix
{
public:
    float a;
    float b;
    float c;
    float d;
    float tx;
    float ty;
    
public:
    Matrix()
    {
        a = 0.f;
        b = 0.f;
        c = 0.f;
        d = 0.f;
        tx = 0.f;
        ty = 0.f;
    }
    Matrix(const Matrix &copyData)
    {
        operator=(copyData);
    }
    void operator=(const Matrix &copyData)
    {
        a = copyData.a;
        b = copyData.b;
        c = copyData.c;
        d = copyData.d;
        tx = copyData.tx;
        ty = copyData.ty;
    }
    virtual ~Matrix() {}
    
    void invert()
    {
        const float a0 = a;
        const float b0 = b;
        const float c0 = c;
        const float d0 = d;
        const float tx0 = tx;
        const float ty0 = ty;
        const float determinant = 1 / (a0 * d0 - b0 * c0);
        a = determinant * d0;
        b = -determinant * b0;
        c = -determinant * c0;
        d = determinant * a0;
        tx = determinant * (c0 * ty0 - d0 * tx0);
        ty = determinant * (b0 * tx0 - a0 * ty0);
    }
    
    void transformPoint(Point &point)
    {
        float x = point.x;
        float y = point.y;
        point.x = a * x + c * y + tx;
        point.y = d * y + b * x + ty;
    }
};
NAME_SPACE_DRAGON_BONES_END
#endif  // GEOMS_MATRIX_H