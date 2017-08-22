#ifndef DRAGONBONES_SLOT_H
#define DRAGONBONES_SLOT_H

#include "TransformObject.h"

#include "../geom/ColorTransform.h"
#include "../geom/Rectangle.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* 插槽，附着在骨骼上，控制显示对象的显示状态和属性。
* 一个骨骼上可以包含多个插槽。
* 一个插槽中可以包含多个显示对象，同一时间只能显示其中的一个显示对象，但可以在动画播放的过程中切换显示对象实现帧动画。
* 显示对象可以是普通的图片纹理，也可以是子骨架的显示容器，网格显示对象，还可以是自定义的其他显示对象。
* @see dragonBones.Armature
* @see dragonBones.Bone
* @see dragonBones.SlotData
* @version DragonBones 3.0
* @language zh_CN
*/
class Slot : public TransformObject
{
public:
    /**
    * 显示对象受到控制的动画状态或混合组名称，设置为 null 则表示受所有的动画状态控制。
    * @default null
    * @see dragonBones.AnimationState#displayControl
    * @see dragonBones.AnimationState#name
    * @see dragonBones.AnimationState#group
    * @version DragonBones 4.5
    * @language zh_CN
    */
    std::string displayController;
    /**
    * @readonly
    */
    SlotData* slotData;

public:
    bool _colorDirty;
    bool _meshDirty;
    BlendMode _blendMode;
    int _zOrder;
    float _pivotX;
    float _pivotY;

    ColorTransform _colorTransform;
    std::vector<float> _ffdVertices;
    std::vector<DisplayData*> _displayDatas;
    std::vector<DisplayData*>* _rawDisplayDatas;
    MeshDisplayData* _meshData;
    void* _rawDisplay;
    void* _meshDisplay;
    std::vector<int>* _cachedFrameIndices;

protected:
    bool _displayDirty;
    bool _zOrderDirty;
    bool _visibleDirty;
    bool _blendModeDirty;
    bool _transformDirty;
    bool _visible;
    int _displayIndex;
    int _animationDisplayIndex;
    int _cachedFrameIndex;
    Matrix _localMatrix;
    std::vector<std::pair<void*, DisplayType>> _displayList;
    std::vector<Bone*> _meshBones;
    DisplayData* _displayData;
    TextureData* _textureData;
    BoundingBoxData* _boundingBoxData;
    void* _display;
    Armature* _childArmature;

public:
    Slot() :
        _rawDisplay(nullptr),
        _meshDisplay(nullptr)
    {}
    virtual ~Slot() {};

protected:
    virtual void _onClear() override;

    virtual void _initDisplay(void* value) = 0;
    virtual void _disposeDisplay(void* value) = 0;
    virtual void _onUpdateDisplay() = 0;
    virtual void _addDisplay() = 0;
    virtual void _replaceDisplay(void* value, bool isArmatureDisplay) = 0;
    virtual void _removeDisplay() = 0;
    virtual void _updateZOrder() = 0;
public:
    virtual void _updateVisible() = 0;
    virtual void _updateBlendMode() = 0;
    virtual void _updateColor() = 0;
protected:
    virtual void _updateFrame() = 0;
    virtual void _updateMesh() = 0;
    virtual void _updateTransform(bool isSkinnedMesh) = 0;

    bool _isMeshBonesUpdate() const;
    void _updateDisplay();
    void _updateDisplayData();
    void _updateGlobalTransformMatrix(bool isCache);

public:
    virtual void _setArmature(Armature* value) override;
    bool _setDisplayIndex(int value, bool isAnimation = false);
    bool _setZorder(int value);
    bool _setColor(const ColorTransform& value);
    bool _setDisplayList(const std::vector<std::pair<void*, DisplayType>>& value);

public:
    void init(SlotData* slotData, std::vector<DisplayData*> * displayDatas, void* rawDisplay, void* meshDisplay);
    void update(int cacheFrameIndex);
    /**
    * @private
    */
    inline void updateTransformAndMatrix()
    {
        if (_transformDirty)
        {
            _transformDirty = false;
            _updateGlobalTransformMatrix(false);
        }
    }
    /**
    * 判断指定的点是否在插槽的自定义包围盒内。
    * @param x 点的水平坐标。（骨架内坐标系）
    * @param y 点的垂直坐标。（骨架内坐标系）
    * @param color 指定的包围盒颜色。 [0: 与所有包围盒进行判断, N: 仅当包围盒的颜色为 N 时才进行判断]
    * @version DragonBones 5.0
    * @language zh_CN
    */
    bool containsPoint(float x, float y);
    /**
    * 判断指定的线段与插槽的自定义包围盒是否相交。
    * @param xA 线段起点的水平坐标。（骨架内坐标系）
    * @param yA 线段起点的垂直坐标。（骨架内坐标系）
    * @param xB 线段终点的水平坐标。（骨架内坐标系）
    * @param yB 线段终点的垂直坐标。（骨架内坐标系）
    * @param intersectionPointA 线段从起点到终点与包围盒相交的第一个交点。（骨架内坐标系）
    * @param intersectionPointB 线段从终点到起点与包围盒相交的第一个交点。（骨架内坐标系）
    * @param normalRadians 碰撞点处包围盒切线的法线弧度。 [x: 第一个碰撞点处切线的法线弧度, y: 第二个碰撞点处切线的法线弧度]
    * @returns 相交的情况。 [-1: 不相交且线段在包围盒内, 0: 不相交, 1: 相交且有一个交点且终点在包围盒内, 2: 相交且有一个交点且起点在包围盒内, 3: 相交且有两个交点, N: 相交且有 N 个交点]
    * @version DragonBones 5.0
    * @language zh_CN
    */
    int intersectsSegment(
        float xA, float yA, float xB, float yB,
        Point* intersectionPointA = nullptr,
        Point* intersectionPointB = nullptr,
        Point* normalRadians = nullptr
    );
    /**
    * 在下一帧更新显示对象的状态。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    inline void invalidUpdate()
    {
        _displayDirty = true;
        _transformDirty = true;
    }
    /**
    * 此时显示的显示对象在显示列表中的索引。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    inline int getDisplayIndex() const
    {
        return _displayIndex;
    }
    void setDisplayIndex(int value);
    /**
    * 包含显示对象或子骨架的显示列表。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline const std::vector<std::pair<void*, DisplayType>>& getDisplayList() const
    {
        return _displayList;
    }
    void setDisplayList(const std::vector<std::pair<void*, DisplayType>>& value);
    /**
    * @language zh_CN
    * 插槽此时的自定义包围盒数据。
    * @see dragonBones.Armature
    * @version DragonBones 3.0
    */
    inline const BoundingBoxData* getBoundingBoxData() const
    {
        return _boundingBoxData;
    }
    /**
    * @private
    */
    inline const void* getRawDisplay() const
    {
        return _rawDisplay;
    }
    /**
    * @private
    */
    inline const void* getMeshDisplay() const
    {
        return _meshDisplay;
    }
    /**
    * 此时显示的显示对象。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline const void* getDisplay() const
    {
        return _display;
    }
    void setDisplay(void* value, DisplayType displayType);
    /**
    * 此时显示的子骨架。
    * @see dragonBones.Armature
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline Armature* getChildArmature()
    {
        return _childArmature;
    }
    void setChildArmature(Armature* value);

public: // For WebAssembly.
    inline const SlotData* getSlotData() const { return slotData; }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_SLOT_H
