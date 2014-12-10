#ifndef DBCC_FACTORY_H
#define DBCC_FACTORY_H

#include "DragonBonesHeaders.h"
#include "DBCCRenderHeaders.h"
#include "cocos2d.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class DBCCFactory : public BaseFactory
{
private:
    static DBCCFactory *_instance;
    
public:

    static DBCCFactory* getInstance();
    static void destroyInstance();
    
    DBCCFactory();
    virtual ~DBCCFactory();

    virtual DBCCArmature* buildArmature(const std::string &armatureName) const override;
    virtual DBCCArmature* buildArmature(const std::string &armatureName, const std::string &dragonBonesName) const override;
    virtual DBCCArmature* buildArmature(const std::string &armatureName, const std::string &skinName, const std::string &animationName, const std::string &dragonBonesName, const std::string &textureAtlasName) const override;
    virtual DBCCArmatureNode* buildArmatureNode(const std::string &armatureName) const;
    virtual DBCCArmatureNode* buildArmatureNode(const std::string &armatureName, const std::string &dragonBonesName) const;
    virtual DBCCArmatureNode* buildArmatureNode(const std::string &armatureName, const std::string &skinName, const std::string &animationName, const std::string &dragonBonesName, const std::string &textureAtlasName) const;
    
    virtual DragonBonesData* loadDragonBonesData(const std::string &dragonBonesFile, const std::string &name = "");
    virtual ITextureAtlas* loadTextureAtlas(const std::string &textureAtlasFile, const std::string &name = "");
    virtual void refreshTextureAtlasTexture(const std::string &name);
    virtual void refreshAllTextureAtlasTexture();
    virtual bool hasDragonBones(const std::string &skeletonName, const std::string &armatureName = "", const std::string &animationName = "");
    
protected:
    virtual DBCCArmature* generateArmature(const ArmatureData *armatureData) const override;
    virtual DBCCSlot* generateSlot(const SlotData *slotData) const override;
    virtual void* generateDisplay(const ITextureAtlas *textureAtlas, const TextureData *textureData, const DisplayData *displayData) const override;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCFactory);
};
NAME_SPACE_DRAGON_BONES_END

#endif  // DBCC_FACTORY_H