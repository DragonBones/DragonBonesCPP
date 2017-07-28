#ifndef DRAGONBONES_TRANSFORM_H
#define DRAGONBONES_TRANSFORM_H

#include "../core/DragonBones.h"
#include "Matrix.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* 2D 变换。
* @version DragonBones 3.0
* @language zh_CN
*/
class Transform final
{
public:
    static const float PI;
    static const float PI_D;
    static const float PI_H;
    static const float PI_Q;
    static const float DEG_RAD;
    static const float RAD_DEG;

    static float normalizeRadian(float value)
    {
        value = fmod(value + Transform::PI, Transform::PI * 2.0f);
        value += value > 0.0f ? -Transform::PI : Transform::PI;

        return value;
    }

public:
    /**
    * 水平位移。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    float x;
    /**
    * 垂直位移。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    float y;
    /**
    * 倾斜。 (以弧度为单位)
    * @version DragonBones 3.0
    * @language zh_CN
    */
    float skew;
    /**
    * 旋转。 (以弧度为单位)
    * @version DragonBones 3.0
    * @language zh_CN
    */
    float rotation;
    /**
    * 水平缩放。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    float scaleX;
    /**
    * 垂直缩放。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    float scaleY;

    Transform():
        x(0.0f),
        y(0.0f),
        skew(0.0f),
        rotation(0.0f),
        scaleX(1.0f),
        scaleY(1.0f)
    {}
    Transform(const Transform& value)
    {
        operator=(value);
    }
    ~Transform() {}

    inline void operator=(const Transform& value)
    {
        x = value.x;
        y = value.y;
        skew = value.skew;
        rotation = value.rotation;
        scaleX = value.scaleX;
        scaleY = value.scaleY;
    }
    /**
    * @private
    */
    inline Transform& identity()
    {
        x = y = skew = rotation = 0.0f;
        scaleX = scaleY = 1.0f;

        return *this;
    }
    /**
    * @private
    */
    inline Transform& add(const Transform& value)
    {
        x += value.x;
        y += value.y;
        skew += value.skew;
        rotation += value.rotation;
        scaleX *= value.scaleX;
        scaleY *= value.scaleY;

        return *this;
    }
    /**
    * @private
    */
    inline Transform& minus(const Transform& value)
    {
        x -= value.x;
        y -= value.y;
        skew -= value.skew;
        rotation -= value.rotation;
        scaleX /= value.scaleX;
        scaleY /= value.scaleY;

        return *this;
    }
    /**
    * 矩阵转换为变换。
    * @param matrix 矩阵。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline Transform& fromMatrix(const Matrix& matrix)
    {
        const auto backupScaleX = scaleX, backupScaleY = scaleY;

        x = matrix.tx;
        y = matrix.ty;

        rotation = std::atan(matrix.b / matrix.a);
        auto skewX = std::atan(-matrix.c / matrix.d);

        scaleX = (rotation > -PI_Q && rotation < PI_Q) ? matrix.a / std::cos(rotation) : matrix.b / std::sin(rotation);
        scaleY = (skewX > -PI_Q && skewX < PI_Q) ? matrix.d / std::cos(skewX) : -matrix.c / std::sin(skewX);

        if (backupScaleX >= 0.0f && scaleX < 0.0f) 
        {
            scaleX = -scaleX;
            rotation = rotation - PI;
        }

        if (backupScaleY >= 0.0f && scaleY < 0.0f) 
        {
            scaleY = -scaleY;
            skewX = skewX - PI;
        }

        skew = skewX - rotation;

        return *this;
    }
    /**
    * 转换为矩阵。
    * @param matrix 矩阵。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline Transform& toMatrix(Matrix& matrix)
    {
        if (skew != 0.0f || rotation != 0.0f) 
        {
            matrix.a = std::cos(rotation);
            matrix.b = std::sin(rotation);

            if (skew == 0.0f) 
            {
                matrix.c = -matrix.b;
                matrix.d = matrix.a;
            }
            else 
            {
                matrix.c = -std::sin(skew + rotation);
                matrix.d = std::cos(skew + rotation);
            }

            if (scaleX != 1.0f) 
            {
                matrix.a *= scaleX;
                matrix.b *= scaleX;
            }

            if (scaleY != 1.0f) 
            {
                matrix.c *= scaleY;
                matrix.d *= scaleY;
            }
        }
        else 
        {
            matrix.a = scaleX;
            matrix.b = 0.0f;
            matrix.c = 0.0f;
            matrix.d = scaleY;
        }

        matrix.tx = x;
        matrix.ty = y;

        return *this;
    }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_TRANSFORM_H
