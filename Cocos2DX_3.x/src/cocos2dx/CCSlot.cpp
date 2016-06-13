#include "CCSlot.h"

DRAGONBONES_NAMESPACE_BEGIN

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
    std::vector<void*> disposeDisplayList;
    for (const auto& pair : this->_displayList)
    {
        if (pair.second == DisplayType::Armature)
        {
            static_cast<Armature*>(pair.first)->returnToPool();
        }
        else if (std::find(disposeDisplayList.cbegin(), disposeDisplayList.cend(), pair.first) != disposeDisplayList.cend())
        {
            disposeDisplayList.push_back(pair.first);
        }
    }

    for (const auto renderDisplay : disposeDisplayList)
    {
        this->_disposeDisplay(renderDisplay);
    }

    Slot::_onClear();

    _renderDisplay = nullptr;
}

void CCSlot::_initDisplay(void* value)
{
    auto renderDisplay = static_cast<cocos2d::Node*>(value);
    renderDisplay->retain();
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

void CCSlot::_disposeDisplay(void* value)
{
    auto renderDisplay = static_cast<cocos2d::Node*>(value);
    renderDisplay->release();
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
    _renderDisplay->setOpacity(this->_colorTransform.alphaMultiplier * 255.f);
    _helpColor.r = this->_colorTransform.redMultiplier * 255.f;
    _helpColor.g = this->_colorTransform.greenMultiplier * 255.f;
    _helpColor.b = this->_colorTransform.blueMultiplier * 255.f;
    _renderDisplay->setColor(_helpColor);
}

void CCSlot::_updateFilters() 
{
}

void CCSlot::_updateFrame()
{
    const auto frameDisplay = static_cast<cocos2d::Sprite*>(this->_rawDisplay);
    const unsigned displayIndex = this->_displayIndex;

    if (this->_display && displayIndex >= 0)
    {
        const auto rawDisplayData = displayIndex < this->_displayDataSet->displays.size() ? this->_displayDataSet->displays[displayIndex] : nullptr;
        const auto replaceDisplayData = displayIndex < this->_replaceDisplayDataSet.size() ? this->_replaceDisplayDataSet[displayIndex] : nullptr;
        const auto contentDisplayData = replaceDisplayData ? replaceDisplayData : rawDisplayData;
        const auto contentTextureData = static_cast<CCTextureData*>(contentDisplayData->textureData);

        if (contentTextureData)
        {
            if (!contentTextureData->texture)
            {
                const auto textureAtlasTexture = static_cast<CCTextureAtlasData*>(contentTextureData->parent)->texture;
                if (textureAtlasTexture)
                {
                    cocos2d::Rect rect(contentTextureData->region.x, contentTextureData->region.y, contentTextureData->region.width, contentTextureData->region.height);
                    cocos2d::Vec2 offset(0.f, 0.f);
                    cocos2d::Size originSize(contentTextureData->region.width, contentTextureData->region.height);
                    contentTextureData->texture = cocos2d::SpriteFrame::createWithTexture(textureAtlasTexture, rect, contentTextureData->rotated, offset, originSize);
                    contentTextureData->texture->retain();
                }
            }

            const auto currentTexture = this->_armature->_replaceTexture ? static_cast<cocos2d::Texture2D*>(this->_armature->_replaceTexture) : contentTextureData->texture->getTexture();

            if (currentTexture)
            {
                const auto& rect = contentTextureData->frame ? *contentTextureData->frame : contentTextureData->region;

                auto width = rect.width;
                auto height = rect.height;
                if (contentTextureData->rotated)
                {
                    width = rect.height;
                    height = rect.width;
                }

                if (this->_meshData && this->_display == this->_meshDisplay)
                {
                    const auto& offset = contentTextureData->texture->getRect().origin;
                    const auto textureSize = contentTextureData->texture->getTexture()->getContentSize();
                    auto displayVertices = new cocos2d::V3F_C4B_T2F[this->_meshData->uvs.size() / 2]; // does cocos2dx release it?
                    auto vertexIndices = new unsigned short[this->_meshData->vertexIndices.size()]; // does cocos2dx release it?

                    for (std::size_t i = 0, l = this->_meshData->uvs.size(); i < l; i += 2)
                    {
                        const auto iH = unsigned(i / 2);
                        cocos2d::V3F_C4B_T2F vertexData;
                        vertexData.vertices.set(this->_meshData->vertices[i], -this->_meshData->vertices[i + 1], 0.f);
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
                    triangles.indexCount = unsigned(this->_meshData->vertexIndices.size());
                    polygonInfo.rect.setRect(0.f, 0.f, width, height);

                    // In cocos2dx render meshDisplay and frameDisplay are the same display
                    frameDisplay->setSpriteFrame(contentTextureData->texture);
                    frameDisplay->setPolygonInfo(polygonInfo);
                    frameDisplay->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT); // cocos2d::Vec2::ZERO

                    if (currentTexture != contentTextureData->texture->getTexture())
                    {
                        frameDisplay->setTexture(currentTexture); // relpace texture
                    }

                    if (this->_meshData->skinned)
                    {
                        frameDisplay->setScale(1.f, 1.f);
                        frameDisplay->setRotationSkewX(0.f);
                        frameDisplay->setRotationSkewY(0.f);
                        frameDisplay->setPosition(0.f, 0.f);
                    }
                }
                else
                {
                    cocos2d::Vec2 pivot(contentDisplayData->pivot.x, contentDisplayData->pivot.y);

                    if (contentDisplayData->isRelativePivot)
                    {
                        pivot.x *= width;
                        pivot.y *= height;
                    }

                    if (contentTextureData->frame)
                    {
                        pivot.x -= contentTextureData->frame->x;
                        pivot.y -= contentTextureData->frame->y;
                    }

                    if (rawDisplayData && replaceDisplayData)
                    {
                        pivot.x += replaceDisplayData->transform.x - rawDisplayData->transform.x;
                        pivot.y += replaceDisplayData->transform.y - rawDisplayData->transform.y;
                    }

                    if (contentTextureData->rotated)
                    {
                        pivot.x = pivot.x / contentTextureData->region.height;
                        pivot.y = 1.f - pivot.y / contentTextureData->region.width;
                    }
                    else
                    {
                        pivot.x = pivot.x / contentTextureData->region.width;
                        pivot.y = 1.f - pivot.y / contentTextureData->region.height;
                    }

                    frameDisplay->setSpriteFrame(contentTextureData->texture);
                    frameDisplay->setAnchorPoint(pivot);

                    if (currentTexture != contentTextureData->texture->getTexture())
                    {
                        frameDisplay->setTexture(currentTexture); // relpace texture
                    }
                }

                this->_updateVisible();

                return;
            }
        }
    }

    frameDisplay->setTexture(nullptr);
    frameDisplay->setVisible(false);
    frameDisplay->setTextureRect(cocos2d::Rect::ZERO);
    frameDisplay->setAnchorPoint(cocos2d::Vec2::ZERO);
}

void CCSlot::_updateMesh() 
{
    const auto meshDisplay = static_cast<cocos2d::Sprite*>(this->_meshDisplay);
    const auto hasFFD = !this->_ffdVertices.empty();
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

            auto& vertices = displayVertices[iH];
            auto& vertex = vertices.vertices;

            vertex.set(xG, - yG, 0.f);
        }
    }
    else if (hasFFD)
    {
        const auto& vertices = _meshData->vertices;
        for (std::size_t i = 0, l = this->_meshData->vertices.size(); i < l; i += 2)
        {
            const auto iH = unsigned(i / 2);
            const auto xG = vertices[i] + _ffdVertices[i];
            const auto yG = vertices[i + 1] + _ffdVertices[i + 1];

            auto& vertices = displayVertices[iH];
            auto& vertex = vertices.vertices;

            vertex.set(xG, - yG, 0.f);
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

DRAGONBONES_NAMESPACE_END
