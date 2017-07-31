#include "BaseFactory.h"

DRAGONBONES_NAMESPACE_BEGIN

JSONDataParser BaseFactory::_jsonParser;
BinaryDataParser BaseFactory::_binaryParser;

TextureData* BaseFactory::_getTextureData(const std::string& textureAtlasName, const std::string& textureName) const
{
    const auto iterator = _textureAtlasDataMap.find(textureAtlasName);
    if (iterator != _textureAtlasDataMap.end())
    {
        for (const auto textureAtlasData : iterator->second)
        {
            const auto textureData = textureAtlasData->getTexture(textureName);
            if (textureData != nullptr)
            {
                return textureData;
            }
        }
    }

    if (autoSearch)
    {
        for (const auto& pair : _textureAtlasDataMap)
        {
            for (const auto textureAtlasData : pair.second)
            {
                if (textureAtlasData->autoSearch)
                {
                    const auto textureData = textureAtlasData->getTexture(textureName);
                    if (textureData != nullptr)
                    {
                        return textureData;
                    }
                }
            }
        }
    }

    return nullptr;
}

bool BaseFactory::_fillBuildArmaturePackage(
    BuildArmaturePackage& dataPackage, 
    const std::string& dragonBonesName, const std::string& armatureName, const std::string& skinName, const std::string& textureAtlasName
) const
{
    std::string mapName = dragonBonesName;
    DragonBonesData* dragonBonesData = nullptr;
    ArmatureData* armatureData = nullptr;

    if (!mapName.empty())
    {
        const auto iterator = _dragonBonesDataMap.find(mapName);
        if (iterator != _dragonBonesDataMap.end())
        {
            dragonBonesData = iterator->second;
            armatureData = dragonBonesData->getArmature(armatureName);
        }
    }

    if (armatureData == nullptr && (mapName.empty() || autoSearch))
    {
        for (const auto& pair : _dragonBonesDataMap)
        {
            dragonBonesData = pair.second;
            if (mapName.empty() || dragonBonesData->autoSearch)
            {
                armatureData = dragonBonesData->getArmature(armatureName);
                if (armatureData != nullptr)
                {
                    mapName = pair.first;
                    break;
                }
            }
        }
    }

    if (armatureData != nullptr)
    {
        dataPackage.dataName = mapName;
        dataPackage.textureAtlasName = textureAtlasName;
        dataPackage.data = dragonBonesData;
        dataPackage.armature = armatureData;
        dataPackage.skin = nullptr;

        if (!skinName.empty()) 
        {
            dataPackage.skin = armatureData->getSkin(skinName);
            if (dataPackage.skin == nullptr && autoSearch)
            {
                for (const auto& pair : _dragonBonesDataMap)
                {
                    const auto skinDragonBonesData = pair.second;
                    const auto skinArmatureData = skinDragonBonesData->getArmature(skinName);
                    if (skinArmatureData != nullptr)
                    {
                        dataPackage.skin = skinArmatureData->defaultSkin;
                        break;
                    }
                }
            }
        }
            
        if (dataPackage.skin == nullptr)
        {
            dataPackage.skin = armatureData->defaultSkin;
        }

        return true;
    }

    return false;
}

void BaseFactory::_buildBones(const BuildArmaturePackage& dataPackage, Armature& armature) const
{
    for (const auto boneData : dataPackage.armature->sortedBones)
    {
        const auto bone = BaseObject::borrowObject<Bone>();
        bone->init(boneData);

        if (boneData->parent != nullptr)
        {
            armature.addBone(bone, boneData->parent->name);
        }
        else
        {
            armature.addBone(bone);
        }

        for (const auto constraintData : boneData->constraints)
        {
            const auto target = armature.getBone(constraintData->target->name);
            if (target == nullptr) 
            {
                continue;
            }

            // TODO more constraint type.
            const auto ikConstraintData = static_cast<IKConstraintData*>(constraintData);
            const auto constraint = BaseObject::borrowObject<IKConstraint>();
            const auto root = ikConstraintData->root != nullptr ? armature.getBone(ikConstraintData->root->name) : nullptr;
            constraint->target = target;
            constraint->bone = bone;
            constraint->root = root;
            constraint->bendPositive = ikConstraintData->bendPositive;
            constraint->scaleEnabled = ikConstraintData->scaleEnabled;
            constraint->weight = ikConstraintData->weight;

            if (root != nullptr) 
            {
                root->addConstraint(constraint);
            }
            else 
            {
                bone->addConstraint(constraint);
            }
        }
    }
}

void BaseFactory::_buildSlots(const BuildArmaturePackage& dataPackage, Armature& armature) const
{
    const auto currentSkin = dataPackage.skin;
    const auto defaultSkin = dataPackage.armature->defaultSkin;
    if (currentSkin == nullptr || defaultSkin == nullptr) 
    {
        return;
    }

    std::map<std::string, std::vector<DisplayData*>*> skinSlots;
    for (auto& pair : defaultSkin->displays) 
    {
        auto& displays = pair.second;
        skinSlots[pair.first] = &displays;
    }

    if (currentSkin != defaultSkin) 
    {
        for (auto& pair : currentSkin->displays) 
        {
            auto& displays = pair.second;
            skinSlots[pair.first] = &displays;
        }
    }

    for (const auto slotData : dataPackage.armature->sortedSlots) 
    {
        if (skinSlots.find(slotData->name) == skinSlots.cend())
        {
            continue;
        }

        const auto displays = skinSlots[slotData->name];
        const auto slot = _buildSlot(dataPackage, slotData, displays, armature);
        std::vector<std::pair<void*, DisplayType>> displayList;
        for (const auto displayData : *displays)
        {
            if (displayData != nullptr) 
            {
                displayList.push_back(_getSlotDisplay(&dataPackage, *displayData, nullptr, *slot));
            }
            else 
            {
                displayList.push_back(std::make_pair(nullptr, DisplayType::Image));
            }
        }

        armature.addSlot(slot, slotData->parent->name);
        slot->_setDisplayList(displayList);
        slot->_setDisplayIndex(slotData->displayIndex, true);
    }
}

std::pair<void*, DisplayType> BaseFactory::_getSlotDisplay(const BuildArmaturePackage* dataPackage, DisplayData& displayData, DisplayData* rawDisplayData, const Slot& slot) const
{
    const auto& dataName = dataPackage != nullptr ? dataPackage->dataName : displayData.parent->parent->name;
    std::pair<void*, DisplayType> display(nullptr, DisplayType::Image);
    switch (displayData.type)
    {
        case DisplayType::Image:
        {
            auto& imageDisplayData = static_cast<ImageDisplayData&>(displayData);
            if (imageDisplayData.texture == nullptr)
            {
                imageDisplayData.texture = _getTextureData(dataName, displayData.path);
            }
            else if (dataPackage != nullptr && !dataPackage->textureAtlasName.empty())
            {
                imageDisplayData.texture = _getTextureData(dataPackage->textureAtlasName, displayData.path);
            }

            display.first = slot._rawDisplay;
            display.second = DisplayType::Image;
            break;
        }

        case DisplayType::Mesh:
        {
            auto& meshDisplayData = static_cast<MeshDisplayData&>(displayData);
            if (meshDisplayData.texture == nullptr)
            {
                meshDisplayData.texture = _getTextureData(dataName, meshDisplayData.path);
            }
            else if (dataPackage != nullptr && !dataPackage->textureAtlasName.empty())
            {
                meshDisplayData.texture = _getTextureData(dataPackage->textureAtlasName, meshDisplayData.path);
            }

            if (_isSupportMesh())
            {
                display.first = slot._meshDisplay;
                display.second = DisplayType::Mesh;
            }
            else
            {
                display.first = slot._rawDisplay;
                display.second = DisplayType::Image;
            }
            break;
        }

        case DisplayType::Armature:
        {
            auto& armatureDisplayData = static_cast<ArmatureDisplayData&>(displayData);
            const auto childArmature = buildArmature(armatureDisplayData.name, dataName, "", dataPackage != nullptr ? dataPackage->textureAtlasName : "");
            if (childArmature != nullptr)
            {
                childArmature->inheritAnimation = armatureDisplayData.inheritAnimation;
                if (!childArmature->inheritAnimation)
                {
                    const auto actions = !armatureDisplayData.actions.empty() ? &armatureDisplayData.actions : &childArmature->armatureData->defaultActions;
                    if (!actions->empty())
                    {
                        for (const auto action : *actions)
                        {
                            childArmature->getAnimation()->fadeIn(action->name);
                        }
                    }
                    else {
                        childArmature->getAnimation()->play();
                    }
                }

                armatureDisplayData.armature = childArmature->armatureData; //
            }

            display.first = childArmature;
            display.second = DisplayType::Armature;
            break;
        }
    }

    return display;
}

void BaseFactory::_replaceSlotDisplay(const BuildArmaturePackage& dataPackage, DisplayData* displayData, Slot& slot, int displayIndex) const
{
    if (displayIndex < 0)
    {
        displayIndex = slot.getDisplayIndex();
    }

    if (displayIndex < 0)
    {
        displayIndex = 0;
    }

    auto displayList = slot.getDisplayList(); // Copy.
    if (displayList.size() <= (unsigned)displayIndex)
    {
        displayList.resize(displayIndex + 1, std::make_pair(nullptr, DisplayType::Image));
    }

    if (slot._displayDatas.size() <= (unsigned)displayIndex)
    {
        slot._displayDatas.resize(displayIndex + 1, nullptr);
    }

    slot._displayDatas[displayIndex] = displayData;
    if (displayData != nullptr) 
    {
        displayList[displayIndex] = _getSlotDisplay(
            &dataPackage, 
            *displayData, 
            ((unsigned)displayIndex < slot._rawDisplayDatas->size()) ?(*slot._rawDisplayDatas)[displayIndex] : nullptr,
            slot
        );
    }
    else 
    {
        displayList[displayIndex] = std::make_pair(nullptr, DisplayType::Image);
    }

    slot.setDisplayList(displayList);
}

DragonBonesData* BaseFactory::parseDragonBonesData(const char* rawData, const std::string& name, float scale)
{
    DRAGONBONES_ASSERT(rawData != nullptr, "");

    DragonBonesData* dragonBonesData = nullptr;

    if (
        rawData[0] == 'D' &&
        rawData[1] == 'B' &&
        rawData[2] == 'D' &&
        rawData[3] == 'T'
    )
    {
        dragonBonesData = BaseFactory::_binaryParser.parseDragonBonesData(rawData, scale);
    }
    else 
    {
        dragonBonesData = _dataParser->parseDragonBonesData(rawData, scale);
    }

    while (true) 
    {
        const auto textureAtlasData = _buildTextureAtlasData(nullptr, nullptr);
        if (_dataParser->parseTextureAtlasData(nullptr, *textureAtlasData, scale))
        {
            addTextureAtlasData(textureAtlasData, name);
        }
        else 
        {
            textureAtlasData->returnToPool();
            break;
        }
    }
        
    if (dragonBonesData != nullptr)
    {
        addDragonBonesData(dragonBonesData, name);
    }

    return dragonBonesData;
}

TextureAtlasData* BaseFactory::parseTextureAtlasData(const char* rawData, void* textureAtlas, const std::string& name, float scale)
{
    const auto textureAtlasData = _buildTextureAtlasData(nullptr, nullptr);
    _dataParser->parseTextureAtlasData(rawData, *textureAtlasData, scale);
    _buildTextureAtlasData(textureAtlasData, textureAtlas);
    addTextureAtlasData(textureAtlasData, name);

    return textureAtlasData;
}

void BaseFactory::addDragonBonesData(DragonBonesData* data, const std::string& name)
{
    const auto& mapName = !name.empty()? name : data->name;
    if (_dragonBonesDataMap.find(mapName) != _dragonBonesDataMap.cend())
    {
        if (_dragonBonesDataMap[name] == data) 
        {
            return;
        }

        DRAGONBONES_ASSERT(false, "Replace data: " + name);
        _dragonBonesDataMap[name]->returnToPool();
    }

    _dragonBonesDataMap[mapName] = data;
}

void BaseFactory::removeDragonBonesData(const std::string& name, bool disposeData)
{
    const auto iterator = _dragonBonesDataMap.find(name);
    if (iterator != _dragonBonesDataMap.cend())
    {
        if (disposeData)
        {
            iterator->second->returnToPool();
        }

        _dragonBonesDataMap.erase(iterator);
    }
}

void BaseFactory::addTextureAtlasData(TextureAtlasData* data, const std::string& name)
{
    const auto& mapName = !name.empty() ? name : data->name; 
    auto& textureAtlasList = _textureAtlasDataMap[mapName];
    if (std::find(textureAtlasList.cbegin(), textureAtlasList.cend(), data) == textureAtlasList.cend())
    {
        textureAtlasList.push_back(data);
    }
}

void BaseFactory::removeTextureAtlasData(const std::string& name, bool disposeData)
{
    const auto iterator = _textureAtlasDataMap.find(name);
    if (iterator != _textureAtlasDataMap.end())
    {
        if (disposeData)
        {
            for (const auto textureAtlasData : iterator->second)
            {
                textureAtlasData->returnToPool();
            }
        }

        _textureAtlasDataMap.erase(iterator);
    }
}

ArmatureData* BaseFactory::getArmatureData(const std::string& name, const std::string& dragonBonesName) const
{
    BuildArmaturePackage dataPackage;
    if (!_fillBuildArmaturePackage(dataPackage, dragonBonesName, name, "", "")) 
    {
        return nullptr;
    }

    return dataPackage.armature;
}

void BaseFactory::clear(bool disposeData)
{
    if (disposeData)
    {
        for (const auto& pair : _dragonBonesDataMap)
        {
            pair.second->returnToPool();
        }

        for (const auto& pair : _textureAtlasDataMap)
        {
            for (const auto textureAtlasData : pair.second)
            {
                textureAtlasData->returnToPool();
            }
        }
    }

    _dragonBonesDataMap.clear();
    _textureAtlasDataMap.clear();
}

Armature * BaseFactory::buildArmature(const std::string& armatureName, const std::string& dragonBonesName, const std::string& skinName, const std::string& textureAtlasName) const
{
    BuildArmaturePackage dataPackage;
    if (!_fillBuildArmaturePackage(dataPackage, dragonBonesName, armatureName, skinName, textureAtlasName))
    {
        DRAGONBONES_ASSERT(false, "No armature data. " + armatureName + ", " + (!dragonBonesName.empty() ? dragonBonesName : ""));
        return nullptr;
    }

    const auto armature = _buildArmature(dataPackage);
    _buildBones(dataPackage, *armature);
    _buildSlots(dataPackage, *armature);
    armature->invalidUpdate("", true);
    armature->advanceTime(0.0f); // Update armature pose.

    return armature;
}

void BaseFactory::replaceSlotDisplay(const std::string& dragonBonesName, const std::string& armatureName, const std::string& slotName, const std::string& displayName, Slot& slot, int displayIndex) const
{
    BuildArmaturePackage dataPackage;
    if (!_fillBuildArmaturePackage(dataPackage, dragonBonesName, armatureName, "", "") || dataPackage.skin == nullptr)
    {
        return;
    }

    const auto displays = dataPackage.skin->getDisplays(slotName);
    if (displays == nullptr) 
    {
        return;
    }

    for (const auto displayData : *displays)
    {
        if (displayData->name == displayName)
        {
            _replaceSlotDisplay(dataPackage, displayData, slot, displayIndex);
        }
    }
}

void BaseFactory::replaceSlotDisplayList(const std::string& dragonBonesName, const std::string& armatureName, const std::string& slotName, Slot& slot) const
{
    BuildArmaturePackage dataPackage;
    if (!_fillBuildArmaturePackage(dataPackage, dragonBonesName, armatureName, "", "") || dataPackage.skin == nullptr)
    {
        return;
    }

    const auto displays = dataPackage.skin->getDisplays(slotName);
    if (displays == nullptr)
    {
        return;
    }

    int displayIndex = 0;
    for (const auto displayData : *displays)
    {
        _replaceSlotDisplay(dataPackage, displayData, slot, displayIndex++);
    }
}

void BaseFactory::changeSkin(Armature* armature, SkinData* skin, std::vector<std::string>* exclude) const
{
    DRAGONBONES_ASSERT(armature && skin, "");

    for (const auto slot : armature->getSlots()) 
    {
        if (skin->displays.find(slot->name) != skin->displays.cend() || std::find(exclude->cbegin(), exclude->cend(), slot->name) != exclude->cend()) 
        {
            continue;
        }

        auto& displays = skin->displays[slot->name];
        auto displayList = slot->getDisplayList(); // Copy.
        displayList.resize(displays.size(), std::make_pair(nullptr, DisplayType::Image));
        for (std::size_t i = 0, l = displays.size(); i < l; ++i) 
        {
            const auto displayData = displays[i];
            if (displayData != nullptr) 
            {
                displayList[i] = _getSlotDisplay(nullptr, *displayData, nullptr, *slot);
            }
            else {
                displayList[i] = std::make_pair(nullptr, DisplayType::Image);
            }
        }

        slot->_rawDisplayDatas = &displays;
        slot->_displayDatas.resize(displays.size(), nullptr);
        for (std::size_t i = 0, l = displays.size(); i < l; ++i)
        {
            slot->_displayDatas[i] = displays[i];
        }

        slot->setDisplayList(displayList);
    }
}

bool BaseFactory::copyAnimationsToArmature(
    Armature& toArmature,
    const std::string& fromArmatreName, const std::string& fromSkinName, const std::string& fromDragonBonesDataName,
    bool replaceOriginalAnimation
) const
{
    BuildArmaturePackage dataPackage;
    if (_fillBuildArmaturePackage(dataPackage, fromDragonBonesDataName, fromArmatreName, fromSkinName, ""))
    {
        const auto fromArmatureData = dataPackage.armature;
        if (replaceOriginalAnimation)
        {
            toArmature.getAnimation()->setAnimations(fromArmatureData->animations);
        }
        else
        {
            auto animations = toArmature.getAnimation()->getAnimations(); // Copy.
            for (const auto& pair : fromArmatureData->animations)
            {
                animations[pair.first] = pair.second;
            }

            toArmature.getAnimation()->setAnimations(animations);
        }

        if (dataPackage.skin)
        {
            for (const auto toSlot : toArmature.getSlots())
            {
                const auto& toSlotDisplayList = toSlot->getDisplayList();
                unsigned index = 0;
                for (const auto& pair : toSlotDisplayList)
                {
                    if (pair.second == DisplayType::Armature)
                    {
                        auto displays = dataPackage.skin->getDisplays(toSlot->name);
                        if (index < displays->size())
                        {
                            const auto fromDisplayData = displays->at(index);
                            if (fromDisplayData != nullptr && fromDisplayData->type == DisplayType::Armature)
                            {
                                copyAnimationsToArmature(*(Armature*)pair.first, fromDisplayData->path, fromSkinName, fromDragonBonesDataName, replaceOriginalAnimation);
                            }
                        }
                    }

                    index++;
                }
            }

            return true;
        }
    }

    return false;
}

DRAGONBONES_NAMESPACE_END
