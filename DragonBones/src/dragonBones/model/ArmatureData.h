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
    Rectangle getAABB() const { return aabb; }
    std::vector<ActionData*>* getDefaultActions() { return &defaultActions; }
    std::vector<ActionData*>* getActions() { return &actions; }
    AnimationData* getDefaultAnimation() const { return defaultAnimation; }
    std::vector<std::string>* getAnimationNames() { return &animationNames; }
    SkinData* getDefaultSkin() const { return defaultSkin; }
    UserData* getUserData() const { return userData; }
    DragonBonesData* getParent() const { return parent; }

    void setAABB(const Rectangle& value) { aabb = value; }
    void inline setDefaultAnimation(AnimationData* value) { defaultAnimation = value; }
    void inline setDefaultSkin(SkinData* value) { defaultSkin = value; }
    void setUserData(UserData* value) { userData = value; }
    void inline setParent(DragonBonesData* value) { parent = value; }
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
    Transform* getTransfrom() { return &transform; }
    std::vector<ConstraintData*>* getConstraints() { return &constraints; }
    BoneData* getParent() const { return parent; }
    UserData* getUserData() const { return userData; }

    void setParent(BoneData* value) { parent = value; }
    void setUserData(UserData* value) { userData = value; }
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
    static ColorTransform* getDEFAULT_COLOR() { return &DEFAULT_COLOR; }

    int getBlendMode() { return (int)blendMode; }
    ColorTransform* getColor() { return color; }
    BoneData* getParent() const { return parent; }
    UserData* getUserData() const { return userData; }

    void setBlendMode(int value) { blendMode = (BlendMode)value; }
    void setColor(ColorTransform* value) { color = value; }
    void setParent(BoneData* value) { parent = value; }
    void setUserData(UserData* value) { userData = value; }
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

public: // For WebAssembly.
    std::vector<std::string> skinSlotNames;
    std::vector<std::string>* getSkinSlotNames() { return &skinSlotNames; } // TODO
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_ARMATURE_DATA_H
