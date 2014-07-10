#include "Cocos2dxAtlasNode.h"
#include "CCTextureAtlas.h"
#include "CCTextureCache.h"
#include "CCDirector.h"
#include "CCGLProgram.h"
#include "ccGLStateCache.h"
#include "CCDirector.h"
#include "CCRenderer.h"

NS_CC_BEGIN

// implementation Cocos2dxAtlasNode

// Cocos2dxAtlasNode - Creation & Init

Cocos2dxAtlasNode::Cocos2dxAtlasNode()
: m_pTextureAtlas(NULL)
, m_bIsOpacityModifyRGB(false)
, m_nUniformColor(0)
, m_bIgnoreContentScaleFactor(false)
{
}

Cocos2dxAtlasNode::~Cocos2dxAtlasNode()
{
    CC_SAFE_RELEASE(m_pTextureAtlas);
}

Cocos2dxAtlasNode * Cocos2dxAtlasNode::create(TextureAtlas* textureAtlas , unsigned int quadIndex , const Rect &sourceRect)
{
    Cocos2dxAtlasNode * pRet = new Cocos2dxAtlasNode();
    if (pRet->initWithTextureAtlas(textureAtlas , quadIndex , sourceRect))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool Cocos2dxAtlasNode::initWithTextureAtlas(TextureAtlas* textureAtlas , unsigned int quadIndex , const Rect &sourceRect)
{
    m_nQuadIndex = quadIndex;

	m_tColorUnmodified = Color3B::WHITE;
    m_bIsOpacityModifyRGB = true;

	m_tBlendFunc = BlendFunc::ALPHA_PREMULTIPLIED;

    m_pTextureAtlas = textureAtlas;

    if (! m_pTextureAtlas)
    {
        CCLOG("cocos2d: Could not initialize Cocos2dxAtlasNode. Invalid Texture.");
        return false;
    }
    m_pTextureAtlas->retain();

    this->updateBlendFunc();
    this->updateOpacityModifyRGB();


	// shader stuff
	setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));


    cocos2d::V3F_C4B_T2F_Quad & quad = textureAtlas->getQuads()[quadIndex];

    quad.bl.vertices.x = sourceRect.getMinX();
	quad.bl.vertices.y = sourceRect.getMinY();
	quad.bl.vertices.z = 0;
	quad.br.vertices.x = sourceRect.getMaxX();
	quad.br.vertices.y = sourceRect.getMinY();
	quad.br.vertices.z = 0;
	quad.tl.vertices.x = sourceRect.getMinX();
	quad.tl.vertices.y = sourceRect.getMaxY();
	quad.tl.vertices.z = 0;
	quad.tr.vertices.x = sourceRect.getMaxX();
	quad.tr.vertices.y = sourceRect.getMaxY();
	quad.tr.vertices.z = 0;


    //*(cocos2d::CCPoint*)&quad.bl.vertices = CCPointApplyAffineTransform(*(cocos2d::CCPoint*)&(quad.bl.vertices), matrix);
    //*(cocos2d::CCPoint*)&quad.br.vertices = CCPointApplyAffineTransform(*(cocos2d::CCPoint*)&(quad.br.vertices), matrix);
    //*(cocos2d::CCPoint*)&quad.tl.vertices = CCPointApplyAffineTransform(*(cocos2d::CCPoint*)&(quad.tl.vertices), matrix);
    //*(cocos2d::CCPoint*)&quad.tr.vertices = CCPointApplyAffineTransform(*(cocos2d::CCPoint*)&(quad.tr.vertices), matrix);

    //quad.bl.vertices.y = -quad.bl.vertices.y;
    //quad.br.vertices.y = -quad.br.vertices.y;
    //quad.tl.vertices.y = -quad.tl.vertices.y;
    //quad.tr.vertices.y = -quad.tr.vertices.y;
    return true;
}

// Cocos2dxAtlasNode - draw
void Cocos2dxAtlasNode::draw(Renderer* renderer, const kmMat4 &transform, bool transformUpdated)
{
    // TODO
    // FIXME
    //
    // eastcowboy: 此处有问题
    // 先是_shaderProgram编译不过。
    // 改为GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR), 编译过了但是各个骨骼的位置不对。
    // 似乎是cocos2d-x有问题，而不是DragonBones有问题。
    // 先写一个山寨版本确保可以运行，然后坐等更新。

#if 0
    CC_NODE_DRAW_SETUP();

	
	_quadCommand.init(
		_globalZOrder,
		m_pTextureAtlas->getTexture()->getName(),
		// _shaderProgram,
        GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR),
		m_tBlendFunc,
		m_pTextureAtlas->getQuads() + m_nQuadIndex,
		1,
		transform);

	renderer->addCommand(&_quadCommand);
#elif 1
    // eastcowboy: 山寨版本

    GLProgramState* state = GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR);
    GLProgram* glProgram = state->getGLProgram();
    glProgram->use();
    glProgram->setUniformsForBuiltins();

    const BlendFunc& blendFunc = getBlendFunc();
    ccGLBlendFunc(blendFunc.src, blendFunc.dst);

    GL::bindTexture2D(m_pTextureAtlas->getTexture()->getName());

    V3F_C4B_T2F_Quad* quad = m_pTextureAtlas->getQuads() + m_nQuadIndex;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color | kCCVertexAttribFlag_TexCoords);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), &quad->tl.vertices.x);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V3F_C4B_T2F), &quad->tl.colors.r);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), &quad->tl.texCoords.u);

    GLushort indices[] = {0, 1, 2, 2, 1, 3};
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
#else
    // eastcowboy: 山寨版本2
    // 只能在桌面版本的OpenGL运行，GLES不支持。
    // Windows调试用。
    ccGLUseProgram(0);

    const Mat4& projectionMatrix = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    const Mat4& modelviewMatrix = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projectionMatrix.m);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelviewMatrix.m);

    const BlendFunc& blendFunc = getBlendFunc();
    ccGLBlendFunc(blendFunc.src, blendFunc.dst);

    GL::bindTexture2D(m_pTextureAtlas->getTexture()->getName());

    V3F_C4B_T2F_Quad* quad = m_pTextureAtlas->getQuads() + m_nQuadIndex;

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBegin(GL_QUADS);
        glTexCoord2fv(&quad->tl.texCoords.u);
        glColor4ubv(&quad->tl.colors.r);
        glVertex3fv(&quad->tl.vertices.x);

        glTexCoord2fv(&quad->bl.texCoords.u);
        glColor4ubv(&quad->bl.colors.r);
        glVertex3fv(&quad->bl.vertices.x);

        glTexCoord2fv(&quad->br.texCoords.u);
        glColor4ubv(&quad->br.colors.r);
        glVertex3fv(&quad->br.vertices.x);

        glTexCoord2fv(&quad->tr.texCoords.u);
        glColor4ubv(&quad->tr.colors.r);
        glVertex3fv(&quad->tr.vertices.x);
    glEnd();

    glDisable(GL_TEXTURE_2D);
#endif
}

// Cocos2dxAtlasNode - RGBA protocol

const Color3B& Cocos2dxAtlasNode::getColor()const
{
    if(m_bIsOpacityModifyRGB)
    {
        return m_tColorUnmodified;
    }
    return Node::getColor();
}

void Cocos2dxAtlasNode::setColor(const Color3B& color3)
{
    Color3B tmp = color3;
    m_tColorUnmodified = color3;

    if( m_bIsOpacityModifyRGB )
    {
        tmp.r = tmp.r * _displayedOpacity/255;
        tmp.g = tmp.g * _displayedOpacity/255;
        tmp.b = tmp.b * _displayedOpacity/255;
    }
    Node::setColor(tmp);
}

void Cocos2dxAtlasNode::setOpacity(GLubyte opacity)
{
	cocos2d::V3F_C4B_T2F_Quad & quad = m_pTextureAtlas->getQuads()[m_nQuadIndex];
	quad.bl.colors.a = opacity;
	quad.bl.colors.a = opacity;
	quad.tl.colors.a = opacity;
	quad.tr.colors.a = opacity;

	/*
    Node::setOpacity(opacity);

    // special opacity for premultiplied textures
    if( m_bIsOpacityModifyRGB )
        this->setColor(m_tColorUnmodified);
	*/
}

void Cocos2dxAtlasNode::setOpacityModifyRGB(bool bValue)
{
    Color3B oldColor = this->getColor();
    m_bIsOpacityModifyRGB = bValue;
    this->setColor(oldColor);
}

bool Cocos2dxAtlasNode::isOpacityModifyRGB()const
{
    return m_bIsOpacityModifyRGB;
}

void Cocos2dxAtlasNode::updateOpacityModifyRGB()
{
    m_bIsOpacityModifyRGB = m_pTextureAtlas->getTexture()->hasPremultipliedAlpha();
}

void Cocos2dxAtlasNode::setIgnoreContentScaleFactor(bool bIgnoreContentScaleFactor)
{
    m_bIgnoreContentScaleFactor = bIgnoreContentScaleFactor;
}

// Cocos2dxAtlasNode - CocosNodeTexture protocol


void Cocos2dxAtlasNode::updateBlendFunc()
{
	if( ! m_pTextureAtlas->getTexture()->hasPremultipliedAlpha() )
		m_tBlendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
}

void Cocos2dxAtlasNode::setTexture(Texture2D *texture)
{
    m_pTextureAtlas->setTexture(texture);
    this->updateBlendFunc();
    this->updateOpacityModifyRGB();
}

Texture2D * Cocos2dxAtlasNode::getTexture()const
{
    return m_pTextureAtlas->getTexture();
}

void Cocos2dxAtlasNode::setTextureAtlas(TextureAtlas* var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE(m_pTextureAtlas);
    m_pTextureAtlas = var;
}

TextureAtlas * Cocos2dxAtlasNode::getTextureAtlas()
{
    return m_pTextureAtlas;
}
const BlendFunc& Cocos2dxAtlasNode::getBlendFunc() const
{
	return m_tBlendFunc;
}

void Cocos2dxAtlasNode::setBlendFunc(const BlendFunc &blendFunc)
{
	m_tBlendFunc = blendFunc;
}

NS_CC_END
