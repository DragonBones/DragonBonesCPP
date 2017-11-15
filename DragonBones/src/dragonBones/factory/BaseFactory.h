#ifndef DRAGONBONES_BASE_FACTORY_H
#define DRAGONBONES_BASE_FACTORY_H

#include "../parser/JSONDataParser.h"
#include "../parser/BinaryDataParser.h"
#include "../armature/Armature.h"
#include "../armature/Bone.h"
#include "../armature/Slot.h"
#include "../armature/Constraint.h"
#include "../animation/Animation.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* 创建骨架的基础工厂。 (通常只需要一个全局工厂实例)
* @see dragonBones.DragonBonesData
* @see dragonBones.TextureAtlasData
* @see dragonBones.ArmatureData
* @see dragonBones.Armature
* @version DragonBones 3.0
* @language zh_CN
*/
class BaseFactory
{
protected:
    static JSONDataParser _jsonParser;
    static BinaryDataParser _binaryParser;

public:
    /**
    * 是否开启共享搜索。
    * 如果开启，创建一个骨架时，可以从多个龙骨数据中寻找骨架数据，或贴图集数据中寻找贴图数据。 (通常在有共享导出的数据时开启)
    * @see dragonBones.DragonBonesData#autoSearch
    * @see dragonBones.TextureAtlasData#autoSearch
    * @version DragonBones 4.5
    * @language zh_CN
    */
    bool autoSearch;

protected:
    std::map<std::string, DragonBonesData*> _dragonBonesDataMap;
    std::map<std::string, std::vector<TextureAtlasData*>> _textureAtlasDataMap;
    DragonBones* _dragonBones;
    DataParser* _dataParser;

public:
    /**
    * 创建一个工厂。 (通常只需要一个全局工厂实例)
    * @param dataParser 龙骨数据解析器，如果不设置，则使用默认解析器。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    BaseFactory(DataParser* dataParser = nullptr) :
        autoSearch(false),
        _dragonBonesDataMap(),
        _textureAtlasDataMap(),
        _dragonBones(nullptr),
        _dataParser(nullptr)
    {
        _dataParser = dataParser != nullptr ? dataParser : &BaseFactory::_jsonParser;
    }
    virtual ~BaseFactory()
    {
        clear();

        _dragonBones = nullptr;
        _dataParser = nullptr;
    }

protected:
    virtual inline bool _isSupportMesh() const
    {
        return true;
    }
    virtual TextureData* _getTextureData(const std::string& textureAtlasName, const std::string& textureName) const;
    virtual bool _fillBuildArmaturePackage(
        BuildArmaturePackage& dataPackage,
        const std::string& dragonBonesName, const std::string& armatureName, const std::string& skinName, const std::string& textureAtlasName
    ) const;
    virtual void _buildBones(const BuildArmaturePackage& dataPackage, Armature* armature) const;
    virtual void _buildSlots(const BuildArmaturePackage& dataPackage, Armature* armature) const;
    virtual Armature* _buildChildArmature(const BuildArmaturePackage* dataPackage, Slot* slot, DisplayData* displayData) const;
    virtual std::pair<void*, DisplayType> _getSlotDisplay(const BuildArmaturePackage* dataPackage, DisplayData* displayData, DisplayData* rawDisplayData, Slot* slot) const;
    virtual TextureAtlasData* _buildTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const = 0;
    virtual Armature* _buildArmature(const BuildArmaturePackage& dataPackage) const = 0;
    virtual Slot* _buildSlot(const BuildArmaturePackage& dataPackage, SlotData* slotData, std::vector<DisplayData*>* displays, Armature* armature) const = 0;

public:
    /**
    * 解析并添加龙骨数据。
    * @param rawData 需要解析的原始数据。
    * @param name 为数据提供一个名称，以便可以通过这个名称获取数据，如果未设置，则使用数据中的名称。
    * @returns DragonBonesData
    * @see #getDragonBonesData()
    * @see #addDragonBonesData()
    * @see #removeDragonBonesData()
    * @see dragonBones.DragonBonesData
    * @version DragonBones 4.5
    * @language zh_CN
    */
    virtual DragonBonesData* parseDragonBonesData(const char* rawData, const std::string& name = "", float scale = 1.0f);
    /**
    * 解析并添加贴图集数据。
    * @param rawData 需要解析的原始数据。
    * @param textureAtlas 贴图。
    * @param name 为数据指定一个名称，以便可以通过这个名称获取数据，如果未设置，则使用数据中的名称。
    * @param scale 为贴图集设置一个缩放值。
    * @returns 贴图集数据
    * @see #getTextureAtlasData()
    * @see #addTextureAtlasData()
    * @see #removeTextureAtlasData()
    * @see dragonBones.TextureAtlasData
    * @version DragonBones 4.5
    * @language zh_CN
    */
    virtual TextureAtlasData* parseTextureAtlasData(const char* rawData, void* textureAtlas, const std::string& name = "", float scale = 1.0f);
    /**
    * 获取指定名称的龙骨数据。
    * @param name 数据名称。
    * @returns DragonBonesData
    * @see #parseDragonBonesData()
    * @see #addDragonBonesData()
    * @see #removeDragonBonesData()
    * @see dragonBones.DragonBonesData
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline DragonBonesData* getDragonBonesData(const std::string& name) const
    {
        return mapFind(_dragonBonesDataMap, name);
    }
    /**
    * 添加龙骨数据。
    * @param data 龙骨数据。
    * @param name 为数据指定一个名称，以便可以通过这个名称获取数据，如果未设置，则使用数据中的名称。
    * @see #parseDragonBonesData()
    * @see #getDragonBonesData()
    * @see #removeDragonBonesData()
    * @see dragonBones.DragonBonesData
    * @version DragonBones 3.0
    * @language zh_CN
    */
    virtual void addDragonBonesData(DragonBonesData* data, const std::string& name = "");
    /**
    * 移除龙骨数据。
    * @param name 数据名称。
    * @param disposeData 是否释放数据。
    * @see #parseDragonBonesData()
    * @see #getDragonBonesData()
    * @see #addDragonBonesData()
    * @see dragonBones.DragonBonesData
    * @version DragonBones 3.0
    * @language zh_CN
    */
    virtual void removeDragonBonesData(const std::string& name, bool disposeData = true);
    /**
    * 获取指定名称的贴图集数据列表。
    * @param name 数据名称。
    * @returns 贴图集数据列表。
    * @see #parseTextureAtlasData()
    * @see #addTextureAtlasData()
    * @see #removeTextureAtlasData()
    * @see dragonBones.TextureAtlasData
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline std::vector<TextureAtlasData*>* getTextureAtlasData(const std::string& name)
    {
        return mapFindB(_textureAtlasDataMap, name);
    }
    /**
    * 添加贴图集数据。
    * @param data 贴图集数据。
    * @param name 为数据指定一个名称，以便可以通过这个名称获取数据，如果未设置，则使用数据中的名称。
    * @see #parseTextureAtlasData()
    * @see #getTextureAtlasData()
    * @see #removeTextureAtlasData()
    * @see dragonBones.TextureAtlasData
    * @version DragonBones 3.0
    * @language zh_CN
    */
    virtual void addTextureAtlasData(TextureAtlasData* data, const std::string& name = "");
    /**
    * 移除贴图集数据。
    * @param name 数据名称。
    * @param disposeData 是否释放数据。
    * @see #parseTextureAtlasData()
    * @see #getTextureAtlasData()
    * @see #addTextureAtlasData()
    * @see dragonBones.TextureAtlasData
    * @version DragonBones 3.0
    * @language zh_CN
    */
    virtual void removeTextureAtlasData(const std::string& name, bool disposeData = true);
    /**
    * 获取骨架数据。
    * @param name 骨架数据名称。
    * @param dragonBonesName 龙骨数据名称。
    * @see dragonBones.ArmatureData
    * @version DragonBones 5.1
    * @language zh_CN
    */
    virtual ArmatureData* getArmatureData(const std::string& name, const std::string& dragonBonesName = "") const;
    /**
    * 清除所有的数据。
    * @param disposeData 是否释放数据。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    virtual void clear(bool disposeData = true);
    /**
    * 创建一个骨架。
    * @param armatureName 骨架数据名称。
    * @param dragonBonesName 龙骨数据名称，如果未设置，将检索所有的龙骨数据，当多个龙骨数据中包含同名的骨架数据时，可能无法创建出准确的骨架。
    * @param skinName 皮肤名称，如果未设置，则使用默认皮肤。
    * @param textureAtlasName 贴图集数据名称，如果未设置，则使用龙骨数据名称。
    * @returns 骨架
    * @see dragonBones.ArmatureData
    * @see dragonBones.Armature
    * @version DragonBones 3.0
    * @language zh_CN
    */
    virtual Armature* buildArmature(const std::string& armatureName, const std::string& dragonBonesName = "", const std::string& skinName = "", const std::string & textureAtlasName = "") const;
    virtual void replaceDisplay(Slot* slot, DisplayData* displayData, int displayIndex) const;
    virtual bool replaceSlotDisplay(
        const std::string& dragonBonesName, const std::string& armatureName, const std::string& slotName, const std::string& displayName,
        Slot* slot, int displayIndex = -1
    ) const;
    virtual bool replaceSlotDisplayList(
        const std::string& dragonBonesName, const std::string& armatureName, const std::string& slotName,
        Slot* slot
    ) const;
    virtual bool replaceSkin(Armature* armature, SkinData* skin, const std::vector<std::string>* exclude = nullptr) const;
    virtual bool replaceAnimation(Armature* armature, ArmatureData* armatureData, bool isReplaceAll = true) const;
    inline const std::map<std::string, std::vector<TextureAtlasData*>>& getAllTextureAtlasData() const
    {
        return _textureAtlasDataMap;
    }
    inline const std::map<std::string, DragonBonesData*>& getAllDragonBonesData() const
    {
        return _dragonBonesDataMap;
    }
    inline WorldClock* getClock() const 
    {
        return _dragonBones->getClock();
    }

    inline bool changeSkin(Armature* armature, SkinData* skin, const std::vector<std::string>* exclude = nullptr) const
    {
        return replaceSkin(armature, skin, exclude);
    }
};
/**
* @private
*/
class BuildArmaturePackage
{
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(BuildArmaturePackage)

public:
    std::string dataName;
    std::string textureAtlasName;
    DragonBonesData* data;
    ArmatureData* armature;
    SkinData* skin;

    BuildArmaturePackage() :
        dataName(),
        textureAtlasName(),
        data(nullptr),
        armature(nullptr),
        skin(nullptr)
    {}
    ~BuildArmaturePackage() {}
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_BASE_FACTORY_H
