
#ifndef DRAGONBONESCPP_DISPLAYDATA_H
#define DRAGONBONESCPP_DISPLAYDATA_H

#include "../core/BaseObject.h"
#include "../geom/Transform.h"
#include "BoundingBoxData.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
 * @private
 */
class DisplayData : public BaseObject
{
    ABSTRACT_CLASS(DisplayData)

public:
    DisplayType type;
    std::string name;
    std::string path;
    Transform transform;
    SkinData* parent;

protected:
    virtual void _onClear() override;

public: // For WebAssembly.
    int getType() const { return (int)type; }
    void setType(int value) { type = (DisplayType)value; }

    Transform* getTransform() { return &transform; }

    SkinData* getParent() const { return parent; }
    void setParent(SkinData* value) { parent = value; }
};
/**
* @private
*/
class ImageDisplayData : public DisplayData
{
    BIND_CLASS_TYPE_A(ImageDisplayData);

public:
    Point pivot;
    TextureData* texture;

protected:
    virtual void _onClear() override;

public: // For WebAssembly.
    Point* getPivot() { return &pivot; }

    TextureData* getTexture() const { return texture; }
    void setTexture(TextureData* value) { texture = value; }
};
/**
* @private
*/
class ArmatureDisplayData : public DisplayData
{
    BIND_CLASS_TYPE_A(ArmatureDisplayData);

public:
    bool inheritAnimation;
    std::vector<ActionData*> actions;
    ArmatureData* armature;

protected:
    virtual void _onClear() override;

public:
    /**
    * @private
    */
    void addAction(ActionData* value);

public: // For WebAssembly.
    const std::vector<ActionData*>& getActions() const { return actions; }

    ArmatureData* getArmature() const { return armature; }
    void setArmature(ArmatureData* value) { armature = value; }
};
/**
* @private
*/
class MeshDisplayData : public ImageDisplayData
{
    BIND_CLASS_TYPE_B(MeshDisplayData);

public:
    bool inheritAnimation;
    unsigned offset;
    WeightData* weight;

    MeshDisplayData() :
        weight(nullptr)
    { 
        _onClear(); 
    }
    ~MeshDisplayData()
    {
        _onClear();
    }

protected:
    virtual void _onClear() override;

public: // For WebAssembly.
    WeightData* getWeight() const { return weight; }
    void setWeight(WeightData* value) { weight = value; }
};
/**
* @private
*/
class BoundingBoxDisplayData : public DisplayData
{
    BIND_CLASS_TYPE_B(BoundingBoxDisplayData);

public:
    BoundingBoxData* boundingBox;

    BoundingBoxDisplayData() : 
        boundingBox(nullptr)
    { 
        _onClear(); 
    }
    ~BoundingBoxDisplayData()
    {
        _onClear();
    }

protected:
    virtual void _onClear() override;

public: // For WebAssembly.
    const BoundingBoxData* getBoundingBox() const { return boundingBox; }
    void setBoundingBox(BoundingBoxData* value) { boundingBox = value; }
};
/**
* @private
*/
class WeightData : public BaseObject
{
    BIND_CLASS_TYPE_A(WeightData);

public:
    unsigned count;
    unsigned offset;
    std::vector<BoneData*> bones;

protected:
    virtual void _onClear() override;

public:
    void addBone(BoneData* value);

public: // For WebAssembly.
    const std::vector<BoneData*>& getBones() const { return bones; }
};

DRAGONBONES_NAMESPACE_END

#endif //DRAGONBONESCPP_DISPLAYDATA_H
