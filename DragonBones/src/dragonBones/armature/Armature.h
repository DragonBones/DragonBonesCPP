#ifndef DRAGONBONES_ARMATURE_H
#define DRAGONBONES_ARMATURE_H

#include "../core/BaseObject.h"
#include "../animation/IAnimatable.h"
#include "../model/ArmatureData.h"
#include "IArmatureProxy.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* 骨架，是骨骼动画系统的核心，由显示容器、骨骼、插槽、动画、事件系统构成。
* @see dragonBones.ArmatureData
* @see dragonBones.Bone
* @see dragonBones.Slot
* @see dragonBones.Animation
* @version DragonBones 3.0
* @language zh_CN
*/
class Armature : public virtual IAnimatable, public BaseObject
{
    BIND_CLASS_TYPE_B(Armature);

private:
    static bool _onSortSlots(Slot* a, Slot* b);

public:
    /**
     * 是否继承父骨架的动画状态。
     * @default true
     * @version DragonBones 4.5
     * @language zh_CN
     */
    bool inheritAnimation;
    /**
     * 用于存储临时数据。
     * @version DragonBones 3.0
     * @language zh_CN
     */
    void* userData;

public:
    int _cacheFrameIndex;
    ArmatureData* _armatureData;
    DragonBones* _dragonBones;
    Slot* _parent;
    TextureAtlasData* _replaceTextureAtlasData;
    std::vector<Constraint*> _constraints;

protected:
    bool _debugDraw;
    bool _lockUpdate;
    bool _bonesDirty;
    bool _slotsDirty;
    bool _zOrderDirty;
    bool _flipX;
    bool _flipY;
    std::vector<Bone*> _bones;
    std::vector<Slot*> _slots;
    std::vector<ActionData*> _actions;
    Animation* _animation;
    IArmatureProxy* _proxy;
    void* _display;
    WorldClock* _clock;
    void* _replacedTexture;

public:
    Armature() :
        _animation(nullptr),
        _proxy(nullptr),
        _clock(nullptr),
        _replaceTextureAtlasData(nullptr)
    {
        _onClear();
    }
    virtual ~Armature()
    {
        _onClear();
    }

protected:
    virtual void _onClear() override;

private:
    void _sortBones();
    void _sortSlots();

public:
    /**
    * @private
    */
    void _sortZOrder(const int16_t* slotIndices, unsigned offset);
    /**
    * @private
    */
    void _addBoneToBoneList(Bone* value);
    /**
    * @private
    */
    void _removeBoneFromBoneList(Bone* value);
    /**
    * @private
    */
    void _addSlotToSlotList(Slot* value);
    /**
    * @private
    */
    void _removeSlotFromSlotList(Slot* value);
    /**
    * @private
    */
    void _bufferAction(ActionData* action, bool append);
    /**
    * 释放骨架。 (回收到对象池)
    * @version DragonBones 3.0
    * @language zh_CN
    */
    void dispose();
    /**
    * @private
    */
    void init(ArmatureData *armatureData, IArmatureProxy* proxy, void* display, DragonBones* dragonBones);
    /**
    * 更新骨架和动画。
    * @param passedTime 两帧之间的时间间隔。 (以秒为单位)
    * @see dragonBones.IAnimateble
    * @see dragonBones.WorldClock
    * @version DragonBones 3.0
    * @language zh_CN
    */
    void advanceTime(float passedTime) override;
    /**
    * 更新骨骼和插槽。 (当骨骼没有动画状态或动画状态播放完成时，骨骼将不在更新)
    * @param boneName 指定的骨骼名称，如果未设置，将更新所有骨骼。
    * @param updateSlotDisplay 是否更新插槽的显示对象。
    * @see dragonBones.Bone
    * @see dragonBones.Slot
    * @version DragonBones 3.0
    * @language zh_CN
    */
    void invalidUpdate(const std::string& boneName = "", bool updateSlot = false);
    /**
    * 判断点是否在所有插槽的自定义包围盒内。
    * @param x 点的水平坐标。（骨架内坐标系）
    * @param y 点的垂直坐标。（骨架内坐标系）
    * @version DragonBones 5.0
    * @language zh_CN
    */
    Slot* containsPoint(float x, float y) const;
    /**
    * 判断线段是否与骨架的所有插槽的自定义包围盒相交。
    * @param xA 线段起点的水平坐标。（骨架内坐标系）
    * @param yA 线段起点的垂直坐标。（骨架内坐标系）
    * @param xB 线段终点的水平坐标。（骨架内坐标系）
    * @param yB 线段终点的垂直坐标。（骨架内坐标系）
    * @param intersectionPointA 线段从起点到终点与包围盒相交的第一个交点。（骨架内坐标系）
    * @param intersectionPointB 线段从终点到起点与包围盒相交的第一个交点。（骨架内坐标系）
    * @param normalRadians 碰撞点处包围盒切线的法线弧度。 [x: 第一个碰撞点处切线的法线弧度, y: 第二个碰撞点处切线的法线弧度]
    * @returns 线段从起点到终点相交的第一个自定义包围盒的插槽。
    * @version DragonBones 5.0
    * @language zh_CN
    */
    Slot* intersectsSegment(
        float xA, float yA, float xB, float yB,
        Point* intersectionPointA = nullptr,
        Point* intersectionPointB = nullptr,
        Point* normalRadians = nullptr
    ) const;
    /**
    * 获取指定名称的骨骼。
    * @param name 骨骼的名称。
    * @returns 骨骼。
    * @see dragonBones.Bone
    * @version DragonBones 3.0
    * @language zh_CN
    */
    Bone* getBone(const std::string& name) const;
    /**
    * 通过显示对象获取骨骼。
    * @param display 显示对象。
    * @returns 包含这个显示对象的骨骼。
    * @see dragonBones.Bone
    * @version DragonBones 3.0
    * @language zh_CN
    */
    Bone* getBoneByDisplay(void* display) const;
    /**
    * 获取插槽。
    * @param name 插槽的名称。
    * @returns 插槽。
    * @see dragonBones.Slot
    * @version DragonBones 3.0
    * @language zh_CN
    */
    Slot* getSlot(const std::string& name) const;
    /**
    * 通过显示对象获取插槽。
    * @param display 显示对象。
    * @returns 包含这个显示对象的插槽。
    * @see dragonBones.Slot
    * @version DragonBones 3.0
    * @language zh_CN
    */
    Slot* getSlotByDisplay(void* display) const;
    /**
    * @deprecated
    */
    void addBone(Bone* value, const std::string& parentName);
    /**
    * @deprecated
    */
    void addSlot(Slot* value, const std::string& boneName);
    void addConstraint(Constraint* value);
    /**
    * @deprecated
    */
    void removeBone(Bone* value);
    /**
    * @deprecated
    */
    void removeSlot(Slot* value);
    /**
    * 获取所有骨骼。
    * @see dragonBones.Bone
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline const std::vector<Bone*>& getBones() const
    {
        return _bones;
    }
    /**
    * 获取所有插槽。
    * @see dragonBones.Slot
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline const std::vector<Slot*>& getSlots() const
    {
        return _slots;
    }

    bool getFlipX() const 
    { 
        return _flipX; 
    }
    void setFlipX(bool value) 
    { 
        _flipX = value;
        invalidUpdate("");
    }

    bool getFlipY() const 
    { 
        return _flipY; 
    }
    void setFlipY(bool value) 
    { 
        _flipY = value;
        invalidUpdate("");
    }
    /**
    * 动画缓存帧率，当设置的值大于 0 的时，将会开启动画缓存。
    * 通过将动画数据缓存在内存中来提高运行性能，会有一定的内存开销。
    * 帧率不宜设置的过高，通常跟动画的帧率相当且低于程序运行的帧率。
    * 开启动画缓存后，某些功能将会失效，比如 Bone 和 Slot 的 offset 属性等。
    * @see dragonBones.DragonBonesData#frameRate
    * @see dragonBones.ArmatureData#frameRate
    * @version DragonBones 4.5
    * @language zh_CN
    */
    inline unsigned getCacheFrameRate() const
    {
        return _armatureData->cacheFrameRate;
    }
    void setCacheFrameRate(unsigned value);
    /**
    * 骨架名称。
    * @see dragonBones.ArmatureData#name
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline const std::string& getName() const
    {
        return _armatureData->name;
    }
    inline const ArmatureData* getArmatureData() const
    {
        return _armatureData;
    }
    /**
    * 获得动画控制器。
    * @see dragonBones.Animation
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline Animation* getAnimation() const
    {
        return _animation;
    }
    /**
    * @pivate
    */
    inline IArmatureProxy* getProxy() const
    {
        return _proxy;
    }
    /**
    * 获取显示容器，插槽的显示对象都会以此显示容器为父级，根据渲染平台的不同，类型会不同，通常是 DisplayObjectContainer 类型。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline void* getDisplay() const
    {
        return _display;
    }
    /**
    * @inheritDoc
    */
    inline WorldClock* getClock() const override
    {
        return _clock;
    }
    void setClock(WorldClock* value) override;
    /**
    * 获取父插槽。 (当此骨架是某个骨架的子骨架时，可以通过此属性向上查找从属关系)
    * @see dragonBones.Slot
    * @version DragonBones 4.5
    * @language zh_CN
    */
    inline Slot* getParent() const
    {
        return _parent;
    }
    /**
    * @language zh_CN
    * 替换骨架的主贴图，根据渲染引擎的不同，提供不同的贴图数据。
    * @version DragonBones 4.5
    */
    inline void* getReplacedTexture() const 
    {
        return _replacedTexture;
    }
    void setReplacedTexture(void* value);

public: // For WebAssembly.
    IAnimatable* getAnimatable() const { return (IAnimatable*)this; }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_ARMATURE_H
