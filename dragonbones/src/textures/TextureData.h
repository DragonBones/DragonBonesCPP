#ifndef DRAGONBONES_TEXTURE_DATA_H
#define DRAGONBONES_TEXTURE_DATA_H

#include "../core/BaseObject.h"
#include "../geom/Rectangle.h"

NAMESPACE_DRAGONBONES_BEGIN

class TextureAtlasData;

class TextureData : public BaseObject
{
public:
    static Rectangle* generateRectangle();

public:
    bool rotated;
    std::string name;
    Rectangle* frame;
    TextureAtlasData* parent;
    Rectangle region;

    TextureData();
    virtual ~TextureData() = 0;

protected:
    virtual void _onClear() override;
};

class TextureAtlasData : public BaseObject
{
public:
    bool autoSearch;
    float scale;
    std::string name;
    std::string imagePath;
    std::map<std::string, TextureData*> textures;

    TextureAtlasData();
    virtual ~TextureAtlasData() = 0;

protected:
    virtual void _onClear() override;

public:
    virtual TextureData* generateTexture() const = 0;
    virtual void addTexture(TextureData* value);

    inline TextureData* getTexture(const std::string& name) const
    {
        return mapFind(textures, name);
    }
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_TEXTURE_DATA_H