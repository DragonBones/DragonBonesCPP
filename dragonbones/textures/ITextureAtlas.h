#ifndef __TEXTURES_I_TEXTURE_ATLAS_H__
#define __TEXTURES_I_TEXTURE_ATLAS_H__

#include "../DragonBones.h"
#include "TextureAtlasData.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class ITextureAtlas
{
public:
    TextureAtlasData *textureAtlasData;
    
public:
    ITextureAtlas()
    {
    }
    virtual ~ITextureAtlas() {};
    virtual void dispose() = 0;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(ITextureAtlas);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __TEXTURES_I_TEXTURE_ATLAS_H__