//
// Created by liangshuochen on 08/06/2017.
//

#ifndef DRAGONBONESCPP_BOUNDINGBOXDATA_H
#define DRAGONBONESCPP_BOUNDINGBOXDATA_H

#include "../core/BaseObject.h"
#include "../geom/Point.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* 边界框数据基类。
* @see dragonBones.RectangleData
* @see dragonBones.EllipseData
* @see dragonBones.PolygonData
* @version DragonBones 5.0
* @language zh_CN
*/
class BoundingBoxData : public BaseObject 
{
    ABSTRACT_CLASS(BoundingBoxData);

public:
    /**
    * 边界框类型。
    * @version DragonBones 5.0
    * @language zh_CN
    */
    BoundingBoxType type;
    /**
    * 边界框颜色。
    * @version DragonBones 5.0
    * @language zh_CN
    */
    unsigned color;
    /**
    * 边界框宽。（本地坐标系）
    * @version DragonBones 5.0
    * @language zh_CN
    */
    float width;
    /**
    * 边界框高。（本地坐标系）
    * @version DragonBones 5.0
    * @language zh_CN
    */
    float height;

protected:
    virtual void _onClear() override;

public:
    /**
    * 是否包含点。
    * @version DragonBones 5.0
    * @language zh_CN
    */
    virtual bool containsPoint(float pX, float pY) = 0;
    /**
    * 是否与线段相交。
    * @version DragonBones 5.0
    * @language zh_CN
    */
    virtual int intersectsSegment(
        float xA, float yA, float xB, float yB,
        Point* intersectionPointA = nullptr,
        Point* intersectionPointB = nullptr,
        Point* normalRadians = nullptr
    ) = 0;

public: // For WebAssembly.
    int getType() const { return (int)type; }
    void setType(int value) { type = (BoundingBoxType)value; }
};
/**
* 矩形边界框。
* @version DragonBones 5.1
* @language zh_CN
*/
class RectangleBoundingBoxData : public BoundingBoxData
{
    BIND_CLASS_TYPE_A(RectangleBoundingBoxData);

private:
    /**
    * Cohen–Sutherland algorithm https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
    * ----------------------
    * | 0101 | 0100 | 0110 |
    * ----------------------
    * | 0001 | 0000 | 0010 |
    * ----------------------
    * | 1001 | 1000 | 1010 |
    * ----------------------
    */
    enum OutCode {
        InSide = 0, // 0000
        Left = 1,   // 0001
        Right = 2,  // 0010
        Top = 4,    // 0100
        Bottom = 8  // 1000
    };
    /**
    * Compute the bit code for a point (x, y) using the clip rectangle
    */
    static int _computeOutCode(float x, float y, float xMin, float yMin, float xMax, float yMax);

public:
    /**
    * @private
    */
    static int rectangleIntersectsSegment(
		float xA, float yA, float xB, float yB,
		float xMin, float yMin, float xMax, float yMax,
		Point* intersectionPointA = nullptr,
		Point* intersectionPointB = nullptr,
		Point* normalRadians = nullptr
    );
    /**
    * @inherDoc
    */
    virtual bool containsPoint(float pX, float pY) override;
    /**
    * @inherDoc
    */
    virtual int intersectsSegment(
        float xA, float yA, float xB, float yB,
        Point* intersectionPointA = nullptr,
        Point* intersectionPointB = nullptr,
        Point* normalRadians = nullptr
    ) override;

protected:
    virtual void _onClear() override;
};
/**
* 椭圆边界框。
* @version DragonBones 5.1
* @language zh_CN
*/
class EllipseBoundingBoxData : public BoundingBoxData
{
    BIND_CLASS_TYPE_A(EllipseBoundingBoxData);

public:
    /**
    * @private
    */
    static int ellipseIntersectsSegment(
        float xA, float yA, float xB, float yB,
        float xC, float yC, float widthH, float heightH,
        Point* intersectionPointA = nullptr,
        Point* intersectionPointB = nullptr,
        Point* normalRadians = nullptr
    );
    /**
    * @inherDoc
    */
    virtual bool containsPoint(float pX, float pY) override;
    /**
    * @inherDoc
    */
    virtual int intersectsSegment(
        float xA, float yA, float xB, float yB,
        Point* intersectionPointA = nullptr,
        Point* intersectionPointB = nullptr,
        Point* normalRadians = nullptr
    ) override;

protected:
    virtual void _onClear() override;
};
/**
* 多边形边界框。
* @version DragonBones 5.1
* @language zh_CN
*/
class PolygonBoundingBoxData : public BoundingBoxData
{
    BIND_CLASS_TYPE_B(PolygonBoundingBoxData);

public:
    /**
    * @private
    */
    static int polygonIntersectsSegment(
        float xA, float yA, float xB, float yB,
        float* vertices, unsigned offset, unsigned count,
        Point* intersectionPointA = nullptr,
        Point* intersectionPointB = nullptr,
        Point* normalRadians = nullptr
    );
	// TODO Check (int->unsigned)
    /**
    * @private
    */
    unsigned count;
    /**
    * @private
    */
    unsigned offset; // FloatArray.
    /**
    * @private
    */
    float x;
    /**
    * @private
    */
    float y;
    /**
    * 多边形顶点。
    * @version DragonBones 5.1
    * @language zh_CN
    */
    float* vertices; // FloatArray.
    /**
    * @private
    */
    WeightData* weight;
    /**
    * @inherDoc
    */
    virtual bool containsPoint(float pX, float pY) override;
    /**
    * @inherDoc
    */
    virtual int intersectsSegment(
        float xA, float yA, float xB, float yB,
        Point* intersectionPointA = nullptr,
        Point* intersectionPointB = nullptr,
        Point* normalRadians = nullptr
    ) override;

    PolygonBoundingBoxData() :
        weight(nullptr)
    {
        _onClear();
    }
    ~PolygonBoundingBoxData()
    {
        _onClear();
    }

protected:
    virtual void _onClear() override;

public: // For WebAssembly.
    WeightData* getWeight() const { return weight; }
    void setWeight(WeightData* value) { weight = value; }
};

DRAGONBONES_NAMESPACE_END
#endif //DRAGONBONESCPP_BOUNDINGBOXDATA_H
