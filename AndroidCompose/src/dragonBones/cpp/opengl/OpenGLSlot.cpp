#include "opengl/OpenGLSlot.h"
#include "dragonBones/model/TextureAtlasData.h"
#include "dragonBones/armature/Armature.h"
#include "opengl/OpenGLFactory.h"
#include "dragonBones/model/DragonBonesData.h"
#include "dragonBones/model/DisplayData.h"
#include "dragonBones/armature/DeformVertices.h"
#include <android/log.h>
#include <string>
#include <cmath>

#define LOG_TAG "DragonBones_Mesh"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

DRAGONBONES_NAMESPACE_BEGIN

namespace opengl
{

void OpenGLSlot::_onClear()
{
    Slot::_onClear();
    vertices.clear();
    indices.clear();
    textureID = 0;
}

void OpenGLSlot::_initDisplay(void* value, bool isRetain) {}
void OpenGLSlot::_disposeDisplay(void* value, bool isRetain) {}
void OpenGLSlot::_onUpdateDisplay()
{
    // The responsibility of this method is to update the vertex buffer based on the current display data.
    // It should NOT modify _meshDisplay, which is set at initialization by the factory to act
    // as a persistent identifier for the mesh display object.

    if (_displayData != nullptr)
    {
        if (_displayData->type == DisplayType::Mesh)
        {
            // The current display is a mesh. Update the vertex buffer with mesh data.
            // The FFD logic in Slot::update() relies on _updateMesh() being called here.
            _updateMesh();
        }
        else
        {
            // The current display is a simple image or something else.
            // Update the vertex buffer for a simple quad.
            _updateFrame();
        }
    }
    else
    {
        // No display data, clear vertices to render nothing.
        vertices.clear();
        indices.clear();
        textureID = 0;
    }
}
void OpenGLSlot::_addDisplay() {}
void OpenGLSlot::_replaceDisplay(void* value, bool isRetain) {}
void OpenGLSlot::_removeDisplay() {}
void OpenGLSlot::_updateTransform() {}
void OpenGLSlot::_updateZOrder() {}

// Pure virtual methods that must be implemented
void OpenGLSlot::_updateVisible() {}
void OpenGLSlot::_updateBlendMode() {}
void OpenGLSlot::_updateColor() {}

// 新增方法的实现
void OpenGLSlot::_updateFrame()
{
    const auto* textureData = static_cast<const opengl::OpenGLTextureData*>(_textureData);
    if (!textureData) {
        vertices.clear();
        indices.clear();
        textureID = 0;
        return;
    }

    const auto* atlasData = static_cast<const opengl::OpenGLTextureAtlasData*>(textureData->parent);
    textureID = atlasData->textureID;

    // x, y, u, v
    vertices.resize(4 * 4); 
    const auto& region = textureData->region;
    const auto atlasWidth = atlasData->width;
    const auto atlasHeight = atlasData->height;

    float u_min = region.x / atlasWidth;
    float v_min = region.y / atlasHeight;
    float u_max = (region.x + region.width) / atlasWidth;
    float v_max = (region.y + region.height) / atlasHeight;

    float w = region.width;
    float h = region.height;

    // Vertex positions (Y-down)
    vertices[0] = -w / 2.0f; vertices[1] = h / 2.0f;   // Bottom-left
    vertices[4] = w / 2.0f;  vertices[5] = h / 2.0f;   // Bottom-right
    vertices[8] = -w / 2.0f; vertices[9] = -h / 2.0f;  // Top-left
    vertices[12] = w / 2.0f; vertices[13] = -h / 2.0f; // Top-right

    // UV coordinates
    vertices[2] = u_min; vertices[3] = v_max;
    vertices[6] = u_max; vertices[7] = v_max;
    vertices[10] = u_min; vertices[11] = v_min;
    vertices[14] = u_max; vertices[15] = v_min;

    indices = {0, 1, 2, 1, 3, 2};
}

void OpenGLSlot::_updateMesh()
{
    const auto scale = _armature->_armatureData->scale;
    const auto textureData = static_cast<const opengl::OpenGLTextureData*>(_textureData);
    if (!textureData)
    {
        vertices.clear();
        indices.clear();
        textureID = 0;
        isSkinned = false;
        return;
    }

    if (_displayData == nullptr || _displayData->type != DisplayType::Mesh)
    {
        _updateFrame();
        isSkinned = false;
        return;
    }

    const auto atlasData = static_cast<const opengl::OpenGLTextureAtlasData*>(textureData->parent);
    textureID = atlasData->textureID;
    
    const auto meshData = static_cast<const MeshDisplayData*>(_displayData);
    const auto& verticesData = meshData->vertices;
    const auto dragonBonesData = verticesData.data;
    const auto weightData = verticesData.weight;

    if (!dragonBonesData)
    {
        vertices.clear();
        indices.clear();
        textureID = 0;
        isSkinned = false;
        return;
    }

    const auto* intArray = dragonBonesData->intArray;
    const auto* floatArray = dragonBonesData->floatArray;

    const auto vertexCount = (std::size_t)intArray[verticesData.offset + (unsigned)BinaryOffset::MeshVertexCount];
    const auto triangleCount = (std::size_t)intArray[verticesData.offset + (unsigned)BinaryOffset::MeshTriangleCount];
    int vertexOffset = intArray[verticesData.offset + (unsigned)BinaryOffset::MeshFloatOffset];
    if (vertexOffset < 0) vertexOffset += 65536;
    const unsigned indexOffset = verticesData.offset + (unsigned)BinaryOffset::MeshVertexIndices;

    vertices.resize(vertexCount * 4);
    indices.resize(triangleCount * 3);

    const auto& deform = _deformVertices->vertices;
    const bool hasDeform = !deform.empty();
    
    isSkinned = weightData != nullptr;

    if (isSkinned)
    {
        const auto& bones = _deformVertices->bones;
        int weightFloatOffset = intArray[weightData->offset + (unsigned)BinaryOffset::WeigthFloatOffset];
        if (weightFloatOffset < 0) weightFloatOffset += 65536;

        for (
            std::size_t i = 0, iB = weightData->offset + (unsigned)BinaryOffset::WeigthBoneIndices + bones.size(), iV = (std::size_t)weightFloatOffset, iF = 0;
            i < vertexCount;
            ++i
        )
        {
            const auto boneCount = (std::size_t)intArray[iB++];
            auto xG = 0.0f, yG = 0.0f;

            for (std::size_t j = 0; j < boneCount; ++j)
            {
                const auto boneIndex = (unsigned)intArray[iB++];
                const auto bone = bones[boneIndex];
                if (bone != nullptr)
                {
                    const auto& matrix = bone->globalTransformMatrix;
                    const auto weight = floatArray[iV++];
                    auto xL = floatArray[iV++] * scale;
                    auto yL = floatArray[iV++] * scale;

                    if (hasDeform)
                    {
                        xL += deform[iF++];
                        yL += deform[iF++];
                    }

                    xG += (matrix.a * xL + matrix.c * yL + matrix.tx) * weight;
                    yG += (matrix.b * xL + matrix.d * yL + matrix.ty) * weight;
                }
            }
            
            const auto iV_out = i * 4;
            vertices[iV_out] = xG;
            vertices[iV_out + 1] = yG;
        }
    }
    else
    {
        std::size_t deform_idx = 0;
        for (std::size_t i = 0, l = vertexCount * 2; i < l; i += 2)
        {
            const auto iH = i / 2;
            const auto iV = iH * 4;

            const auto x_orig = floatArray[vertexOffset + i] * scale;
            const auto y_orig = floatArray[vertexOffset + i + 1] * scale;

            auto x = x_orig;
            auto y = y_orig;
            
            float deformX = 0.f;
            float deformY = 0.f;

            if (hasDeform)
            {
                if (deform_idx < deform.size()) {
                    deformX = deform[deform_idx];
                    x += deformX;
                }
                deform_idx++;

                if (deform_idx < deform.size()) {
                    deformY = deform[deform_idx];
                    y += deformY;
                }
                deform_idx++;
            }

            vertices[iV] = x;
            vertices[iV + 1] = y;
        }
    }

    const auto uvOffset = vertexOffset + vertexCount * 2;
    const auto& region = textureData->region;
    const auto atlasWidth = (float)atlasData->width;
    const auto atlasHeight = (float)atlasData->height;

    for (std::size_t i = 0, l = vertexCount; i < l; ++i)
    {
        const auto iD = i * 2;
        const auto iV = i * 4;

        const auto u = floatArray[uvOffset + iD];
        const auto v = floatArray[uvOffset + iD + 1];

        if (textureData->rotated)
        {
            vertices[iV + 2] = (region.x + (1.0f - v) * region.width) / atlasWidth;
            vertices[iV + 3] = (region.y + u * region.height) / atlasHeight;
        }
        else
        {
            vertices[iV + 2] = (region.x + u * region.width) / atlasWidth;
            vertices[iV + 3] = (region.y + v * region.height) / atlasHeight;
        }
    }
    
    for (std::size_t i = 0, l = triangleCount * 3; i < l; ++i)
    {
        indices[i] = intArray[indexOffset + i];
    }
}

void OpenGLSlot::_identityTransform() {}

}  // namespace opengl

DRAGONBONES_NAMESPACE_END 