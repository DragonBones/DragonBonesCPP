#ifndef DRAGONBONES_CC_FACTORY_H
#define DRAGONBONES_CC_FACTORY_H

#include "DragonBonesHeaders.h"
#include "cocos2d.h"

NAMESPACE_DRAGONBONES_BEGIN

class CCFactory final : public BaseFactory
{
public:
    CCFactory();
    ~CCFactory();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(CCFactory);

protected:
    virtual TextureAtlasData* _generateTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const override;
    virtual Armature* _generateArmature(const BuildArmaturePackage& dataPackage) const override;
    virtual Slot* _generateSlot(const BuildArmaturePackage& dataPackage, const SlotDisplayDataSet& slotDisplayDataSet) const override;
    virtual void* _generateDisplay(const BuildArmaturePackage& dataPackage, const DisplayData& displayData, const DisplayData& rawDisplayData) const override;

public:
    virtual DragonBonesData* loadDragonBonesData(const std::string& filePath, const std::string& dragonBonesName = "");
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_CC_FACTORY_H
