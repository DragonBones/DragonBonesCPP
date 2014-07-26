#include "BaseFactory.h"

NAME_SPACE_DRAGON_BONES_BEGIN
BaseFactory::BaseFactory() {}
BaseFactory::~BaseFactory()
{
    dispose();
}

const DragonBonesData *BaseFactory::getDragonBonesData(const String &name) const
{
    auto iterator = _dragonBonesDataMap.find(name);
    if (iterator != _dragonBonesDataMap.end())
    {
        return iterator->second;
    }
    else
    {
        return nullptr;
    }
}
void BaseFactory::addDragonBonesData(DragonBonesData *data, const String &name)
{
    if (!data)
    {
        // throw
    }
    const String key = name.empty() ? data->name : name;
    if (key.empty())
    {
        // throw
    }
    if (_dragonBonesDataMap[key])
    {
        // throw
        removeDragonBonesData(key, true);
    }
    _dragonBonesDataMap[key] = data;
}
void BaseFactory::removeDragonBonesData(const String &name, bool disposeData)
{
    auto iterator = _dragonBonesDataMap.find(name);
    if (iterator != _dragonBonesDataMap.end())
    {
        if (disposeData)
        {
            iterator->second->dispose();
            delete iterator->second;
        }
        _dragonBonesDataMap.erase(iterator);
    }
}

const ITextureAtlas *BaseFactory::getTextureAtlas(const String &name) const
{
    auto iterator = _textureAtlasMap.find(name);
    if (iterator != _textureAtlasMap.end())
    {
        return iterator->second;
    }
    else
    {
        return nullptr;
    }
}
void BaseFactory::addTextureAtlas(ITextureAtlas *textureAtlas, const String &name)
{
    if (!textureAtlas)
    {
        // throw
    }
    String key = name.empty() ? textureAtlas->textureAtlasData->name : name;
    if (key.empty())
    {
        // throw
    }
    if (_textureAtlasMap[key])
    {
        // throw
        removeTextureAtlas(key, true);
    }
    _textureAtlasMap[key] = textureAtlas;
}
void BaseFactory::removeTextureAtlas(const String &name, bool disposeData)
{
    auto iterator = _textureAtlasMap.find(name);
    if (iterator != _textureAtlasMap.end())
    {
        if (disposeData)
        {
            iterator->second->dispose();
            delete iterator->second;
        }
        _textureAtlasMap.erase(iterator);
    }
}

void BaseFactory::dispose(const bool disposeData)
{
    if (disposeData)
    {
        for (auto iterator = _dragonBonesDataMap.begin(); iterator != _dragonBonesDataMap.end(); ++iterator)
        {
            iterator->second->dispose();
            delete iterator->second;
        }
        for (auto iterator = _textureAtlasMap.begin(); iterator != _textureAtlasMap.end(); ++iterator)
        {
            iterator->second->dispose();
            delete iterator->second;
        }
    }
    _dragonBonesDataMap.clear();
    _textureAtlasMap.clear();
}

Armature *BaseFactory::buildArmature(const String &armatureName) const
{
    return buildArmature(armatureName, "", armatureName, "", "");
}
Armature *BaseFactory::buildArmature(const String &armatureName, const String &dragonBonesName) const
{
    return buildArmature(armatureName, "", armatureName, dragonBonesName, dragonBonesName);
}
Armature *BaseFactory::buildArmature(const String &armatureName, const String &skinName, const String &animationName, const String &dragonBonesName, const String &textureAtlasName) const
{
    DragonBonesData *dragonBonesData = nullptr;
    ArmatureData *armatureData = nullptr;
    ArmatureData *animationArmatureData = nullptr;
    SkinData *skinData = nullptr;
    SkinData *skinDataCopy = nullptr;
    if (!dragonBonesName.empty())
    {
        auto iterator = _dragonBonesDataMap.find(dragonBonesName);
        if (iterator != _dragonBonesDataMap.end())
        {
            dragonBonesData = iterator->second;
            armatureData = dragonBonesData->getArmatureData(armatureName);
        }
    }
    else
    {
        for (auto iterator = _dragonBonesDataMap.begin(); iterator != _dragonBonesDataMap.end(); ++iterator)
        {
            dragonBonesData = iterator->second;
            armatureData = dragonBonesData->getArmatureData(armatureName);
            if (armatureData)
            {
                break;
            }
        }
    }
    if (!armatureData)
    {
        return nullptr;
    }
    _currentDragonBonesDataName = dragonBonesData->name;
    _currentTextureAtlasName = textureAtlasName.empty() ? _currentDragonBonesDataName : textureAtlasName;
    if (!animationName.empty() && animationName != armatureName)
    {
        animationArmatureData = dragonBonesData->getArmatureData(animationName);
        if (!animationArmatureData)
        {
            for (auto iterator = _dragonBonesDataMap.begin(); iterator != _dragonBonesDataMap.end(); ++iterator)
            {
                dragonBonesData = iterator->second;
                animationArmatureData = dragonBonesData->getArmatureData(animationName);
                if (animationArmatureData)
                {
                    break;
                }
            }
        }
        if (animationArmatureData)
        {
            skinDataCopy = animationArmatureData->getSkinData("");
        }
    }
    skinData = armatureData->getSkinData(skinName);
    Armature *armature = generateArmature(armatureData);
    armature->name = armatureName;
    if (animationArmatureData)
    {
        armature->getAnimation()->animationDataList = animationArmatureData->animationDataList;
    }
    else
    {
        armature->getAnimation()->animationDataList = armatureData->animationDataList;
    }
    //
    buildBones(armature, armatureData);
    //
    if (skinData)
    {
        buildSlots(armature, armatureData, skinData, skinDataCopy);
    }
    //
    //armature->advanceTime(0);
    return armature;
}

void *BaseFactory::getTextureDisplay(const String &textureName, const String &textureAtlasName) const
{
    ITextureAtlas *textureAtlas = nullptr;
    if (!textureAtlasName.empty())
    {
        auto iterator = _textureAtlasMap.find(textureAtlasName);
        if (iterator != _textureAtlasMap.end())
        {
            textureAtlas = iterator->second;
        }
    }
    else
    {
        for (auto iterator = _textureAtlasMap.begin(); iterator != _textureAtlasMap.end(); ++iterator)
        {
            textureAtlas = iterator->second;
            const TextureData *textureData = textureAtlas->textureAtlasData->getTextureData(textureName);
            if (textureData)
            {
                break;
            }
        }
    }
    if (textureAtlas)
    {
        /*
        if(isNaN(pivotX) || isNaN(pivotY))
        {
            var data:SkeletonData = _dataDic[textureAtlasName];
            if(data)
            {
                var pivot:Point = data.getSubTexturePivot(textureName);
                if(pivot)
                {
                    pivotX = pivot.x;
                    pivotY = pivot.y;
                }
            }
        }
        */
        return generateDisplay(textureAtlas, textureName, nullptr);
    }
    return nullptr;
}

void BaseFactory::buildBones(Armature *armature, const ArmatureData *armatureData) const
{
    for (size_t i = 0, l = armatureData->boneDataList.size(); i < l; ++i)
    {
        const BoneData *boneData = armatureData->boneDataList[i];
        Bone *bone = new Bone();
        bone->name = boneData->name;
        bone->inheritRotation = boneData->inheritRotation;
        bone->inheritScale = boneData->inheritScale;
        // copy
        bone->origin = boneData->transform;
        if (armatureData->getBoneData(boneData->parent))
        {
            armature->addBone(bone, boneData->parent);
        }
        else
        {
            armature->addBone(bone);
        }
    }
}

void BaseFactory::buildSlots(Armature *armature, const ArmatureData *armatureData, const SkinData *skinData, const SkinData *skinDataCopy) const
{
    for (size_t i = 0, l = skinData->slotDataList.size(); i < l; ++i)
    {
        SlotData *slotData = skinData->slotDataList[i];
        Bone *bone = armature->getBone(slotData->parent);
        if (!bone || slotData->displayDataList.empty())
        {
            continue;
        }
        Slot *slot = generateSlot(slotData);
        slot->name = slotData->name;
        slot->_originZOrder = slotData->zOrder;
        slot->_slotData = slotData;
        std::vector<std::pair<void *, DisplayType>> displayList;
        for (size_t j = 0, l = slotData->displayDataList.size(); j < l; ++j)
        {
            const DisplayData *displayData = slotData->displayDataList[j];
            switch (displayData->type)
            {
                case DisplayType::DT_ARMATURE:
                {
                    const DisplayData *displayDataCopy = nullptr;
                    if (skinDataCopy)
                    {
                        const SlotData *slotDataCopy = skinDataCopy->getSlotData(slotData->name);
                        if (slotDataCopy)
                        {
                            displayDataCopy = slotDataCopy->displayDataList[i];
                        }
                    }
                    Armature *childArmature = buildArmature(displayData->name, "", displayDataCopy ? displayDataCopy->name : "", _currentDragonBonesDataName, _currentTextureAtlasName);
                    displayList.push_back(std::make_pair(childArmature, DisplayType::DT_ARMATURE));
                }
                break;
                case DisplayType::DT_IMAGE:
                default:
                {
                    const ITextureAtlas *textureAtlas = nullptr;
                    auto iterator = _textureAtlasMap.find(_currentTextureAtlasName);
                    if (iterator != _textureAtlasMap.end())
                    {
                        textureAtlas = iterator->second;
                    }
                    void *display = generateDisplay(textureAtlas, displayData->name, displayData);
                    displayList.push_back(std::make_pair(display, DisplayType::DT_IMAGE));
                }
                break;
            }
        }
        bone->addChild(slot);
        slot->setDisplayList(displayList, false);
    }
}
NAME_SPACE_DRAGON_BONES_END
