#ifndef DRAGONBONES_BONE_H
#define DRAGONBONES_BONE_H

#include "TransformObject.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* 骨骼，一个骨架中可以包含多个骨骼，骨骼以树状结构组成骨架。
* 骨骼在骨骼动画体系中是最重要的逻辑单元之一，负责动画中的平移旋转缩放的实现。
* @see dragonBones.BoneData
* @see dragonBones.Armature
* @see dragonBones.Slot
* @version DragonBones 3.0
* @language zh_CN
*/
class Bone final : public TransformObject
{
    BIND_CLASS_TYPE_A(Bone);

public:
    /**
    * @private
    */
    OffsetMode offsetMode;
    /**
    * @private
    */
    Transform animationPose;
    /**
    * @private
    */
    std::vector<Constraint*> constraints;
    /**
    * @readonly
    */
    BoneData* boneData;
    /**
    * @private
    */
    bool _transformDirty;
    /**
    * @private
    */
    bool _childrenTransformDirty;
    /**
    * @private
    */
    bool _blendDirty;
    /**
    * @private
    */
    int _blendLayer;
    /**
    * @private
    */
    float _blendLeftWeight;
    /**
    * @private
    */
    float _blendLayerWeight;
    /**
    * @private
    */
    std::vector<int>* _cachedFrameIndices;

private:
    bool _localDirty;
    bool _visible;
    int _cachedFrameIndex;
    mutable std::vector<Bone*> _bones;
    mutable std::vector<Slot*> _slots;
    void _updateGlobalTransformMatrix(bool isCache);

protected:
    void _onClear() override;

public:
    /**
    * @private
    */
    virtual void _setArmature(Armature* value) override;

public:
    /**
    * @private
    */
    void init(BoneData* boneData);
    /**
    * @private
    */
    void update(int cacheFrameIndex);
    /**
    * @private
    */
    void updateByConstraint();
    /**
    * @private
    */
    void addConstraint(Constraint* constraint);
    /**
    * 下一帧更新变换。 (当骨骼没有动画状态或动画状态播放完成时，骨骼将不在更新)
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline void invalidUpdate()
    {
        _transformDirty = true;
    }
    /**
    * 是否包含骨骼或插槽。
    * @returns
    * @see dragonBones.TransformObject
    * @version DragonBones 3.0
    * @language zh_CN
    */
    bool contains(const TransformObject* child) const;
    /**
    * 所有的子骨骼。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    const std::vector<Bone*>& getBones() const;
    /**
    * 所有的插槽。
    * @see dragonBones.Slot
    * @version DragonBones 3.0
    * @language zh_CN
    */
    const std::vector<Slot*>& getSlots() const;
    /**
    * 控制此骨骼所有插槽的可见。
    * @default true
    * @see dragonBones.Slot
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline bool getVisible() const
    {
        return _visible;
    }
    void setVisible(bool value);

public: // For WebAssembly.
    inline int getOffsetMode() const { return (int)offsetMode; }
    inline void setOffsetMode(int value) { offsetMode = (OffsetMode)value; }
    inline const BoneData* getBoneData() const { return boneData; }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_BONE_H
