#ifndef DRAGONBONES_CANVAS_DATA_H
#define DRAGONBONES_CANVAS_DATA_H

#include "../core/BaseObject.h"
#include "../geom/Rectangle.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* @private
*/
class CanvasData : public BaseObject
{
    BIND_CLASS_TYPE_A(CanvasData);

public:
    bool hasBackground;
    unsigned color;
    Rectangle aabb;

protected:
    virtual void _onClear() override;
};
DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_CANVAS_DATA_H
