#ifndef DRAGONBONES_ARMATURE_DATA_H
#define DRAGONBONES_ARMATURE_DATA_H

#include "../core/BaseObject.h"
#include "../geom/Transform.h"
#include "../geom/Point.h"
#include "../geom/ColorTransform.h"
#include "AnimationData.h"

class ArmatureData;
class TextureData;

NAMESPACE_DRAGONBONES_BEGIN

class BoneData : public BaseObject 
{
    BIND_CLASS_TYPE(BoneData);

public:
    bool inheritTranslation;
    bool inheritRotation;
    bool inheritScale;
    bool bendPositive;
    unsigned chain;
    float weight;
    float length;
    std::string name;
    BoneData* parent;
    BoneData* ik;
    Transform transform;

    BoneData();
    ~BoneData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(BoneData);

protected:
    virtual void _onClear() override;
};

class SlotData final : public BaseObject
{
    BIND_CLASS_TYPE(SlotData);

public:
	static ColorTransform DEFAULT_COLOR;
    static ColorTransform* generateColor();

public:
    int displayIndex;
    int zOrder;
    BlendMode blendMode;
    std::string name;
    BoneData* parent;
    ColorTransform* color;

    SlotData();
    ~SlotData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(SlotData);

protected:
    void _onClear() override;
};

class MeshData final : public BaseObject
{
    BIND_CLASS_TYPE(MeshData);

public:
    bool skinned;
    Matrix slotPose;

    std::vector<float> uvs;
    std::vector<float> vertices;
    std::vector<unsigned short> vertexIndices;

    std::vector<std::vector<unsigned short>> boneIndices;
    std::vector<std::vector<float>> weights;
    std::vector<std::vector<float>> boneVertices;

    std::vector<BoneData*> bones;
    std::vector<Matrix> inverseBindPose;

    MeshData();
    ~MeshData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(MeshData);

protected:
    void _onClear() override;
};

class DisplayData final : public BaseObject
{
    BIND_CLASS_TYPE(DisplayData);

public:
    bool isRelativePivot;
    DisplayType type;
    std::string name;
    TextureData* textureData;
    ArmatureData* armatureData;
    MeshData* meshData;
    Point pivot;
    Transform transform;

    DisplayData();
    ~DisplayData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(DisplayData);

protected:
    void _onClear() override;
};

class SlotDisplayDataSet final : public BaseObject
{
    BIND_CLASS_TYPE(SlotDisplayDataSet);

public:
    SlotData* slot;
    std::vector<DisplayData*> displays;

    SlotDisplayDataSet();
    ~SlotDisplayDataSet();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(SlotDisplayDataSet);

protected:
    void _onClear() override;
};

class SkinData final : public BaseObject
{
    BIND_CLASS_TYPE(SkinData);

public:
    std::string name;
    std::map<std::string, SlotDisplayDataSet*> slots;

    SkinData();
    ~SkinData();

protected:
    void _onClear() override;

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(SkinData);

public:
    void addSlot(SlotDisplayDataSet* value);

    inline SlotDisplayDataSet* getSlot(const std::string& name) const
    {
        return mapFind(slots, name);
    }
};

class ArmatureData final : public BaseObject
{
    BIND_CLASS_TYPE(ArmatureData);

private:
    static bool _onSortSlots(const SlotData* a, const SlotData* b);

public:
    unsigned frameRate;
    unsigned cacheFrameRate;
    ArmatureType type;
    std::string name;
    std::map<std::string, BoneData*> bones;
    std::map<std::string, SlotData*> slots;
    std::map<std::string, SkinData*> skins;
    std::map<std::string, AnimationData*> animations;

private:
    bool _boneDirty;
    bool _slotDirty;
    SkinData* _defaultSkin;
    AnimationData* _defaultAnimation;
    std::vector<BoneData*> _sortedBones;
    std::vector<SlotData*> _sortedSlots;
    std::map<std::string, std::vector<BoneData*>> _bonesChildren;

public:
    ArmatureData();
    ~ArmatureData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(ArmatureData);

    void _sortBones();
    void _sortSlots();

protected:
    void _onClear() override;

public:
    void cacheFrame(unsigned value);
    void addBone(BoneData* value, const std::string& parentName = "");
    void addSlot(SlotData* value);
    void addSkin(SkinData* value);
    void addAnimation(AnimationData* value);

    inline BoneData* getBone(const std::string& name) const
    {
        return mapFind(bones, name);
    }

    inline SlotData* getSlot(const std::string& name) const
    {
        return mapFind(slots, name);
    }

    inline SkinData* getSkin(const std::string& name) const
    {
        if (name.empty())
        {
            return _defaultSkin;
        }

        return mapFind(skins, name);
    }

    inline AnimationData* getAnimation(const std::string& name) const
    {
        if (name.empty())
        {
            return _defaultAnimation;
        }

        return mapFind(animations, name);
    }

    inline const std::vector<BoneData*>& getSortedBones() // const
    {
        if (_boneDirty)
        {
            _boneDirty = false;
            _sortBones();
        }

        return _sortedBones;
    }

    inline const std::vector<SlotData*>& getSortedSlots() // const
    {
        if (_slotDirty)
        {
            _slotDirty = false;
            _sortSlots();
        }

        return _sortedSlots;
    }

    inline SkinData* getDefaultSkin() const
    {
        return _defaultSkin;
    }

    inline AnimationData* getDefaultAnimation() const
    {
        return _defaultAnimation;
    }
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_ARMATURE_DATA_H