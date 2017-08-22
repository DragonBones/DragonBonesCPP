#include "CCSlot.h"
#include "CCTextureAtlasData.h"
#include "CCArmatureDisplay.h"

DRAGONBONES_NAMESPACE_BEGIN

void CCSlot::_onClear()
{
    Slot::_onClear();

    _renderDisplay = nullptr;
}

void CCSlot::_initDisplay(void* value)
{
    const auto renderDisplay = static_cast<cocos2d::Node*>(value);
    renderDisplay->retain();
}

void CCSlot::_disposeDisplay(void* value)
{
    const auto renderDisplay = static_cast<cocos2d::Node*>(value);
    renderDisplay->release();
}

void CCSlot::_onUpdateDisplay()
{
    if (_display != nullptr)
    {
        if (_childArmature != nullptr)
        {
            _renderDisplay = static_cast<cocos2d::Node*>(_display);
        }
        else
        {
            _renderDisplay = static_cast<cocos2d::Node*>(_display);
        }
    }
    else
    {
        _renderDisplay = static_cast<cocos2d::Node*>(_rawDisplay);
    }
}

void CCSlot::_addDisplay()
{
    const auto container = static_cast<CCArmatureDisplay*>(_armature->getDisplay());
    container->addChild(_renderDisplay);
}

void CCSlot::_replaceDisplay(void* value, bool isArmatureDisplay)
{
    const auto container = static_cast<CCArmatureDisplay*>(_armature->getDisplay());
    const auto prevDisplay = isArmatureDisplay ? static_cast<cocos2d::Node*>(value) : static_cast<cocos2d::Node*>(value);
    container->addChild(_renderDisplay, prevDisplay->getLocalZOrder());
    container->removeChild(prevDisplay);
}

void CCSlot::_removeDisplay()
{
    _renderDisplay->removeFromParent();
}

void CCSlot::_updateZOrder()
{
    if (_renderDisplay->getLocalZOrder() == _zOrder) 
    {
        return;
    }

    _renderDisplay->setLocalZOrder(_zOrder);
}

void CCSlot::_updateVisible()
{
    _renderDisplay->setVisible(_parent->getVisible());
}

void CCSlot::_updateBlendMode()
{
    cocos2d::Sprite* spriteDisplay = dynamic_cast<cocos2d::Sprite*>(_renderDisplay);
    if (spriteDisplay)
    {
        switch (_blendMode)
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
    else if (_childArmature)
    {
        for (const auto slot : _childArmature->getSlots())
        {
            slot->_blendMode = _blendMode;
            slot->_updateBlendMode();
        }
    }
}

void CCSlot::_updateColor()
{
    _renderDisplay->setOpacity(_colorTransform.alphaMultiplier * 255.0f);

    static cocos2d::Color3B helpColor;
    helpColor.r = _colorTransform.redMultiplier * 255.0f;
    helpColor.g = _colorTransform.greenMultiplier * 255.0f;
    helpColor.b = _colorTransform.blueMultiplier * 255.0f;

    _renderDisplay->setColor(helpColor);
}

void CCSlot::_updateFrame()
{
    const auto meshData = _display == _meshDisplay ? _meshData : nullptr;
    auto currentTextureData = static_cast<CCTextureData*>(_textureData);
    const auto frameDisplay = static_cast<DBCCSprite*>(_renderDisplay); // In cocos2dx render meshDisplay and frameDisplay are the same display

    if (_displayIndex >= 0 && _display !=nullptr && currentTextureData != nullptr)
    {
        // Update replaced texture atlas.
        if (_armature->getReplacedTexture() != nullptr && std::find(_rawDisplayDatas->cbegin(), _rawDisplayDatas->cend(), _displayData) != _rawDisplayDatas->cend())
        {
            auto currentTextureAtlasData = static_cast<CCTextureAtlasData*>(currentTextureData->parent);
            if (_armature->_replaceTextureAtlasData == nullptr)
            {
                currentTextureAtlasData = BaseObject::borrowObject<CCTextureAtlasData>();
                currentTextureAtlasData->copyFrom(*currentTextureData->parent);
                currentTextureAtlasData->setRenderTexture(static_cast<cocos2d::Texture2D*>(_armature->getReplacedTexture()));
                _armature->_replaceTextureAtlasData = currentTextureAtlasData;
            }
            else 
            {
                currentTextureAtlasData = static_cast<CCTextureAtlasData*>(_armature->_replaceTextureAtlasData);
            }

            currentTextureData = static_cast<CCTextureData*>(currentTextureAtlasData->getTexture(currentTextureData->name));
        }

        if (currentTextureData->spriteFrame != nullptr) 
        {
            const auto scale = _armature->armatureData->scale;

            if (meshData != nullptr) // Mesh.
            {
                const auto intArray = meshData->parent->parent->intArray;
                const auto floatArray = meshData->parent->parent->floatArray;
                const unsigned vertexCount = intArray[meshData->offset + (unsigned)BinaryOffset::MeshVertexCount];
                const unsigned triangleCount = intArray[meshData->offset + (unsigned)BinaryOffset::MeshTriangleCount];
                const unsigned verticesOffset = intArray[meshData->offset + (unsigned)BinaryOffset::MeshFloatOffset];
                const unsigned triangleOffset = meshData->offset + (unsigned)BinaryOffset::MeshVertexIndices;
                const unsigned uvOffset = verticesOffset + vertexCount * 2;

                const auto& region = currentTextureData->region;
                const auto& textureAtlasSize = currentTextureData->spriteFrame->getTexture()->getContentSizeInPixels();
                auto vertices = new cocos2d::V3F_C4B_T2F[vertexCount]; // does cocos2dx release it?
                auto vertexIndices = new unsigned short[triangleCount * 3]; // does cocos2dx release it?
                cocos2d::Rect boundsRect(999999.0f, 999999.0f, -999999.0f, -999999.0f);

                for (std::size_t i = 0, l = vertexCount * 2; i < l; i += 2) 
                {
                    const auto iH = i / 2;
                    const auto x = floatArray[verticesOffset + i];
                    const auto y = floatArray[verticesOffset + i + 1];
                    cocos2d::V3F_C4B_T2F vertexData;
                    vertexData.vertices.set(x, -y, 0.0f);
                    vertexData.texCoords.u = (region.x + floatArray[uvOffset + i] * region.width) / textureAtlasSize.width;
                    vertexData.texCoords.v = (region.y + floatArray[uvOffset + i + 1] * region.height) / textureAtlasSize.height;
                    vertexData.colors = cocos2d::Color4B::WHITE;
                    vertices[iH] = vertexData;

                    if (boundsRect.origin.x > x)
                    {
                        boundsRect.origin.x = x;
                    }

                    if (boundsRect.size.width < x)
                    {
                        boundsRect.size.width = x;
                    }

                    if (boundsRect.origin.y > -y)
                    {
                        boundsRect.origin.y = -y;
                    }

                    if (boundsRect.size.height < -y)
                    {
                        boundsRect.size.height = -y;
                    }
                }

                boundsRect.size.width -= boundsRect.origin.x;
                boundsRect.size.height -= boundsRect.origin.y;

                for (std::size_t i = 0; i < triangleCount * 3; ++i) 
                {
                    vertexIndices[i] = intArray[triangleOffset + i];
                }

                frameDisplay->setSpriteFrame(currentTextureData->spriteFrame); // polygonInfo will be override

                //
                cocos2d::PolygonInfo polygonInfo;
                auto& triangles = polygonInfo.triangles;
                triangles.verts = vertices;
                triangles.indices = vertexIndices;
                triangles.vertCount = vertexCount;
                triangles.indexCount = triangleCount * 3;
                const auto& transform = frameDisplay->getNodeToParentTransform(); // Backup transform. (Set rect and polygon will override transform).

#if COCOS2D_VERSION >= 0x00031400
                polygonInfo.setRect(boundsRect);
#else
                polygonInfo.rect = boundsRect; // Copy
                frameDisplay->setContentSize(boundsRect.size);
#endif
                frameDisplay->setNodeToParentTransform(transform);
                frameDisplay->setPolygonInfo(polygonInfo);
            }
            else // Normal texture.
            {
                _pivotY -= currentTextureData->region.height * scale; // TODO check frame
                frameDisplay->setSpriteFrame(currentTextureData->spriteFrame); // polygonInfo will be override
            }

            _visibleDirty = true;
            _blendModeDirty = true; // Relpace texture will override blendMode and color.
            _colorDirty = true;

            return;
        }
    }

    frameDisplay->setTexture(nullptr);
    frameDisplay->setTextureRect(cocos2d::Rect::ZERO);
    frameDisplay->setPosition(0.0f, 0.0f);
    frameDisplay->setVisible(false);
}

void CCSlot::_updateMesh() 
{
    const auto hasFFD = !_ffdVertices.empty();
    const auto meshData = _meshData;
    const auto weightData = meshData->weight;
    const auto meshDisplay = static_cast<DBCCSprite*>(_renderDisplay);
    const auto vertices = meshDisplay->getPolygonInfoModify().triangles.verts;
    cocos2d::Rect boundsRect(999999.0f, 999999.0f, -999999.0f, -999999.0f);

    if (weightData != nullptr)
    {
        const auto intArray = meshData->parent->parent->intArray;
        const auto floatArray = meshData->parent->parent->floatArray;
        const auto vertexCount = (std::size_t)intArray[meshData->offset + (unsigned)BinaryOffset::MeshVertexCount];
        const auto weightFloatOffset = (std::size_t)intArray[weightData->offset + (unsigned)BinaryOffset::WeigthFloatOffset];

        for (
            std::size_t i = 0, iD = 0, iB = weightData->offset + (unsigned)BinaryOffset::WeigthBoneIndices + weightData->bones.size(), iV = weightFloatOffset, iF = 0;
            i < vertexCount;
            ++i
        )
        {
            const auto boneCount = (std::size_t)intArray[iB++];
            auto xG = 0.0f, yG = 0.0f;
            for (std::size_t j = 0; j < boneCount; ++j)
            {
                const auto boneIndex = (unsigned)intArray[iB++];
                const auto bone = _meshBones[boneIndex];
                if (bone != nullptr) 
                {
                    const auto& matrix = bone->globalTransformMatrix;
                    const auto weight = floatArray[iV++];
                    auto xL = floatArray[iV++];
                    auto yL = floatArray[iV++];

                    if (hasFFD) 
                    {
                        xL += _ffdVertices[iF++];
                        yL += _ffdVertices[iF++];
                    }

                    xG += (matrix.a * xL + matrix.c * yL + matrix.tx) * weight;
                    yG += (matrix.b * xL + matrix.d * yL + matrix.ty) * weight;
                }
            }

            auto& vertex = vertices[i];
            auto& vertexPosition = vertex.vertices;

            vertexPosition.set(xG, -yG, 0.0f);

            if (boundsRect.origin.x > xG)
            {
                boundsRect.origin.x = xG;
            }

            if (boundsRect.size.width < xG)
            {
                boundsRect.size.width = xG;
            }

            if (boundsRect.origin.y > -yG)
            {
                boundsRect.origin.y = -yG;
            }

            if (boundsRect.size.height < -yG)
            {
                boundsRect.size.height = -yG;
            }
        }
    }
    else if (hasFFD)
    {
        const auto intArray = meshData->parent->parent->intArray;
        const auto floatArray = meshData->parent->parent->floatArray;
        const auto vertexCount = (std::size_t)intArray[meshData->offset + (unsigned)BinaryOffset::MeshVertexCount];
        const auto vertexOffset = (std::size_t)intArray[meshData->offset + (unsigned)BinaryOffset::MeshFloatOffset];

        for (std::size_t i = 0, l = vertexCount * 2; i < l; i += 2)
        {
            const auto iH = i / 2;
            const auto xG = floatArray[vertexOffset + i] + _ffdVertices[i];
            const auto yG = floatArray[vertexOffset + i + 1] + _ffdVertices[i + 1];

            auto& vertex = vertices[iH];
            auto& vertexPosition = vertex.vertices;

            vertexPosition.set(xG, -yG, 0.0f);

            if (boundsRect.origin.x > xG)
            {
                boundsRect.origin.x = xG;
            }

            if (boundsRect.size.width < xG)
            {
                boundsRect.size.width = xG;
            }

            if (boundsRect.origin.y > -yG)
            {
                boundsRect.origin.y = -yG;
            }

            if (boundsRect.size.height < -yG)
            {
                boundsRect.size.height = -yG;
            }
        }
    }

    boundsRect.size.width -= boundsRect.origin.x;
    boundsRect.size.height -= boundsRect.origin.y;

    auto polygonInfo = meshDisplay->getPolygonInfo();
    const auto& transform = meshDisplay->getNodeToParentTransform(); // Backup transform. (Set rect and polygon will override transform).

#if COCOS2D_VERSION >= 0x00031400
    polygonInfo.setRect(boundsRect);
#else
    polygonInfo.rect = boundsRect; // Copy
    meshDisplay->setContentSize(boundsRect.size);
#endif
    meshDisplay->setPolygonInfo(polygonInfo);
    meshDisplay->setNodeToParentTransform(transform);
}

void CCSlot::_updateTransform(bool isSkinnedMesh)
{
    static cocos2d::Mat4 transform; 
    if (isSkinnedMesh) // Identity transform.
    {
        transform.m[0] = 1.0f;
        transform.m[1] = 0.0f;
        transform.m[4] = 0.0f;
        transform.m[5] = DragonBones::yDown ? 1.0f : -1.0f;
        transform.m[12] = 0.0f;
        transform.m[13] = 0.0f;
    }
    else if (DragonBones::yDown)
    {
        transform.m[0] = globalTransformMatrix.a;
        transform.m[1] = -globalTransformMatrix.b;
        transform.m[4] = -globalTransformMatrix.c;
        transform.m[5] = globalTransformMatrix.d;
        transform.m[12] = globalTransformMatrix.tx - (globalTransformMatrix.a * _pivotX + globalTransformMatrix.c * _pivotY);
        transform.m[13] = -(globalTransformMatrix.ty - (globalTransformMatrix.b * _pivotX + globalTransformMatrix.d * _pivotY));
    }
    else 
    {
        transform.m[0] = globalTransformMatrix.a;
        transform.m[1] = globalTransformMatrix.b;
        transform.m[4] = -globalTransformMatrix.c;
        transform.m[5] = -globalTransformMatrix.d;
        transform.m[12] = globalTransformMatrix.tx - (globalTransformMatrix.a * _pivotX + globalTransformMatrix.c * _pivotY);
        transform.m[13] = (globalTransformMatrix.ty - (globalTransformMatrix.b * _pivotX + globalTransformMatrix.d * _pivotY));
    }

    _renderDisplay->setNodeToParentTransform(transform);
}

DRAGONBONES_NAMESPACE_END
