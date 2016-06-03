#include "CCSlot.h"

NAMESPACE_DRAGONBONES_BEGIN

CCSlot::CCSlot() 
{
    _onClear();
}
CCSlot::~CCSlot() 
{
    _onClear();
}

void CCSlot::_onClear()
{
    // _displayList // TODO

    Slot::_onClear();

    _renderDisplay = nullptr;
}

void CCSlot::_initDisplay(void * value)
{
    // TODO
}

void CCSlot::_onUpdateDisplay()
{
    _renderDisplay = static_cast<cocos2d::Node*>(this->_display ? this->_display : this->_rawDisplay);
}

void CCSlot::_addDisplay()
{
    const auto container = static_cast<CCArmatureDisplayContainer*>(this->_armature->getDisplay());
    container->addChild(_renderDisplay);
}

void CCSlot::_replaceDisplay(void * value)
{
    const auto container = static_cast<CCArmatureDisplayContainer*>(this->_armature->getDisplay());
    const auto prevDisplay = static_cast<cocos2d::Node*>(value);
    container->addChild(_renderDisplay, prevDisplay->getLocalZOrder());
    container->removeChild(prevDisplay);
}

void CCSlot::_removeDisplay()
{
    _renderDisplay->removeFromParent();
}

void CCSlot::_disposeDisplay(void * value)
{
    // TODO
}

void CCSlot::_updateVisible()
{
    this->_renderDisplay->setVisible(this->_parent->getVisible());
}

void CCSlot::_updateBlendMode()
{
    cocos2d::Sprite* spriteDisplay = dynamic_cast<cocos2d::Sprite*>(_renderDisplay);
    if (spriteDisplay)
    {
        switch (this->_blendMode)
        {
        case BlendMode::Normal:
            //spriteDisplay->setBlendFunc(cocos2d::BlendFunc::DISABLE);
            break;

        case BlendMode::Add:
        {
            const auto texture = spriteDisplay->getTexture();
            if (texture && texture->hasPremultipliedAlpha())
            {
                cocos2d::BlendFunc blendFunc = { GL_ONE, GL_ONE };
                spriteDisplay->setBlendFunc(blendFunc);
            }
            else
            {
                spriteDisplay->setBlendFunc(cocos2d::BlendFunc::ADDITIVE);
            }
            break;
        }

        default:
            break;
        }
    }
    else if (this->_childArmature)
    {
        for (const auto slot : this->_childArmature->getSlots())
        {
            slot->_blendMode = this->_blendMode;
            slot->_updateBlendMode();
        }
    }
}

void CCSlot::_updateColor()
{
    /*_renderDisplay->setOpacity(this->_colorTransform.alphaMultiplier * 255.f);
    _helpColor.r = this->_colorTransform.redMultiplier * 255.f;
    _helpColor.g = this->_colorTransform.greenMultiplier * 255.f;
    _helpColor.b = this->_colorTransform.blueMultiplier * 255.f;
    _renderDisplay->setColor(_helpColor);*/
}

void CCSlot::_updateFilters() 
{
}

void CCSlot::_updateFrame()
{
    const auto frameDisplay = static_cast<cocos2d::Sprite*>(this->_rawDisplay);
    const unsigned displayIndex = this->_displayIndex;

    if (this->_display && displayIndex >= 0 && displayIndex < this->_displayDataSet->displays.size())
    {
        const auto displayData = this->_displayDataSet->displays[displayIndex];
        const auto textureData = static_cast<CCTextureData*>(displayData->textureData);

        if (textureData && !textureData->texture)
        {
            const auto textureAtlasTexture = static_cast<CCTextureAtlasData*>(textureData->parent)->texture;
            if (textureAtlasTexture)
            {
                cocos2d::Rect rect(textureData->region.x, textureData->region.y, textureData->region.width, textureData->region.height);
                cocos2d::Vec2 offset(0.f, 0.f);
                cocos2d::Size originSize(textureData->region.width, textureData->region.height);
                textureData->texture = cocos2d::SpriteFrame::createWithTexture(textureAtlasTexture, rect, textureData->rotated, offset, originSize);
                textureData->texture->retain();
            }
        }

        if (textureData && textureData->texture)
        {
            const auto& rect = textureData->frame ? *textureData->frame : textureData->region;

            auto width = rect.width;
            auto height = rect.height;
            if (textureData->rotated)
            {
                width = rect.height;
                height = rect.width;
            }

            if (this->_meshData && this->_display == this->_meshDisplay)
            {
                const auto& offset = textureData->texture->getRect().origin;
                const auto& textureSize = textureData->texture->getTexture()->getContentSize();
                auto displayVertices = new cocos2d::V3F_C4B_T2F[this->_meshData->uvs.size()];
                auto vertexIndices = new unsigned short[this->_meshData->vertexIndices.size()];

                for (std::size_t i = 0, l = this->_meshData->uvs.size(); i < l; i += 2)
                {
                    const auto iH = unsigned(i / 2);
                    cocos2d::V3F_C4B_T2F vertexData;
                    vertexData.vertices.set(this->_meshData->vertices[i] + width * 0.5f, height * 0.5f - this->_meshData->vertices[i + 1], 0.f);
                    vertexData.texCoords.u = (offset.x + this->_meshData->uvs[i] * width) / textureSize.width;
                    vertexData.texCoords.v = (offset.y + this->_meshData->uvs[i + 1] * height) / textureSize.height;
                    vertexData.colors = cocos2d::Color4B::WHITE;
                    displayVertices[iH] = vertexData;
                }

                for (std::size_t i = 0, l = this->_meshData->vertexIndices.size(); i < l; ++i)
                {
                    vertexIndices[i] = this->_meshData->vertexIndices[i];
                }

                cocos2d::PolygonInfo polygonInfo;
                auto& triangles = polygonInfo.triangles;
                triangles.verts = displayVertices;
                triangles.indices = vertexIndices;
                triangles.vertCount = unsigned(this->_meshData->uvs.size() / 2);
                triangles.indexCount = unsigned(this->_meshData->vertexIndices.size() / 3);
                polygonInfo.rect.setRect(0.f, 0.f, width, height);
                frameDisplay->setPolygonInfo(polygonInfo);
                frameDisplay->setSpriteFrame(textureData->texture);
                frameDisplay->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));

                if (this->_meshData->skinned)
                {
                    frameDisplay->setScale(1.f, 1.f);
                    frameDisplay->setRotationSkewX(0.f);
                    frameDisplay->setRotationSkewY(0.f);
                    frameDisplay->setPosition(0.f, 0.f);
                    frameDisplay->setAnchorPoint(cocos2d::Vec2::ZERO);
                }
                else
                {
                    frameDisplay->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
                }
            }
            else
            {
                cocos2d::Vec2 pivot(displayData->pivot.x, displayData->pivot.y);

                if (displayData->isRelativePivot)
                {
                    pivot.x *= width;
                    pivot.y *= height;
                }

                if (textureData->frame)
                {
                    pivot.x -= textureData->frame->x;
                    pivot.y -= textureData->frame->y;
                }

                if (textureData->rotated)
                {
                    pivot.x = pivot.x / textureData->region.height;
                    pivot.y = 1.f - pivot.y / textureData->region.width;
                }
                else
                {
                    pivot.x = pivot.x / textureData->region.width;
                    pivot.y = 1.f - pivot.y / textureData->region.height;
                }

                frameDisplay->setSpriteFrame(textureData->texture);
                frameDisplay->setAnchorPoint(pivot);
            }

            this->_updateVisible();

            return;
        }
    }

    frameDisplay->setTexture(nullptr);
    frameDisplay->setVisible(false);
    frameDisplay->setTextureRect(cocos2d::Rect::ZERO);
    frameDisplay->setAnchorPoint(cocos2d::Vec2::ZERO);
}

void CCSlot::_updateMesh() 
{
    const auto meshDisplay = static_cast<cocos2d::Sprite*>(_renderDisplay);
    const auto hasFFD = !this->_ffdVertices.empty();
    const auto& textureRect = meshDisplay->getTextureRect();
    const auto width = textureRect.size.width;
    const auto height = textureRect.size.height;
    const auto displayVertices = meshDisplay->getPolygonInfo().triangles.verts;

    if (this->_meshData->skinned)
    {
        std::size_t iF = 0;
        for (std::size_t i = 0, l = this->_meshData->vertices.size(); i < l; i += 2)
        {
            const auto iH = unsigned(i / 2);

            const auto& boneIndices = this->_meshData->boneIndices[iH];
            const auto& boneVertices = this->_meshData->boneVertices[iH];
            const auto& weights = this->_meshData->weights[iH];

            float xG = 0.f, yG = 0.f;
            for (std::size_t iB = 0, lB = boneIndices.size(); iB < lB; ++iB)
            {
                const auto bone = this->_meshBones[boneIndices[iB]];
                const auto matrix = bone->globalTransformMatrix;
                const auto weight = weights[iB];

                float xL = 0.f, yL = 0.f;
                if (hasFFD)
                {
                    xL = boneVertices[iB * 2] + this->_ffdVertices[iF];
                    yL = boneVertices[iB * 2 + 1] + this->_ffdVertices[iF + 1];
                }
                else
                {
                    xL = boneVertices[iB * 2];
                    yL = boneVertices[iB * 2 + 1];
                }


                xG += (matrix->a * xL + matrix->c * yL + matrix->tx) * weight;
                yG += (matrix->b * xL + matrix->d * yL + matrix->ty) * weight;

                iF += 2;
            }

            displayVertices[iH].vertices.set(xG + width * 0.5f, height * 0.5f - yG, 0.f);
        }
    }
    else if (hasFFD)
    {
        const auto& vertices = _meshData->vertices;
        for (std::size_t i = 0, l = this->_meshData->vertices.size(); i < l; i += 2)
        {
            const auto iH = unsigned(i / 2);
            const auto xG = vertices[i];
            const auto yG = vertices[i + 1];

			displayVertices[iH].vertices.set(xG + width * 0.5f, height * 0.5f - yG, 0.f);
        }
    }
}

void CCSlot::_updateTransform()
{
    if (_renderDisplay)
    {
        _renderDisplay->setScale(this->global.scaleX, this->global.scaleY);
        _renderDisplay->setRotationSkewX(this->global.skewX * RADIAN_TO_ANGLE);
        _renderDisplay->setRotationSkewY(this->global.skewY * RADIAN_TO_ANGLE);
        _renderDisplay->setPosition(this->global.x, -this->global.y);
    }
}

NAMESPACE_DRAGONBONES_END
