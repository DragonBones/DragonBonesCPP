#ifndef __TEXTURES_TEXTURE_DATA_H__
#define __TEXTURES_TEXTURE_DATA_H__

#include "../DragonBones.h"
#include "../geoms/Rectangle.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class TextureData
{
public:
    bool rotated;
    
    String name;
    Rectangle region;
    
    Rectangle *frame;
    
public:
    TextureData()
    {
        rotated = false;
        frame = nullptr;
    }
    TextureData(const TextureData &copyData)
    {
        operator=(copyData);
    }
    TextureData &operator=(const TextureData &copyData)
    {
        dispose();
        rotated = copyData.rotated;
        name = copyData.name;
        region = copyData.region;
        if (copyData.frame)
        {
            frame = new Rectangle();
            *frame = *(copyData.frame);
        }
        return *this;
    }
    virtual ~TextureData()
    {
        dispose();
    }
    void dispose()
    {
        if (frame)
        {
            delete frame;
            frame = nullptr;
        }
    }
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __TEXTURES_TEXTURE_DATA_H__