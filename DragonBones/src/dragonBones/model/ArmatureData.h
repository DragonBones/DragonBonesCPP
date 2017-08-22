#ifndef DRAGONBONES_ARMATURE_DATA_H
#define DRAGONBONES_ARMATURE_DATA_H

#include "../core/BaseObject.h"
#include "../geom/Matrix.h"
#include "../geom/Transform.h"
#include "../geom/ColorTransform.h"
#include "../geom/Rectangle.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
 * 骨架数据。
 * @version DragonBones 3.0
 * @language zh_CN
 */
class ArmatureData : public BaseObject
{
    BIND_CLASS_TYPE_B(ArmatureData);

public:
    /**
    * @private
    */
    ArmatureType type;
    /**
    * 动画帧率。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    unsigned frameRate;
    /**
    * @private
    */
    unsigned cacheFrameRate;
    /**
    * @private
    */
    float scale;
    /**
    * 数据名称。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::string name;
    /**
    * @private
    */
    Rectangle aabb;
    /**
    * 所有动画数据名称。
    * @see #armatures
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::vector<std::string> animationNames;
    /**
    * @private
    */
    std::vector<BoneData*> sortedBones;
    /**
    * @private
    */
    std::vector<SlotData*> sortedSlots;
    /**
    * @private
    */
    std::vector<ActionData*> defaultActions;
    /**
    * @private
    */
    std::vector<ActionData*> actions;
    /**
    * 所有骨骼数据。
    * @see dragonBones.BoneData
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::map<std::string, BoneData*> bones;
    /**
    * 所有插槽数据。
    * @see dragonBones.SlotData
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::map<std::string, SlotData*> slots;
    /**
    * 所有皮肤数据。
    * @see dragonBones.SkinData
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::map<std::string, SkinData*> skins;
    /**
    * 所有动画数据。
    * @see dragonBones.AnimationData
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::map<std::string, AnimationData*> animations;
    /**
    * 获取默认皮肤数据。
    * @see dragonBones.SkinData
    * @version DragonBones 4.5
    * @language zh_CN
    */
    SkinData* defaultSkin;
    /**
    * @private
    */
    AnimationData* defaultAnimation;
    /**
    * @private
    */
    UserData* userData;
    /**
    * 所属的龙骨数据。
    * @see dragonBones.DragonBonesData
    * @version DragonBones 4.5
    * @language zh_CN
    */
    DragonBonesData* parent;
    /**
    * @private
    */
    ArmatureData() :
        userData(nullptr)
    {
        _onClear();
    }
    ~ArmatureData()
    {
        _onClear();
    }

protected:
    virtual void _onClear() override;

public:
    /**
    * @private
    */
    void sortBones();
    /**
    * @private
    */
    void cacheFrames(unsigned frameRate);
    /**
    * @private
    */
    int setCacheFrame(const Matrix& globalTransformMatrix, const Transform& transform);
    /**
    * @private
    */
    void getCacheFrame(Matrix& globalTransformMatrix, Transform& transform, unsigned arrayOffset);
    /**
    * @private
    */
    void addBone(BoneData* value);
    /**
    * @private
    */
    void addSlot(SlotData* value);
    /**
    * @private
    */
    void addSkin(SkinData* value);
    /**
    * @private
    */
    void addAnimation(AnimationData* value);
    /**
    * 获取骨骼数据。
    * @param name 数据名称。
    * @version DragonBones 3.0
    * @see dragonBones.BoneData
    * @language zh_CN
    */
    inline BoneData* getBone(const std::string& name) const
    {
        return mapFind<BoneData>(bones, name);
    }
    /**
    * 获取插槽数据。
    * @param name 数据名称。
    * @version DragonBones 3.0
    * @see dragonBones.SlotData
    * @language zh_CN
    */
    inline SlotData* getSlot(const std::string& name) const
    {
        return mapFind<SlotData>(slots, name);
    }
    /**
    * 获取皮肤数据。
    * @param name 数据名称。
    * @version DragonBones 3.0
    * @see dragonBones.SkinData
    * @language zh_CN
    */
    inline SkinData* getSkin(const std::string& name) const
    {
        return mapFind(skins, name);
    }
    /**
    * 获取动画数据。
    * @param name 数据名称。
    * @version DragonBones 3.0
    * @see dragonBones.AnimationData
    * @language zh_CN
    */
    inline AnimationData* getAnimation(const std::string& name) const
    {
        return mapFind(animations, name);
    }

public: // For WebAssembly.
    const Rectangle& getAABB() const { return aabb; }
    const SkinData* getDefaultSkin() const { return defaultSkin; }
    const AnimationData* getDefaultAnimation() const { return defaultAnimation; }
    const UserData* getUserData() const { return userData; }
    const DragonBonesData* getParent() const { return parent; }
};
/**
* 骨骼数据。
* @version DragonBones 3.0
* @language zh_CN
*/
class BoneData : public BaseObject 
{
    BIND_CLASS_TYPE_B(BoneData);

public:
    /**
    * @private
    */
    bool inheritTranslation;
    /**
    * @private
    */
    bool inheritRotation;
    /**
    * @private
    */
    bool inheritScale;
    /**
    * @private
    */
    bool inheritReflection;
    /**
    * @private
    */
    float length;
    /**
    * 数据名称。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::string name;
    /**
    * @private
    */
    Transform transform;
    /**
    * @private
    */
    std::vector<ConstraintData*> constraints;
    /**
    * 所属的父骨骼数据。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    BoneData* parent;
    /**
    * @private
    */
    UserData* userData;
    /**
    * @private
    */
    BoneData() :
        userData(nullptr)
    {
        _onClear();
    }
    ~BoneData()
    {
        _onClear();
    }

protected:
    virtual void _onClear() override;

public: // For WebAssembly.
    const Transform& getTransfrom() const { return transform; }
    const UserData* getUserData() const { return userData; }
    const BoneData* getParent() const { return parent; }
};
/**
* 插槽数据。
* @see dragonBones.Slot
* @version DragonBones 3.0
* @language zh_CN
*/
class SlotData : public BaseObject
{
    BIND_CLASS_TYPE_B(SlotData);

public:
    /**
    * @private
    */
    static ColorTransform DEFAULT_COLOR;
    /**
    * @private
    */
    static ColorTransform* createColor();

public:
    /**
    * @private
    */
    BlendMode blendMode;
    /**
    * @private
    */
    int displayIndex;
    /**
    * @private
    */
    int zOrder;
    /**
    * 数据名称。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::string name;
    /**
    * 所属的父骨骼数据。
    * @see dragonBones.BoneData
    * @version DragonBones 3.0
    * @language zh_CN
    */
    BoneData* parent;
    /**
    * @private
    */
    ColorTransform* color;
    /**
    * @private
    */
    UserData* userData;
    /**
    * @private
    */
    SlotData() :
        color(nullptr),
        userData(nullptr)
    {
        _onClear();
    }
    ~SlotData()
    {
        _onClear();
    }

protected:
    virtual void _onClear() override;

public: // For WebAssembly.
    BoneData* getParent() const { return parent; }
    UserData* getUserData() const { return userData; }
};
/**
* 皮肤数据。（通常一个骨架数据至少包含一个皮肤数据）
* @version DragonBones 3.0
* @language zh_CN
*/
class SkinData final : public BaseObject
{
    BIND_CLASS_TYPE_A(SkinData);

public:
    /**
    * 数据名称。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::string name;
    /**
    * @private
    */
    std::map<std::string, std::vector<DisplayData*>> displays;

protected:
    virtual void _onClear() override;

public:
    /**
    * @private
    */
    void addDisplay(const std::string& slotName, DisplayData* value);
    /**
    * @private
    */
    DisplayData* getDisplay(const std::string& slotName, const std::string& displayName);
    /**
    * @private
    */
    inline std::vector<DisplayData*>* getDisplays(const std::string& slotName)
    {
        return mapFindB(displays, slotName);
    }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_ARMATURE_DATA_H
