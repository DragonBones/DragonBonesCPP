#ifndef DRAGONBONES_CC_TEXTUREATLAS_DATA_H
#define DRAGONBONES_CC_TEXTUREATLAS_DATA_H

#include "dragonBones/DragonBonesHeaders.h"
#include "cocos2d.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* @private
*/
class CCTextureData : public TextureData
{
    BIND_CLASS_TYPE_B(CCTextureData);

public:
    cocos2d::SpriteFrame* spriteFrame;

    CCTextureData() :
        spriteFrame(nullptr)
    {
        _onClear();
    }
    virtual ~CCTextureData()
    {
        _onClear();
    }

protected:
    virtual void _onClear() override;
};
/**
* Cocos 贴图集数据。
* @version DragonBones 3.0
* @language zh_CN
*/
class CCTextureAtlasData : public TextureAtlasData
{
    BIND_CLASS_TYPE_B(CCTextureAtlasData);

private:
    cocos2d::Texture2D* _renderTexture;

public:
    /**
    * @private
    */
    CCTextureAtlasData() :
        _renderTexture(nullptr)
    {
        _onClear();
    }
    virtual ~CCTextureAtlasData()
    {
        _onClear();
    }

protected:
    virtual void _onClear() override;

public:
    /**
    * @private
    */
    virtual TextureData* createTexture() const override;
    /**
    * Cocos 贴图。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline cocos2d::Texture2D* getRenderTexture() const 
    {
        return _renderTexture;
    }
    void setRenderTexture(cocos2d::Texture2D* value);
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_CC_TEXTUREATLAS_DATA_H
