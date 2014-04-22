
#ifndef __COCOS2DX_ATLAS_NODE_H__
#define __COCOS2DX_ATLAS_NODE_H__

#include "CCNode.h"
#include "CCProtocols.h"
#include "ccTypes.h"
#include "preDB.h"
#include "BytesType.h"
#include "CCTextureAtlas.h"
#include "CCQuadCommand.h"
NS_CC_BEGIN

/**
 * @addtogroup base_nodes
 * @{
 */


/** @brief Cocos2dxAtlasNode is a subclass of CCNode that implements the CCRGBAProtocol and CCTextureProtocol protocol

It knows how to render a TextureAtlas object.
If you are going to render a TextureAtlas consider subclassing Cocos2dxAtlasNode (or a subclass of Cocos2dxAtlasNode)

All features from CCNode are valid, plus the following features:
- opacity and RGB colors
*/
class Cocos2dxAtlasNode : public Node, public TextureProtocol
{
protected:
    
    Color3B    m_tColorUnmodified;

    CC_PROPERTY(TextureAtlas*, m_pTextureAtlas, TextureAtlas);

    // protocol variables
    bool m_bIsOpacityModifyRGB;
    
    BlendFunc m_tBlendFunc;

    // color uniform
    GLint    m_nUniformColor;
    // This varible is only used for CCLabelAtlas FPS display. So plz don't modify its value.
    bool m_bIgnoreContentScaleFactor;

    // 要画哪个矩形
    unsigned int m_nQuadIndex;
	// quad command
	QuadCommand _quadCommand;

public:
    Cocos2dxAtlasNode();
    virtual ~Cocos2dxAtlasNode();

    /** creates a Cocos2dxAtlasNode  with an Atlas file the width and height of each item and the quantity of items to render*/
    static Cocos2dxAtlasNode * create(TextureAtlas* textureAtlas , unsigned int quadIndex , const Rect &sourceRect);
    bool initWithTextureAtlas(TextureAtlas* textureAtlas , unsigned int quadIndex , const Rect &sourceRect);
    

    
    // Overrides
    virtual void draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated) override;
    virtual Texture2D* getTexture() const override;
    virtual void setTexture(Texture2D *texture) override;
    virtual bool isOpacityModifyRGB() const override;
    virtual void setOpacityModifyRGB(bool isOpacityModifyRGB) override;
    virtual const Color3B& getColor(void) const override;
    virtual void setColor(const Color3B& color) override;
    virtual void setOpacity(GLubyte opacity) override;
    /**
    * @code
    * When this function bound into js or lua,the parameter will be changed
    * In js: var setBlendFunc(var src, var dst)
    * @endcode
    * @lua NA
    */
    virtual void setBlendFunc(const BlendFunc& blendFunc) override;
    /**
    * @js NA
    * @lua NA
    */
    virtual const BlendFunc& getBlendFunc() const override;

private :
    void updateBlendFunc();
    void updateOpacityModifyRGB();
    
    friend class Director;
    void setIgnoreContentScaleFactor(bool bIgnoreContentScaleFactor);
};

// end of base_node group
/// @}

NS_CC_END

#endif // __CCATLAS_NODE_H__


