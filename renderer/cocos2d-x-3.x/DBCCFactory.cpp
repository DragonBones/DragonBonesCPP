#include "DBCCFactory.h"
#include "DBCCEventDispatcher.h"
#include "DBCCTextureAtlas.h"
#include "DBCCArmature.h"
#include "DBCCSlot.h"
#include "cocos2d.h"

NAME_SPACE_DRAGON_BONES_BEGIN

DBCCFactory::DBCCFactory() {}
DBCCFactory::~DBCCFactory()
{
    dispose();
}
void DBCCFactory::dispose(const bool disposeData)
{
    BaseFactory::dispose();
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
        float x = textureData->region.x;
        float y = textureData->region.y;
        float width = textureData->region.width;
        float height = textureData->region.height;
        cocos2d::Rect rect(x, y, width, height);
        cocos2d::Sprite *spriteDisplay = cocos2d::Sprite::createWithTexture(dbccTextureAtlas->texture, rect, false);
        float pivotX = displayData->pivot.x;
        float pivotY = displayData->pivot.y;
        
        if (textureData->frame)
        {
            pivotX += textureData->frame->x;
            pivotY += textureData->frame->y;
        }
        
        spriteDisplay->setAnchorPoint(cocos2d::Point(pivotX / width, 1.f - pivotY / height));
        spriteDisplay->setContentSize(cocos2d::Size(width, height));
        return spriteDisplay;
    }
    
    return nullptr;
}
NAME_SPACE_DRAGON_BONES_END