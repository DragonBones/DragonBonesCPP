#include "DBCCFactory.h"
#include "DBCCTextureAtlas.h"
#include "DBCCSlot.h"
#include "DBCCEventDispatcher.h"
#include "DBCCArmature.h"

NAME_SPACE_DRAGON_BONES_BEGIN
DBCCFactory* DBCCFactory::_instance = nullptr;

DBCCFactory* DBCCFactory::getInstance()
{
    if (!_instance)
    {
        _instance = new DBCCFactory();
    }
    return _instance;
}

void DBCCFactory::destroyInstance()
{
    if (_instance)
    {
       CC_SAFE_DELETE(_instance);
    }
}

DBCCFactory::DBCCFactory() {}
DBCCFactory::~DBCCFactory() {}

DBCCArmature* DBCCFactory::buildArmature(const std::string &armatureName) const
{
    return (DBCCArmature*) BaseFactory::buildArmature(armatureName);
}

DBCCArmature* DBCCFactory::buildArmature(const std::string &armatureName, const std::string &dragonBonesName) const
{
    return (DBCCArmature*) BaseFactory::buildArmature(armatureName, dragonBonesName);
}

DBCCArmature* DBCCFactory::buildArmature(const std::string &armatureName, const std::string &skinName, const std::string &animationName,
                                         const std::string &dragonBonesName, const std::string &textureAtlasName) const
{
    return (DBCCArmature*) BaseFactory::buildArmature(armatureName, skinName, animationName, dragonBonesName, textureAtlasName);
}

DBCCArmatureNode* DBCCFactory::buildArmatureNode(const std::string &armatureName) const
{
    auto armature = buildArmature(armatureName);
    return DBCCArmatureNode::create(armature);
}

DBCCArmatureNode* DBCCFactory::buildArmatureNode(const std::string &armatureName, const std::string &dragonBonesName) const
{
    auto armature = buildArmature(armatureName, dragonBonesName);
    return DBCCArmatureNode::create(armature);
}

DBCCArmatureNode* DBCCFactory::buildArmatureNode(const std::string &armatureName, const std::string &skinName, const std::string &animationName,
    const std::string &dragonBonesName, const std::string &textureAtlasName) const
{
    auto armature = buildArmature(armatureName, skinName, animationName, dragonBonesName, textureAtlasName);
    return DBCCArmatureNode::create(armature);
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

    if (std::string::npos != pos)
    {
        std::string base_path = textureAtlasFile.substr(0, pos + 1);
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
            textureAtlas->reloadTexture();
        }
    }
}

void DBCCFactory::refreshAllTextureAtlasTexture()
{
    for (auto iterator = _textureAtlasMap.begin(); iterator != _textureAtlasMap.end(); ++iterator)
    {
        DBCCTextureAtlas *textureAtlas = static_cast<DBCCTextureAtlas*>(iterator->second);
        const TextureAtlasData *textureAtlasData = textureAtlas->textureAtlasData;
        textureAtlas->reloadTexture();
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

    if (!dbccTextureAtlas || !textureData) return nullptr;

    auto texture = dbccTextureAtlas->getTexture();
    assert(texture);

    const float x = textureData->region.x;
    const float y = textureData->region.y;
    const bool rotated = textureData->rotated;
    const float width = rotated ? textureData->region.height : textureData->region.width;
    const float height = rotated ? textureData->region.width : textureData->region.height;
    cocos2d::Rect rect(x, y, width, height);
    cocos2d::Vec2 offset;
    cocos2d::Size originSize(width, height);

    cocos2d::Node *display = nullptr;

    if (textureData->frame)
    {
        float frameX = -textureData->frame->x;
        float frameY = -textureData->frame->y;
        originSize.width = textureData->frame->width;
        originSize.height = textureData->frame->height;
        // offset = trimed center - origin center
        // y use cocos2d coordinates
        offset.x = (width - originSize.width) * 0.5 + frameX;
        offset.y = (originSize.height - height)*0.5 - frameY;

        auto spriteFrame = cocos2d::SpriteFrame::createWithTexture(texture, rect,
            textureData->rotated, offset, originSize);
        display = cocos2d::Sprite::createWithSpriteFrame(spriteFrame);
    }
    else
    {
        display = cocos2d::Sprite::createWithTexture(texture, rect, rotated);
    }
    // sprite
    
    display->setCascadeColorEnabled(true);
    display->setCascadeOpacityEnabled(true);
    display->retain();
    float pivotX = 0.f;
    float pivotY = 0.f;

    if (displayData)
    {
        pivotX = displayData->pivot.x;
        pivotY = displayData->pivot.y;
    }
    display->setAnchorPoint(cocos2d::Vec2(pivotX / originSize.width, 1.f - pivotY / originSize.height));
    display->setContentSize(originSize);
    return display;
}
NAME_SPACE_DRAGON_BONES_END
