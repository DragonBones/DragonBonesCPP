#include "CCFactory.h"
#include "CCTextureData.h"
#include "CCArmatureDisplayContainer.h"
#include "CCSlot.h"

NAMESPACE_DRAGONBONES_BEGIN

CCFactory::CCFactory() 
{
    clear();
}
CCFactory::~CCFactory() 
{
    clear();
}

TextureAtlasData * CCFactory::_generateTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const
{
    if (textureAtlasData)
    {
        static_cast<CCTextureAtlasData*>(textureAtlasData)->texture = (cocos2d::Texture2D*)textureAtlas;
    }
    else
    {
        textureAtlasData = BaseObject::borrowObject<CCTextureAtlasData>();
    }

    return textureAtlasData;
}

Armature * CCFactory::_generateArmature(const BuildArmaturePackage & dataPackage) const
{
    const auto armature = BaseObject::borrowObject<Armature>();
    const auto armatureDisplayContainer = CCArmatureDisplayContainer::create();
    armatureDisplayContainer->retain();
    armature->_armatureData = dataPackage.armature;
    armature->_skinData = dataPackage.skin;
    armature->_animation = BaseObject::borrowObject<Animation>();
    armature->_display = armatureDisplayContainer;

    armatureDisplayContainer->_armature = armature;
    armature->_animation->_armature = armature;

    armature->getAnimation().setAnimations(dataPackage.armature->animations);

    return armature;
}

Slot * CCFactory::_generateSlot(const BuildArmaturePackage& dataPackage, const SlotDisplayDataSet& slotDisplayDataSet) const
{
    const auto slot = BaseObject::borrowObject<CCSlot>();
    const auto slotData = slotDisplayDataSet.slot;
    std::vector<std::pair<void*, DisplayType>> displayList;
    const auto rawDisplay = cocos2d::Sprite::create();

    slot->name = slotData->name;
    slot->_rawDisplay = rawDisplay;

    displayList.reserve(slotDisplayDataSet.displays.size());
    rawDisplay->retain();

    for (const auto displayData : slotDisplayDataSet.displays)
    {
        switch (displayData->type)
        {
            case DisplayType::Image:
                if (!displayData->textureData)
                {
                    displayData->textureData = this->_getTextureData(dataPackage.dataName, displayData->name);
                }

                displayList.push_back(std::make_pair(slot->_rawDisplay, DisplayType::Image));
                break;

            case DisplayType::Armature:
            {
                const auto childArmature = buildArmature(displayData->name, dataPackage.dataName);
                if (childArmature)
                {
                    childArmature->getAnimation().play();
                }

                displayList.push_back(std::make_pair(childArmature, DisplayType::Armature));
                break;
            }

            case DisplayType::Mesh:
                //displayData.textureData = displayData.textureData || this._getTextureData(dataPackage.dataName, displayData.name);
                displayList.push_back(std::make_pair(nullptr, DisplayType::Armature));
                break;

            default:
                displayList.push_back(std::make_pair(nullptr, DisplayType::Image));
                break;
        }

    }

    slot->_setDisplayList(displayList);

    return slot;
}

void * CCFactory::_generateDisplay(const BuildArmaturePackage& dataPackage, const DisplayData& displayData, const DisplayData& rawDisplayData) const
{
    return nullptr;
}

DragonBonesData* CCFactory::loadDragonBonesData(const std::string& filePath, const std::string& dragonBonesName)
{
    if (!dragonBonesName.empty())
    {
        DragonBonesData* existData = getDragonBonesData(filePath);

        if (existData)
        {
            return existData;
        }
    }

    const auto& fullpath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filePath);
    const auto& data = cocos2d::FileUtils::getInstance()->getStringFromFile(fullpath);
    if (data.empty())
    {
        return nullptr;
    }

    //const auto scale = cocos2d::Director::getInstance()->getContentScaleFactor();

    return parseDragonBonesData(data.c_str(), dragonBonesName);
}

NAMESPACE_DRAGONBONES_END
