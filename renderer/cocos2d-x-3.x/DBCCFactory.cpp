#include "DBCCFactory.h"
#include "DBCCTextureAtlas.h"
#include "DBCCSlot.h"
#include "DBCCEventDispatcher.h"
#include "DBCCArmature.h"
NAME_SPACE_DRAGON_BONES_BEGIN
DBCCFactory DBCCFactory::factory;

DBCCFactory* DBCCFactory::getInstance()
{
    return &factory;
}

DBCCFactory::DBCCFactory() {}
DBCCFactory::~DBCCFactory() {}

DBCCArmature* DBCCFactory::buildArmature(const String &armatureName) const
{
    return (DBCCArmature*) BaseFactory::buildArmature(armatureName);
}

DBCCArmature* DBCCFactory::buildArmature(const String &armatureName, const String &dragonBonesName) const
{
    return (DBCCArmature*) BaseFactory::buildArmature(armatureName, dragonBonesName);
}

DBCCArmature* DBCCFactory::buildArmature(const String &armatureName, const String &skinName, const String &animationName,
                                         const String &dragonBonesName, const String &textureAtlasName) const
{
    return (DBCCArmature*) BaseFactory::buildArmature(armatureName, skinName, animationName, dragonBonesName, textureAtlasName);
}

DragonBonesData* DBCCFactory::loadDragonBonesData(const std::string &dragonBonesFilePath, const std::string &name)
{
    DragonBonesData *existDragonBonesData = getDragonBonesData(name);
    
    if (existDragonBonesData)
    {
        return existDragonBonesData;
    }
    
    const auto &data = cocos2d::FileUtils::getInstance()->getDataFromFile(dragonBonesFilePath);
    if (data.getSize() == 0)
    {
        return nullptr;
    }

    // armature scale
    float scale = cocos2d::Director::getInstance()->getContentScaleFactor();

    // load skeleton.xml using XML parser.
    dragonBones::XMLDocument doc;
    doc.Parse(reinterpret_cast<char*>(data.getBytes()), data.getSize());
    // paser dragonbones skeleton data.
    dragonBones::XMLDataParser parser;
    DragonBonesData *dragonBonesData = parser.parseDragonBonesData(doc.RootElement(), scale);
    addDragonBonesData(dragonBonesData, name);
    return dragonBonesData;
}

ITextureAtlas* DBCCFactory::loadTextureAtlas(const std::string &textureAtlasFile, const std::string &name)
{
    ITextureAtlas *existTextureAtlas = getTextureAtlas(name);
    
    if (existTextureAtlas)
    {
        refreshTextureAtlasTexture(name.empty() ? existTextureAtlas->textureAtlasData->name : name);
        return existTextureAtlas;
    }
    
    const auto &data = cocos2d::FileUtils::getInstance()->getDataFromFile(textureAtlasFile);
    if (data.getSize() == 0)
    {
        return nullptr;
    }

    // textureAtlas scale
    float scale = cocos2d::Director::getInstance()->getContentScaleFactor();

    dragonBones::XMLDocument doc;
    doc.Parse(reinterpret_cast<char*>(data.getBytes()), data.getSize());
    dragonBones::XMLDataParser parser;
    DBCCTextureAtlas *textureAtlas = new DBCCTextureAtlas();
    textureAtlas->textureAtlasData = parser.parseTextureAtlasData(doc.RootElement(), scale);

    int pos = textureAtlasFile.find_last_of("/");
    
    if (String::npos != pos)
    {
        String base_path = textureAtlasFile.substr(0, pos + 1);
        textureAtlas->textureAtlasData->imagePath = base_path + textureAtlas->textureAtlasData->imagePath;
    }
    
    //
    addTextureAtlas(textureAtlas, name);
    refreshTextureAtlasTexture(name.empty() ? textureAtlas->textureAtlasData->name : name);
    return textureAtlas;
}

void DBCCFactory::refreshTextureAtlasTexture(const std::string &name)
{
    for (auto iterator = _textureAtlasMap.begin(); iterator != _textureAtlasMap.end(); ++iterator)
    {
        DBCCTextureAtlas *textureAtlas = static_cast<DBCCTextureAtlas*>(iterator->second);
        const TextureAtlasData *textureAtlasData = textureAtlas->textureAtlasData;
        
        if (iterator->first == name)
        {
            textureAtlas->texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(textureAtlasData->imagePath.c_str());
        }
    }
}

void DBCCFactory::refreshAllTextureAtlasTexture()
{
    for (auto iterator = _textureAtlasMap.begin(); iterator != _textureAtlasMap.end(); ++iterator)
    {
        DBCCTextureAtlas *textureAtlas = static_cast<DBCCTextureAtlas*>(iterator->second);
        const TextureAtlasData *textureAtlasData = textureAtlas->textureAtlasData;
        textureAtlas->texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(textureAtlasData->imagePath.c_str());
    }
}

bool DBCCFactory::hasDragonBones(const std::string &skeletonName, const std::string &armatureName, const std::string &animationName)
{
    auto dragonbonesData = getDragonBonesData(skeletonName);
    
    if (!dragonbonesData) { return false; }
    
    if (!armatureName.empty())
    {
        auto armatureData = dragonbonesData->getArmatureData(armatureName);
        
        if (!armatureData) { return false; }
        
        if (!animationName.empty())
        {
            auto animationData = armatureData->getAnimationData(animationName);
            return animationData != nullptr;
        }
    }
    
    return true;
}

DBCCArmature* DBCCFactory::generateArmature(const ArmatureData *armatureData) const
{
    Animation *animation = new Animation();
    // sprite
    cocos2d::Node *display = cocos2d::Node::create();
    display->setCascadeColorEnabled(true);
    display->setCascadeOpacityEnabled(true);
    display->retain();
    // eventDispatcher
    DBCCEventDispatcher *eventDispatcher = new DBCCEventDispatcher();
    eventDispatcher->eventDispatcher = new cocos2d::EventDispatcher();
    eventDispatcher->eventDispatcher->setEnabled(true);
    // armature
    return new DBCCArmature((ArmatureData*)(armatureData), animation, eventDispatcher, display);
}

DBCCSlot* DBCCFactory::generateSlot(const SlotData *slotData) const
{
    return new DBCCSlot((SlotData*)(slotData));
}

void* DBCCFactory::generateDisplay(const ITextureAtlas *textureAtlas, const TextureData *textureData, const DisplayData *displayData) const
{
    DBCCTextureAtlas *dbccTextureAtlas = (DBCCTextureAtlas*)(textureAtlas);
    
    if (dbccTextureAtlas && textureData)
    {
        if (!dbccTextureAtlas->texture)
        {
            // throw
        }
        
        const float x = textureData->region.x;
        const float y = textureData->region.y;
        const float width = textureData->region.width;
        const float height = textureData->region.height;
        const cocos2d::Rect rect(x, y, width, height);
        // sprite
        cocos2d::Node *display = cocos2d::Sprite::createWithTexture(dbccTextureAtlas->texture, rect, false);
        display->retain();
        float pivotX = 0;
        float pivotY = 0;
        
        if (displayData)
        {
            pivotX = displayData->pivot.x;
            pivotY = displayData->pivot.y;
            
            if (textureData->frame)
            {
                pivotX += textureData->frame->x;
                pivotY += textureData->frame->y;
            }
        }
        
        display->setAnchorPoint(cocos2d::Point(pivotX / width, 1.f - pivotY / height));
        display->setContentSize(cocos2d::Size(width, height));
        return display;
    }
    
    return nullptr;
}
NAME_SPACE_DRAGON_BONES_END