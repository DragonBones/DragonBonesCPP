#ifndef __FACTORIES_BASE_FACTORY_H__
#define __FACTORIES_BASE_FACTORY_H__

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
    mutable String _currentDragonBonesDataName;
    mutable String _currentTextureAtlasName;
    std::map<String, DragonBonesData *> _dragonBonesDataMap;
    std::map<String, ITextureAtlas *> _textureAtlasMap;
    
public:
    const std::map<String, DragonBonesData *> &getDragonBonesDataMap() const;
    const std::map<String, ITextureAtlas *> &getTextureAtlasMap() const;
    
public:
    BaseFactory();
    virtual ~BaseFactory();
    virtual void dispose(bool disposeData = true);
    
    virtual DragonBonesData *getDragonBonesData(const String &name) const;
    virtual void addDragonBonesData(DragonBonesData *data, const String &name = "");
    virtual void removeDragonBonesData(const String &name, bool disposeData = true);
    
    virtual ITextureAtlas *getTextureAtlas(const String &name) const;
    virtual void addTextureAtlas(ITextureAtlas *textureAtlas, const String &name = "");
    virtual void removeTextureAtlas(const String &name, bool disposeData = true);
    
    virtual Armature *buildArmature(const String &armatureName) const;
    virtual Armature *buildArmature(const String &armatureName, const String &dragonBonesName) const;
    virtual Armature *buildArmature(const String &armatureName, const String &skinName, const String &animationName, const String &dragonBonesName, const String &textureAtlasName) const;
    virtual void *getTextureDisplay(const String &textureName, const String &textureAtlasName = "", const DisplayData *displayData = nullptr) const;
    
protected:

    virtual void buildBones(Armature *armature, const ArmatureData *armatureData) const;
    virtual void buildSlots(Armature *armature, const ArmatureData *armatureData, const SkinData *skinData, const SkinData *skinDataCopy) const;
    
    virtual Armature *generateArmature(const ArmatureData *armatureData) const = 0;
    virtual Slot *generateSlot(const SlotData *slotData) const = 0;
    virtual void *generateDisplay(const ITextureAtlas *textureAtlas, const TextureData *textureData, const DisplayData *displayData) const = 0;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(BaseFactory);
};
NAME_SPACE_DRAGON_BONES_END
#endif // __FACTORIES_BASE_FACTORY_H__
