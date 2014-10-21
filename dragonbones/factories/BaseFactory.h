#ifndef FACTORIES_BASE_FACTORY_H
#define FACTORIES_BASE_FACTORY_H

#include "../DragonBones.h"
#include "../objects/ArmatureData.h"
#include "../objects/DragonBonesData.h"
#include "../textures/ITextureAtlas.h"
#include "../core/Slot.h"
#include "../core/Bone.h"
#include "../core/Armature.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class BaseFactory
{
protected:
    enum class AutoSearchType {AST_ALL, AST_AUTO, AST_NONE};
    
public:
    bool autoSearchDragonBonesData;
    bool autoSearchTexture;
    
protected:
    mutable std::string _currentDragonBonesDataName;
    mutable std::string _currentTextureAtlasName;
    std::map<std::string, DragonBonesData*> _dragonBonesDataMap;
    std::map<std::string, ITextureAtlas*> _textureAtlasMap;
    
public:
    const std::map<std::string, DragonBonesData*>& getDragonBonesDataMap() const;
    const std::map<std::string, ITextureAtlas*>& getTextureAtlasMap() const;
    
public:
    BaseFactory();
    virtual ~BaseFactory();
    virtual void dispose(bool disposeData = true);
    
    virtual DragonBonesData* getDragonBonesData(const std::string &name) const;
    virtual void addDragonBonesData(DragonBonesData *data, const std::string &name = "");
    virtual void removeDragonBonesData(const std::string &name, bool disposeData = true);
    
    virtual ITextureAtlas* getTextureAtlas(const std::string &name) const;
    virtual void addTextureAtlas(ITextureAtlas *textureAtlas, const std::string &name = "");
    virtual void removeTextureAtlas(const std::string &name, bool disposeData = true);
    
    virtual Armature* buildArmature(const std::string &armatureName) const;
    virtual Armature* buildArmature(const std::string &armatureName, const std::string &dragonBonesName) const;
    virtual Armature* buildArmature(const std::string &armatureName, const std::string &skinName, const std::string &animationName, const std::string &dragonBonesName, const std::string &textureAtlasName) const;
    virtual void* getTextureDisplay(const std::string &textureName, const std::string &textureAtlasName = "", const DisplayData *displayData = nullptr) const;
    
protected:

    virtual void buildBones(Armature *armature, const ArmatureData *armatureData) const;
    virtual void buildSlots(Armature *armature, const ArmatureData *armatureData, const SkinData *skinData, const SkinData *skinDataCopy) const;
    
    virtual Armature* generateArmature(const ArmatureData *armatureData) const = 0;
    virtual Slot* generateSlot(const SlotData *slotData) const = 0;
    virtual void* generateDisplay(const ITextureAtlas *textureAtlas, const TextureData *textureData, const DisplayData *displayData) const = 0;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(BaseFactory);
};
NAME_SPACE_DRAGON_BONES_END
#endif // FACTORIES_BASE_FACTORY_H
