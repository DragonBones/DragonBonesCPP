#ifndef TEXTURES_I_TEXTURE_ATLAS_H
#define TEXTURES_I_TEXTURE_ATLAS_H

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
#endif  // TEXTURES_I_TEXTURE_ATLAS_H