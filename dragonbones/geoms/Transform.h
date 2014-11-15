#ifndef GEOMS_TRANSFORM_H
#define GEOMS_TRANSFORM_H

#include "../DragonBones.h"
#include "Matrix.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class Transform
{
public:
    float x;
    float y;
    float skewX;
    float skewY;
    float scaleX;
    float scaleY;
    
public:
    float getRotation()
    {
        return skewX;
    }
    void setRotation(float value)
    {
        skewX = skewY = value;
    }
    
public:
    Transform()
    {
        x = 0.f;
        y = 0.f;
        skewX = 0.f;
        skewY = 0.f;
        scaleX = 1.f;
        scaleY = 1.f;
    }
    Transform(const Transform &copyData)
    {
        operator=(copyData);
    }
    Transform& operator=(const Transform &copyData)
    {
        x = copyData.x;
        y = copyData.y;
        skewX = copyData.skewX;
        skewY = copyData.skewY;
        scaleX = copyData.scaleX;
        scaleY = copyData.scaleY;
        return *this;
    }
    virtual ~Transform() {}
    
    void toMatrix(Matrix &matrix, bool keepScale = false) const
    {
        if (keepScale)
        {
            matrix.a = scaleX * cos(skewY);
            matrix.b = scaleX * sin(skewY);
            matrix.c = -scaleY * sin(skewX);
            matrix.d = scaleY * cos(skewX);
        }
        else
        {
            matrix.a = cos(skewY);
            matrix.b = sin(skewY);
            matrix.c = -sin(skewX);
            matrix.d = cos(skewX);
        }
        
        matrix.tx = x;
        matrix.ty = y;
    }
    
    void transformWith(const Transform &parent)
    {
        Matrix matrix;
        parent.toMatrix(matrix, true);
        matrix.invert();
        const float x0 = x;
        const float y0 = y;
        x = matrix.a * x0 + matrix.c * y0 + matrix.tx;
        y = matrix.d * y0 + matrix.b * x0 + matrix.ty;
        skewX = formatRadian(skewX - parent.skewX);
        skewY = formatRadian(skewY - parent.skewY);
    }
};
NAME_SPACE_DRAGON_BONES_END
#endif  // GEOMS_TRANSFORM_H
