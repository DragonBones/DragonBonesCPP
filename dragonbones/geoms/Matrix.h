#ifndef __GEOMS_MATRIX_H__
#define __GEOMS_MATRIX_H__

#include "../DragonBones.h"

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
        float a0 = a;
        float b0 = b;
        float c0 = c;
        float d0 = d;
        float tx0 = tx;
        float ty0 = ty;
        float determinant = 1 / (a0 * d0 - b0 * c0);
        a = determinant * d0;
        b = -determinant * b0;
        c = -determinant * c0;
        d = determinant * a0;
        tx = determinant * (c0 * ty0 - d0 * tx0);
        ty = determinant * (b0 * tx0 - a0 * ty0);
    }
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __GEOMS_MATRIX_H__