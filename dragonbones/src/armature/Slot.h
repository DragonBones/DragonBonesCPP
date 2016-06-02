#ifndef DRAGONBONES_SLOT_H
#define DRAGONBONES_SLOT_H

#include "TransformObject.h"
#include "../model/ArmatureData.h"
#include "Bone.h"

NAMESPACE_DRAGONBONES_BEGIN

class Slot : public TransformObject
{
public:
    bool inheritAnimation;
    std::string displayController;

public:
    bool _colorDirty;
    bool _ffdDirty;
    int _zOrder;
    BlendMode _blendMode;
    SlotDisplayDataSet* _displayDataSet;
    MeshData* _meshData;
    std::vector<Matrix*>* _cacheFrames;
    void* _rawDisplay;
    ColorTransform _colorTransform;
    std::vector<float> _ffdVertices;

protected:
    bool _displayDirty;
    bool _blendModeDirty;
    bool _originDirty;
    bool _transformDirty;
    int _displayIndex;
    void* _display;
    Armature* _childArmature;
    Matrix _localMatrix;
    std::vector<std::pair<void*, DisplayType>> _displayList;
    std::vector<Bone*> _meshBones;

public:
    Slot();
    virtual ~Slot() = 0;

protected:
    void _onClear() override;

    virtual void _onUpdateDisplay() = 0;
    virtual void _initDisplay(void* value) = 0;
    virtual void _addDisplay() = 0;
    virtual void _replaceDisplay(void* value) = 0;
    virtual void _removeDisplay() = 0;
    virtual void _disposeDisplay(void* value) = 0;
    virtual void _updateColor() = 0;
    virtual void _updateFilters() = 0;
    virtual void _updateFrame() = 0;
    virtual void _updateMesh() = 0;
    virtual void _updateTransform() = 0;
    void _updateDisplay();

    inline void _updateLocalTransformMatrix()
    {
        this->global = this->origin;
        this->global.add(this->offset).toMatrix(_localMatrix);
    }

    inline void _updateGlobalTransformMatrix()
    {
        this->globalTransformMatrix = &_localMatrix;
        this->globalTransformMatrix->concat(*this->_parent->globalTransformMatrix);
        this->global.fromMatrix(*this->globalTransformMatrix);
    }

    inline bool _isMeshBonesUpdate() const
    {
        for (auto bone : _meshBones)
        {
            if (bone->_transformDirty != Bone::BoneTransformDirty::None)
            {
                return true;
            }
        }

        return false;
    }

public:
    virtual void _updateVisible() = 0;
    virtual void _updateBlendMode() = 0;

    virtual void _setArmature(Armature* value) override;
    void _updateMeshData(bool isTimelineUpdate);
    void _update(int cacheFrameIndex);
    bool _setDisplayList(const std::vector<std::pair<void*, DisplayType>>& value);
    bool _setDisplayIndex(int value);
    bool _setBlendMode(BlendMode value);
    bool _setColor(const ColorTransform& value);

public:
    inline void invalidUpdate() {} // TODO
    inline void* getRawDisplay() const
    {
        return _rawDisplay;
    }

    inline int getDisplayIndex() const
    {
        return _displayIndex;
    }
    void setDisplayIndex(int value);

    inline const std::vector<std::pair<void*, DisplayType>>& getDisplayList() const
    {
        return _displayList;
    }
    void setDisplayList(const std::vector<std::pair<void*, DisplayType>>& value);

    inline void* getDisplay() const
    {
        return _display;
    }
    void setDisplay(void* value, DisplayType displayType);
    
    inline Armature* getChildArmature() const
    {
        return _childArmature;
    }
    void setChildArmature(Armature* value);
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_SLOT_H
