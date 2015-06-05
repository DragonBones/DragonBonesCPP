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

        auto textureCache = cocos2d::Director::getInstance()->getTextureCache();
        auto texture = textureCache->getTextureForKey(textureAtlasData->imagePath);
        if (!texture)
        {
            cocos2d::Texture2D::PixelFormat old = cocos2d::Texture2D::getDefaultAlphaPixelFormat();
            cocos2d::Texture2D::PixelFormat now = old;
            switch (textureAtlasData->format)
            {
            case PixelFormat::RGBA8888:
                now = cocos2d::Texture2D::PixelFormat::RGBA8888;
                break;
            case PixelFormat::BGRA8888:
                now = cocos2d::Texture2D::PixelFormat::BGRA8888;
                break;
            case PixelFormat::RGBA4444:
                now = cocos2d::Texture2D::PixelFormat::RGBA4444;
                break;
            case PixelFormat::RGB888:
                now = cocos2d::Texture2D::PixelFormat::RGB888;
                break;
            case PixelFormat::RGB565:
                now = cocos2d::Texture2D::PixelFormat::RGB565;
                break;
            case PixelFormat::RGBA5551:
                now = cocos2d::Texture2D::PixelFormat::RGB5A1;
                break;
            }
            cocos2d::Texture2D::setDefaultAlphaPixelFormat(now);
            texture = textureCache->addImage(textureAtlasData->imagePath);
            cocos2d::Texture2D::setDefaultAlphaPixelFormat(old);
        }
        return texture;
    }

    virtual cocos2d::Texture2D* reloadTexture()
    {
        return getTexture();
//         if (!textureAtlasData) return nullptr;
// 
//         auto textureCache = cocos2d::Director::getInstance()->getTextureCache();
//         return textureCache->addImage(textureAtlasData->imagePath);
    }
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCTextureAtlas);
};
NAME_SPACE_DRAGON_BONES_END

#endif  // DBCC_TEXTURE_ATLAS_H