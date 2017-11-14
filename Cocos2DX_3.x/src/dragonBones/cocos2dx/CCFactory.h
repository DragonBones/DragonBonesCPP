#ifndef DRAGONBONES_CC_FACTORY_H
#define DRAGONBONES_CC_FACTORY_H

#include "dragonBones/DragonBonesHeaders.h"
#include "cocos2d.h"
#include "CCArmatureDisplay.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
 * The Cocos2d factory.
 * @version DragonBones 3.0
 * @language en_US
 */
/**
 * Cocos2d 工厂。
 * @version DragonBones 3.0
 * @language zh_CN
 */
class CCFactory : public BaseFactory
{
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(CCFactory)

private:
    static DragonBones* _dragonBonesInstance;
    static CCFactory* _factory;

public:
    /**
     * A global factory instance that can be used directly.
     * @version DragonBones 4.7
     * @language en_US
     */
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
    virtual Slot* _buildSlot(const BuildArmaturePackage& dataPackage, SlotData* slotData, std::vector<DisplayData*>* displays, Armature* armature) const override;

public:
    virtual DragonBonesData* loadDragonBonesData(const std::string& filePath, const std::string& name = "", float scale = 1.0f);

    virtual TextureAtlasData* loadTextureAtlasData(const std::string& filePath, const std::string& name = "", float scale = 1.0f);

    virtual CCArmatureDisplay* buildArmatureDisplay(const std::string& armatureName, const std::string& dragonBonesName = "", const std::string& skinName = "", const std::string& textureAtlasName = "") const;

    virtual cocos2d::Sprite* getTextureDisplay(const std::string& textureName, const std::string& dragonBonesName = "") const;

    virtual CCArmatureDisplay* getSoundEventManager() const
    {
        return dynamic_cast<CCArmatureDisplay*>(static_cast<IArmatureProxy*>(_dragonBones->getEventManager()));
    }

    /**
     * Deprecated, please refer to {@link #clock}.
     * @deprecated
     * @language en_US
     */
    /**
     * 已废弃，请参考 {@link #clock}。
     * @deprecated
     * @language zh_CN
     */
    static WorldClock* getClock()
    {
        return _dragonBonesInstance->getClock();
    }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_CC_FACTORY_H
