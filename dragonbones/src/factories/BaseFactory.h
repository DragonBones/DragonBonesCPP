#ifndef DRAGONBONES_BASE_FACTORY_H
#define DRAGONBONES_BASE_FACTORY_H

#include "../parsers/JSONDataParser.h"
#include "../armature/Armature.h"
#include "../animation/Animation.h"
#include "../armature/Bone.h"
#include "../armature/Slot.h"

NAMESPACE_DRAGONBONES_BEGIN

class BuildArmaturePackage final
{
public:
    std::string dataName;
    DragonBonesData* data;
    ArmatureData* armature;
    SkinData* skin;

public:
    BuildArmaturePackage() :
        dataName(),
        data(nullptr),
        armature(nullptr),
        skin(nullptr)
    {
    }
    ~BuildArmaturePackage() {}

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(BuildArmaturePackage);
};

class BaseFactory
{
public:
    bool autoSearch;

protected:
    JSONDataParser _jsonDataParser;
    std::map<std::string, DragonBonesData*> _dragonBonesDataMap;
    std::map<std::string, std::vector<TextureAtlasData*>> _textureAtlasDataMap;

public:
    BaseFactory();
    virtual ~BaseFactory() = 0;

protected:
    TextureData* _getTextureData(const std::string& textureAtlasName, const std::string& textureName) const;
    bool _fillBuildArmaturePackage(const std::string& dragonBonesName, const std::string& armatureName, const std::string& skinName, BuildArmaturePackage& dataPackage) const;
    void _buildBones(const BuildArmaturePackage& dataPackage, Armature& armature) const;
    void _buildSlots(const BuildArmaturePackage& dataPackage, Armature& armature) const;

    virtual TextureAtlasData* _generateTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const = 0;
    virtual Armature* _generateArmature(const BuildArmaturePackage& dataPackage) const = 0;
    virtual Slot* _generateSlot(const BuildArmaturePackage& dataPackage, const SlotDisplayDataSet& slotDisplayDataSet) const = 0;
    virtual void* _generateDisplay(const BuildArmaturePackage& dataPackage, const DisplayData& displayData, const DisplayData& rawDisplayData) const = 0;

public:
    DragonBonesData* parseDragonBonesData(const char* rawData, const std::string& dragonBonesName = "");
    TextureAtlasData* parseTextureAtlasData(const char* rawData, void* textureAtlas, const std::string& name = "", float scale = 0.f);
    void addDragonBonesData(DragonBonesData* data, const std::string& dragonBonesName = "");
    void removeDragonBonesData(const std::string& dragonBonesName, bool dispose = true);
    void addTextureAtlasData(TextureAtlasData* data, const std::string& name = "");
    void removeTextureAtlasData(const std::string& name, bool dispose = true);
    virtual void clear();

    virtual Armature* buildArmature(const std::string& armatureName, const std::string& dragonBonesName = "", const std::string& skinName = "") const;
    virtual bool copyAnimationsToArmature(
        Armature& toArmature, 
        const std::string& fromArmatreName, const std::string& fromSkinName = "", const std::string& fromDragonBonesDataName = "", 
        bool ifRemoveOriginalAnimationList = true
    ) const;

    inline DragonBonesData* getDragonBonesData(const std::string& dragonBonesName) const 
    {
        return mapFind(_dragonBonesDataMap, dragonBonesName);
    }

    inline std::vector<TextureAtlasData*>* getTextureAtlasData(const std::string& name) const
    {
        auto iterator = _textureAtlasDataMap.find(name);
        return (iterator != _textureAtlasDataMap.end()) ? &const_cast<std::vector<TextureAtlasData*>&>(iterator->second) : nullptr;
    }
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_BASE_FACTORY_H
