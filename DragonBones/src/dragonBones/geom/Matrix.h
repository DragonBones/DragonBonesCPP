#ifndef DRAGONBONES_MATRIX_H
#define DRAGONBONES_MATRIX_H

#include "../core/DragonBones.h"
#include "Point.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* 2D 矩阵。
* @version DragonBones 3.0
* @language zh_CN
*/
class Matrix
{
public:
    float a;
    float b;
    float c;
    float d;
    float tx;
    float ty;

    Matrix():
        a(1.0f),
        b(0.0f),
        c(0.0f),
        d(1.0f),
        tx(0.0f),
        ty(0.0f)
    {}
    Matrix(const Matrix& value)
    {
        operator=(value);
    }
    ~Matrix() {}

    inline void operator=(const Matrix& value)
    {
        a = value.a;
        b = value.b;
        c = value.c;
        d = value.d;
        tx = value.tx;
        ty = value.ty;
    }
    /**
    * 转换为单位矩阵。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline void identity()
    {
        a = d = 1.0f;
        b = c = 0.0f;
        tx = ty = 0.0f;
    }
    /**
    * 将当前矩阵与另一个矩阵相乘。
    * @param value 需要相乘的矩阵。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline void concat(const Matrix& value)
    {
        const auto aA = a;
        const auto bA = b;
        const auto cA = c;
        const auto dA = d;
        const auto txA = tx;
        const auto tyA = ty;
        const auto aB = value.a;
        const auto bB = value.b;
        const auto cB = value.c;
        const auto dB = value.d;
        const auto txB = value.tx;
        const auto tyB = value.ty;

        a = aA * aB + bA * cB;
        b = aA * bB + bA * dB;
        c = cA * aB + dA * cB;
        d = cA * bB + dA * dB;
        tx = aB * txA + cB * tyA + txB;
        ty = dB * tyA + bB * txA + tyB;
    }
    /**
    * 转换为逆矩阵。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline void invert() 
    {
        const auto aA = a;
        const auto bA = b;
        const auto cA = c;
        const auto dA = d;
        const auto txA = tx;
        const auto tyA = ty;
        const auto n = aA * dA - bA * cA;

        a = dA / n;
        b = -bA / n;
        c = -cA / n;
        d = aA / n;
        tx = (cA * tyA - dA * txA) / n;
        ty = -(aA * tyA - bA * txA) / n;
    }
    /**
    * 将矩阵转换应用于指定点。
    * @param x 横坐标。
    * @param y 纵坐标。
    * @param result 应用转换之后的坐标。
    * @params delta 是否忽略 tx，ty 对坐标的转换。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline void transformPoint(float x, float y, Point& result, bool delta = false) const
    {
        result.x = a * x + c * y;
        result.y = b * x + d * y;

        if (!delta)
        {
            result.x += tx;
            result.y += ty;
        }
    }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_MATRIX_H
