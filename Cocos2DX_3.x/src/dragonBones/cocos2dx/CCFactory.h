#ifndef DRAGONBONES_CC_FACTORY_H
#define DRAGONBONES_CC_FACTORY_H

#include "dragonBones/DragonBonesHeaders.h"
#include "cocos2d.h"
#include "CCArmatureDisplay.h"

DRAGONBONES_NAMESPACE_BEGIN

class CCFactory : public BaseFactory
{
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(CCFactory)

protected:
    static DragonBones* _dragonBonesInstance;

public:
    static CCFactory factory;
    static WorldClock* getClock() 
    {
        return _dragonBonesInstance->getClock();
    }

protected:
    std::string _prevPath;

public:
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
    ~CCFactory() 
    {
    }

protected:
    virtual TextureAtlasData* _buildTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const override;
    virtual Armature* _buildArmature(const BuildArmaturePackage& dataPackage) const override;
    virtual Slot* _buildSlot(const BuildArmaturePackage& dataPackage, SlotData* slotData, std::vector<DisplayData*>* displays, Armature& armature) const override;

public:
    virtual DragonBonesData* loadDragonBonesData(const std::string& filePath, const std::string& name = "");
    virtual TextureAtlasData* loadTextureAtlasData(const std::string& filePath, const std::string& name = "", float scale = 0.0f);
    virtual CCArmatureDisplay* buildArmatureDisplay(const std::string& armatureName, const std::string& dragonBonesName = "", const std::string& skinName = "", const std::string& textureAtlasName = "") const;
    virtual cocos2d::Sprite* getTextureDisplay(const std::string& textureName, const std::string& dragonBonesName = "") const;

    virtual CCArmatureDisplay* getSoundEventManager() const
    {
        return dynamic_cast<CCArmatureDisplay*>(static_cast<IArmatureProxy*>(_dragonBones->getEventManager()));
    }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_CC_FACTORY_H
