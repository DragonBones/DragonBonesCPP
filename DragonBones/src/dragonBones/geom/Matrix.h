#ifndef DRAGONBONES_MATRIX_H
#define DRAGONBONES_MATRIX_H

#include "../core/DragonBones.h"
#include "Point.h"
#include "Rectangle.h"

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

    inline void transformRectangle(Rectangle& rectangle, bool delta = false) const
    {
        const auto a = this->a;
        const auto b = this->b;
        const auto c = this->c;
        const auto d = this->d;
        const auto tx = delta ? 0.0f : this->tx;
        const auto ty = delta ? 0.0f : this->ty;

        const auto x = rectangle.x;
        const auto y = rectangle.y;
        const auto xMax = x + rectangle.width;
        const auto yMax = y + rectangle.height;

        auto x0 = a * x + c * y + tx;
        auto y0 = b * x + d * y + ty;
        auto x1 = a * xMax + c * y + tx;
        auto y1 = b * xMax + d * y + ty;
        auto x2 = a * xMax + c * yMax + tx;
        auto y2 = b * xMax + d * yMax + ty;
        auto x3 = a * x + c * yMax + tx;
        auto y3 = b * x + d * yMax + ty;
        auto tmp = 0;

        if (x0 > x1) 
        {
            tmp = x0;
            x0 = x1;
            x1 = tmp;
        }

        if (x2 > x3) 
        {
            tmp = x2;
            x2 = x3;
            x3 = tmp;
        }

        rectangle.x = std::floor(x0 < x2 ? x0 : x2);
        rectangle.width = std::ceil((x1 > x3 ? x1 : x3) - rectangle.x);

        if (y0 > y1) 
        {
            tmp = y0;
            y0 = y1;
            y1 = tmp;
        }

        if (y2 > y3) 
        {
            tmp = y2;
            y2 = y3;
            y3 = tmp;
        }

        rectangle.y = std::floor(y0 < y2 ? y0 : y2);
        rectangle.height = std::ceil((y1 > y3 ? y1 : y3) - rectangle.y);
    }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_MATRIX_H
