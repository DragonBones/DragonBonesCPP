#ifndef OBJECTS_TRANSFORM_FRAME_H
#define OBJECTS_TRANSFORM_FRAME_H

#include "../DragonBones.h"
#include "Frame.h"
#include "../geoms/Point.h"
#include "../geoms/Transform.h"
#include "../geoms/ColorTransform.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class TransformFrame : public Frame
{
public:
    bool visible;
    bool tweenScale;
    int tweenRotate;
    int displayIndex;
    float zOrder;
    // NaN:no tween, 10:auto tween, [-1, 0):ease in, 0:line easing, (0, 1]:ease out, (1, 2]:ease in out
    float tweenEasing;
    
    Transform global;
    Transform transform;
    Point pivot;
    Point scaleOffset;
    ColorTransform *color;
    
public:
    TransformFrame()
    {
        visible = true;
        tweenScale = true;
        tweenRotate = 0;
        displayIndex = 0;
        zOrder = 0.f;
        tweenEasing = NO_TWEEN_EASING;
        frameType = FrameType::FT_TRANSFORM_FRAME;
        color = nullptr;
    }
    TransformFrame(const TransformFrame &copyData)
    {
        operator=(copyData);
    }
    TransformFrame& operator=(const TransformFrame &copyData)
    {
        Frame::operator=(copyData);
        _dispose();
        visible = copyData.visible;
        tweenScale = copyData.tweenScale;
        tweenRotate = copyData.tweenRotate;
        displayIndex = copyData.displayIndex;
        zOrder = copyData.zOrder;
        tweenEasing = copyData.tweenEasing;
        global = copyData.global;
        transform = copyData.transform;
        pivot = copyData.pivot;
        scaleOffset = copyData.scaleOffset;
        
        if (copyData.color)
        {
            color = new ColorTransform();
            *color = *(copyData.color);
        }
        
        return *this;
    }
    virtual ~TransformFrame()
    {
        dispose();
    }
    virtual void dispose()
    {
        Frame::dispose();
        _dispose();
    }
    
private:
    void _dispose()
    {
        if (color)
        {
            delete color;
            color = nullptr;
        }
    }
};
NAME_SPACE_DRAGON_BONES_END
#endif  // OBJECTS_TRANSFORM_FRAME_H
