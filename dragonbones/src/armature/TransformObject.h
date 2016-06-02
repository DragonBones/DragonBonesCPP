#ifndef DRAGONBONES_TRANSFORM_OBJECT_H
#define DRAGONBONES_TRANSFORM_OBJECT_H

#include "../core/BaseObject.h"
#include "../geom/Matrix.h"
#include "../geom/Transform.h"

NAMESPACE_DRAGONBONES_BEGIN

class Armature;
class Bone;
class Slot;

class TransformObject : public BaseObject // ABSTRACT
{
public:
    void* userData;
    std::string name;
    Matrix* globalTransformMatrix;
    Transform global;
    Transform origin;
    Transform offset;

public:
    Armature* _armature;
    Bone* _parent;

protected:
    Matrix _globalTransformMatrix;

public:
    TransformObject() {}
    virtual ~TransformObject() {};

protected:
    void _onClear() override
    {
        userData = nullptr;
        name.clear();
        globalTransformMatrix = nullptr;
        global.identity();
        origin.identity();
        offset.identity();

        _armature = nullptr;
        _parent = nullptr;
        _globalTransformMatrix.identity();
    }

public:
    virtual void _setArmature(Armature* value)
    {
        _armature = value;
    }

    virtual void _setParent(Bone* value)
    {
        _parent = value;
    }

public:
    inline Armature* getArmature() const
    {
        return _armature;
    }

    inline Bone* getParent() const
    {
        return _parent;
    }
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_TRANSFORM_OBJECT_H
