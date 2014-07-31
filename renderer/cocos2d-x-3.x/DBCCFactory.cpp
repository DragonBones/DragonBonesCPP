#include "DBCCFactory.h"
#include "DBCCEventDispatcher.h"
#include "DBCCTextureAtlas.h"
#include "DBCCArmature.h"
#include "DBCCSlot.h"

NAME_SPACE_DRAGON_BONES_BEGIN

DBCCFactory::DBCCFactory() {}
DBCCFactory::~DBCCFactory() {}

void DBCCFactory::loadDragonBonesData(const String &dragonBonesFilePath, const String &name)
{
    if (getDragonBonesData(name))
    {
        return;
    }
    const auto &data = cocos2d::FileUtils::getInstance()->getDataFromFile(dragonBonesFilePath);
    // 使用XML解析器载入dragonBones的skeleton.xml
    dragonBones::XMLDocument doc;
    doc.Parse(reinterpret_cast<char *>(data.getBytes()), data.getSize());
    // 解析骨骼动画数据
    dragonBones::XMLDataParser parser;
    DragonBonesData *dragonBonesData = parser.parseDragonBonesData(doc.RootElement());
    addDragonBonesData(dragonBonesData, name);
}

void DBCCFactory::loadTextureAtlas(const String &textureAtlasFile, const String &name)
{
    if (getTextureAtlas(name))
    {
        return;
    }
    const auto &data = cocos2d::FileUtils::getInstance()->getDataFromFile(textureAtlasFile);
    dragonBones::XMLDocument doc;
    doc.Parse(reinterpret_cast<char *>(data.getBytes()), data.getSize());
    dragonBones::XMLDataParser parser;
    TextureAtlasData *textureAtlasData = parser.parseTextureAtlasData(doc.RootElement());
    DBCCTextureAtlas *textureAtlas = new DBCCTextureAtlas(textureAtlasData);
    //
    int pos = textureAtlasFile.find_last_of("/");
    if (String::npos != pos)
    {
        String base_path = textureAtlasFile.substr(0, pos + 1);
        textureAtlasData->imagePath = base_path + textureAtlasData->imagePath;
    }
    //
    textureAtlas->texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(textureAtlasData->imagePath.c_str());
    addTextureAtlas(textureAtlas, name);
}

void DBCCFactory::refreshTextureAtlasTexture()
{
    for (auto iterator = _textureAtlasMap.begin(); iterator != _textureAtlasMap.end(); ++iterator)
    {
        DBCCTextureAtlas *textureAtlas = static_cast<DBCCTextureAtlas *>(iterator->second);
        const TextureAtlasData *textureAtlasData = textureAtlas->textureAtlasData;
        textureAtlas->texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(textureAtlasData->imagePath.c_str());
    }
}

Armature *DBCCFactory::generateArmature(const ArmatureData *armatureData) const
{
    Animation *animation = new Animation();
    // sprite
    cocos2d::Sprite *display = cocos2d::Sprite::create();
    display->setCascadeColorEnabled(true);
    display->setCascadeOpacityEnabled(true);
    display->retain();
    // eventDispatcher
    DBCCEventDispatcher *eventDispatcher = new DBCCEventDispatcher();
    eventDispatcher->eventDispatcher = display->getEventDispatcher();
    // armature
    Armature *armature = new DBCCArmature((ArmatureData *)(armatureData), animation, eventDispatcher, display);
    return armature;
}

Slot *DBCCFactory::generateSlot(const SlotData *slotData) const
{
    DBCCSlot *dbccSlot = new DBCCSlot((SlotData *)(slotData));
    return dbccSlot;
}

void *DBCCFactory::generateDisplay(const ITextureAtlas *textureAtlas, const String &textureName, const DisplayData *displayData) const
{
    DBCCTextureAtlas *dbccTextureAtlas = (DBCCTextureAtlas *)(textureAtlas);
    if (dbccTextureAtlas)
    {
        TextureData *textureData = dbccTextureAtlas->textureAtlasData->getTextureData(textureName);
        if (textureData)
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
            cocos2d::Sprite *display = cocos2d::Sprite::createWithTexture(dbccTextureAtlas->texture, rect, false);
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
    }
    return nullptr;
}
NAME_SPACE_DRAGON_BONES_END