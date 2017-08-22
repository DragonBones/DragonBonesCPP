#ifndef DRAGONBONES_TEXTUREATLAS_DATA_H
#define DRAGONBONES_TEXTUREATLAS_DATA_H

#include "../core/BaseObject.h"
#include "../geom/Rectangle.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* 贴图集数据。
* @version DragonBones 3.0
* @language zh_CN
*/
class TextureAtlasData : public BaseObject
{
    ABSTRACT_CLASS(TextureAtlasData);

public:
    /**
    * 是否开启共享搜索。
    * @default false
    * @version DragonBones 4.5
    * @language zh_CN
    */
    bool autoSearch;
    /**
    * @private
    */
    TextureFormat format;
    /**
    * @private
    */
    unsigned width;
    /**
    * @private
    */
    unsigned height;
    /**
    * 贴图集缩放系数。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    float scale;
    /**
    * 贴图集名称。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::string name;
    /**
    * 贴图集图片路径。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::string imagePath;
    /**
    * @private
    */
    std::map<std::string, TextureData*> textures;
    /**
    * @private
    */
    void copyFrom(const TextureAtlasData& value);
    /**
    * @private
    */
    virtual TextureData* createTexture() const = 0;
    /**
    * @private
    */
    virtual void addTexture(TextureData* value);
    /**
    * @private
    */
    inline TextureData* getTexture(const std::string& name) const
    {
        return mapFind(textures, name);
    }

protected:
    virtual void _onClear() override;
};
/**
* @private
*/
class TextureData : public BaseObject
{
public:
    static Rectangle* createRectangle();

public:
    bool rotated;
    std::string name;
    Rectangle region;
    TextureAtlasData* parent;
    Rectangle* frame;

    TextureData() :
        frame(nullptr)
    {}
    virtual ~TextureData() = 0;

    void copyFrom(const TextureData& value);

protected:
    virtual void _onClear() override;

public: // For WebAssembly.
    inline const Rectangle& getRegion() const { return region; }
    inline const TextureAtlasData* getParent() const { return parent; }
    inline const Rectangle* getFrame() const { return frame; }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_TEXTUREATLAS_DATA_H
