#ifndef TEXTURES_TEXTURE_ATLAS_DATA_H
#define TEXTURES_TEXTURE_ATLAS_DATA_H

#include "../DragonBones.h"
#include "TextureData.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class TextureAtlasData
{
public:
    bool autoSearch;
    std::string name;
    std::string imagePath;
    PixelFormat format;

    
    std::vector<TextureData*> textureDataList;
    
public:
    TextureAtlasData()
        :autoSearch(false)
        ,name()
        ,imagePath()
        ,format(PixelFormat::AUTO)
    {}
    TextureAtlasData(const TextureAtlasData &copyData)
    {
        operator=(copyData);
    }
    TextureAtlasData &operator=(const TextureAtlasData &copyData)
    {
        dispose();
        autoSearch = copyData.autoSearch;
        name = copyData.name;
        imagePath = copyData.imagePath;
        textureDataList.reserve(copyData.textureDataList.size());
        
        for (size_t i = 0, l = textureDataList.size(); i < l; ++i)
        {
            textureDataList.push_back(new TextureData());
            *(textureDataList[i]) = *(copyData.textureDataList[i]);
        }
        
        return *this;
    }
    virtual ~TextureAtlasData()
    {
        dispose();
    }
    void dispose()
    {
        for (size_t i = 0, l = textureDataList.size(); i < l; ++i)
        {
            textureDataList[i]->dispose();
            delete textureDataList[i];
        }
        
        textureDataList.clear();
    }
    
    TextureData* getTextureData(const std::string &textureName) const
    {
        for (size_t i = 0, l = textureDataList.size(); i < l; ++i)
        {
            if (textureDataList[i]->name == textureName)
            {
                return textureDataList[i];
            }
        }
        
        return nullptr;
    }
};
NAME_SPACE_DRAGON_BONES_END
#endif  // TEXTURES_TEXTURE_ATLAS_DATA_H