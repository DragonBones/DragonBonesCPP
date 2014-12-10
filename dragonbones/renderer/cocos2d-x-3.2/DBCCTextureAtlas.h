#ifndef DBCC_TEXTURE_ATLAS_H
#define DBCC_TEXTURE_ATLAS_H

#include "DragonBonesHeaders.h"
#include "cocos2d.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class DBCCTextureAtlas : public ITextureAtlas
{
public:
    DBCCTextureAtlas()
    {
        textureAtlasData = nullptr;
    }
    virtual ~DBCCTextureAtlas()
    {
        dispose();
    }
    virtual void dispose() override
    {
        if (textureAtlasData)
        {
            textureAtlasData->dispose();
            textureAtlasData = nullptr;
        }
    }

    virtual cocos2d::Texture2D* getTexture()
    {
        if (!textureAtlasData) return nullptr;

        auto textureCache = cocos2d::TextureCache::getInstance();
        auto texture = textureCache->getTextureForKey(textureAtlasData->imagePath);
        if (!texture)
        {
            texture = textureCache->addImage(textureAtlasData->imagePath);
        }
        return texture;
    }

    virtual cocos2d::Texture2D* reloadTexture()
    {
        if (!textureAtlasData) return nullptr;

        cocos2d::TextureCache::getInstance()->addImage(textureAtlasData->imagePath);
    }
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCTextureAtlas);
};
NAME_SPACE_DRAGON_BONES_END

#endif  // DBCC_TEXTURE_ATLAS_H