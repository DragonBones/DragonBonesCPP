#ifndef DRAGONBONES_TRANSFORM_OBJECT_H
#define DRAGONBONES_TRANSFORM_OBJECT_H

#include "../core/BaseObject.h"
#include "../geom/Matrix.h"
#include "../geom/Transform.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* 基础变换对象。
* @version DragonBones 4.5
* @language zh_CN
*/
class TransformObject : public BaseObject
{
    ABSTRACT_CLASS(TransformObject);

protected:
    static Matrix _helpMatrix;
    static Transform _helpTransform;
    static Point _helpPoint;

public:
    /**
    * 对象的名称。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::string name;
    /**
    * 相对于骨架坐标系的矩阵。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    Matrix globalTransformMatrix;
    /**
    * 相对于骨架坐标系的变换。
    * @see dragonBones.Transform
    * @version DragonBones 3.0
    * @language zh_CN
    */
    Transform global;
    /**
    * 相对于骨架或父骨骼坐标系的偏移变换。
    * @see dragonBones.Transform
    * @version DragonBones 3.0
    * @language zh_CN
    */
    Transform offset;
    /**
    * 相对于骨架或父骨骼坐标系的绑定变换。
    * @see dragonBones.Transform
    * @version DragonBones 3.0
    * @readOnly
    * @language zh_CN
    */
    Transform* origin;
    /**
    * 可以用于存储临时数据。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    void* userData;

public:
    /**
    * @private
    */
    Armature* _armature;
    /**
    * @private
    */
    Bone* _parent;

protected:
    bool _globalDirty;

protected:
    virtual void _onClear() override;

public:
    /**
    * @private
    */
    virtual void _setArmature(Armature* value);
    /**
    * @private
    */
    virtual void _setParent(Bone* value);

public:
    /**
    * @private
    */
    void updateGlobalTransform();
    /**
    * 所属的骨架。
    * @see dragonBones.Armature
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline Armature* getArmature() const
    {
        return _armature;
    }
    /**
    * 所属的父骨骼。
    * @see dragonBones.Bone
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline Bone* getParent() const
    {
        return _parent;
    }

public: // For WebAssembly.
    Matrix* getGlobalTransformMatrix() { return &globalTransformMatrix; }
    Transform* getGlobal() { return &global; }
    Transform* getOffset() { return &offset; }
    Transform* getOrigin() const { return origin; }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_TRANSFORM_OBJECT_H
