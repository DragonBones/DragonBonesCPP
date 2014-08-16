#ifndef __DBCC_FACTORY_H__
#define __DBCC_FACTORY_H__

#include "DragonBonesHeaders.h"
#include "cocos2d.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class DBCCFactory : public BaseFactory
{
public:
    static DBCCFactory factory;
    
public:
    static DBCCFactory* getInstance();
    
    DBCCFactory();
    virtual ~DBCCFactory();
    
    virtual DragonBonesData* loadDragonBonesData(const std::string &dragonBonesFile, const std::string &name = "");
    virtual ITextureAtlas* loadTextureAtlas(const std::string &textureAtlasFile, const std::string &name = "");
    virtual void refreshTextureAtlasTexture(const std::string &name);
    virtual void refreshAllTextureAtlasTexture();
    virtual bool hasDragonBones(const std::string &skeletonName, const std::string &armatureName = "", const std::string &animationName = "");
    
protected:
    virtual Armature* generateArmature(const ArmatureData *armatureData) const override;
    virtual Slot* generateSlot(const SlotData *slotData) const override;
    virtual void* generateDisplay(const ITextureAtlas *textureAtlas, const TextureData *textureData, const DisplayData *displayData) const override;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCFactory);
};
NAME_SPACE_DRAGON_BONES_END

#endif  // __DBCC_FACTORY_H__