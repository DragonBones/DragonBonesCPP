#ifndef DRAGONBONES_CC_FACTORY_H
#define DRAGONBONES_CC_FACTORY_H

#include "dragonBones/DragonBonesHeaders.h"
#include "cocos2d.h"
#include "CCArmatureDisplay.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* Cocos 工厂。
* @version DragonBones 3.0
* @language zh_CN
*/
class CCFactory : public BaseFactory
{
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(CCFactory)

protected:
    static DragonBones* _dragonBonesInstance;
    static CCFactory* _factory;

public:
    /**
    * 一个可以直接使用的全局 WorldClock 实例。(由引擎驱动)
    * @version DragonBones 5.0
    * @language zh_CN
    */
    static WorldClock* getClock() 
    {
        return _dragonBonesInstance->getClock();
    }
    /**
    * 一个可以直接使用的全局工厂实例。
    * @version DragonBones 4.7
    * @language zh_CN
    */
    static CCFactory* getFactory()
    {
        if (CCFactory::_factory == nullptr) 
        {
            CCFactory::_factory = new CCFactory();
        }

        return CCFactory::_factory;
    }

protected:
    std::string _prevPath;

public:
    /**
    * @inheritDoc
    */
    CCFactory() :
        _prevPath()
    {
        if (_dragonBonesInstance == nullptr)
        {
            const auto eventManager = CCArmatureDisplay::create();
            eventManager->retain();

            _dragonBonesInstance = new DragonBones(eventManager);
            _dragonBonesInstance->yDown = false;

            cocos2d::Director::getInstance()->getScheduler()->schedule(
                [&](float passedTime)
                {
                    _dragonBonesInstance->advanceTime(passedTime);
                },
                this, 0.0f, false, "dragonBonesClock"
            );
        }

        _dragonBones = _dragonBonesInstance;
    }
    virtual ~CCFactory() 
    {
        clear();
    }

protected:
    virtual TextureAtlasData* _buildTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const override;
    virtual Armature* _buildArmature(const BuildArmaturePackage& dataPackage) const override;
    virtual Slot* _buildSlot(const BuildArmaturePackage& dataPackage, SlotData* slotData, std::vector<DisplayData*>* displays, Armature& armature) const override;

public:
    /**
    * 加载并解析 Cocos 本地龙骨数据。
    * @param filePath 文件路径。
    * @param name 为数据提供一个名称，以便可以通过这个名称获取数据，如果未设置，则使用数据中的名称。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    virtual DragonBonesData* loadDragonBonesData(const std::string& filePath, const std::string& name = "");
    /**
    * 加载并解析 Cocos 本地贴图数据。
    * @param filePath 文件路径。
    * @param name 为数据提供一个名称，以便可以通过这个名称获取数据，如果未设置，则使用数据中的名称。
    * @param scale 为贴图集设置一个缩放值。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    virtual TextureAtlasData* loadTextureAtlasData(const std::string& filePath, const std::string& name = "", float scale = 0.0f);
    /**
    * 创建一个指定名称的骨架。
    * @param armatureName 骨架名称。
    * @param dragonBonesName 龙骨数据名称，如果未设置，将检索所有的龙骨数据，如果多个数据中包含同名的骨架数据，可能无法创建出准确的骨架。
    * @param skinName 皮肤名称，如果未设置，则使用默认皮肤。
    * @param textureAtlasName 贴图集数据名称，如果未设置，则使用龙骨数据。
    * @returns 骨架的显示容器。
    * @see dragonBones.CCArmatureDisplay
    * @version DragonBones 4.5
    * @language zh_CN
    */
    virtual CCArmatureDisplay* buildArmatureDisplay(const std::string& armatureName, const std::string& dragonBonesName = "", const std::string& skinName = "", const std::string& textureAtlasName = "") const;
    /**
    * 获取带有指定贴图的显示对象。
    * @param textureName 指定的贴图名称。
    * @param textureAtlasName 指定的贴图集数据名称，如果未设置，将检索所有的贴图集数据。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    virtual cocos2d::Sprite* getTextureDisplay(const std::string& textureName, const std::string& dragonBonesName = "") const;
    /**
    * 获取全局声音事件管理器。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    virtual CCArmatureDisplay* getSoundEventManager() const
    {
        return dynamic_cast<CCArmatureDisplay*>(static_cast<IArmatureProxy*>(_dragonBones->getEventManager()));
    }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_CC_FACTORY_H
