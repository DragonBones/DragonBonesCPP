#include "DBCCFactory.h"
#include "DBCCEventDispatcher.h"
#include "DBCCTextureAtlas.h"
#include "DBCCArmature.h"
#include "DBCCSlot.h"
#include "cocos2d.h"

NAME_SPACE_DRAGON_BONES_BEGIN

DBCCFactory::DBCCFactory() {}
DBCCFactory::~DBCCFactory() {}

void DBCCFactory::loadSkeleton(const String &skeletonFilePath, const String &name)
{
    if (getSkeletonData(name))
    {
        return;
    }
    auto data = cocos2d::FileUtils::getInstance()->getDataFromFile(skeletonFilePath);
    // 使用XML解析器载入skeleton的xml
    dragonBones::XMLDocument doc;
    doc.Parse(reinterpret_cast<char *>(data.getBytes()), data.getSize());
    // 解析骨骼动画数据
    dragonBones::XMLDataParser parser;
    SkeletonData *skeleton = parser.parseSkeletonData(doc.RootElement());
    addSkeletonData(skeleton, name);
}

void DBCCFactory::loadTextureAtlas(const String &textureAtlasFile, const String &name)
{
    if (getTextureAtlas(name))
    {
        return;
    }
    auto data = cocos2d::FileUtils::getInstance()->getDataFromFile(textureAtlasFile);
    // 载入皮肤数据
    dragonBones::XMLDocument doc;
    doc.Parse(reinterpret_cast<char *>(data.getBytes()), data.getSize());
    int pos = textureAtlasFile.find_last_of("/");
    if (std::string::npos != pos)
    {
        std::string base_path = textureAtlasFile.substr(0, pos + 1);
        std::string img_path = doc.RootElement()->Attribute(A_IMAGE_PATH);
        std::string new_img_path = base_path + img_path;
        doc.RootElement()->SetAttribute(A_IMAGE_PATH, new_img_path.c_str());
    }
    // 解析皮肤数据
    dragonBones::XMLDataParser parser;
    TextureAtlasData *textureAtlasData = parser.parseTextureAtlasData(doc.RootElement());
    DBCCTextureAtlas *textureAtlas = new DBCCTextureAtlas(textureAtlasData);
    textureAtlas->texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(textureAtlasData->imagePath.c_str());
    addTextureAtlas(textureAtlas, name);
}

Armature *DBCCFactory::generateArmature(const ArmatureData *armatureData) const
{
    Armature *armature = new DBCCArmature((ArmatureData *)(armatureData), new Animation(), new DBCCEventDispatcher(), cocos2d::Sprite::create());
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
            float x = textureData->region.x;
            float y = textureData->region.y;
            float width = textureData->region.width;
            float height = textureData->region.height;
            cocos2d::Rect rect(x, y, width, height);

			//sprite
            cocos2d::Node *display = cocos2d::Sprite::createWithTexture(dbccTextureAtlas->texture, rect, false);
            display->retain();
            float pivotX = displayData->pivot.x;
            float pivotY = displayData->pivot.y;
            if (textureData->frame)
            {
                pivotX += textureData->frame->x;
                pivotY += textureData->frame->y;
            }
            display->setAnchorPoint(cocos2d::Point(pivotX / width, 1.f - pivotY / height));
            display->setContentSize(cocos2d::Size(width, height));
            return display;
        }
    }
    return nullptr;
}
NAME_SPACE_DRAGON_BONES_END