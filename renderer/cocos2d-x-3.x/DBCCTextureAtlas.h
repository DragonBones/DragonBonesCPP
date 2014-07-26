#ifndef __DBCC_TEXTURE_ATLAS_H__
#define __DBCC_TEXTURE_ATLAS_H__

#include "../dragonBones/DragonBonesHeaders.h"
#include "cocos2d.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class DBCCTextureAtlas : public ITextureAtlas
{
public:
    cocos2d::Texture2D *texture;
    
public:
    DBCCTextureAtlas(const TextureAtlasData *textureAtlasData_)
        : ITextureAtlas(textureAtlasData_)
    {
    }
    virtual ~DBCCTextureAtlas()
    {
        dispose();
    }
    virtual void dispose() override
    {
        texture = nullptr;
    }
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCTextureAtlas);
};
NAME_SPACE_DRAGON_BONES_END

#endif  // __DBCC_TEXTURE_ATLAS_H__