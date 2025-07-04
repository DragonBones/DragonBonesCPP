#include "opengl/OpenGLFactory.h"
#include "opengl/OpenGLSlot.h"
#include "dragonBones/armature/Armature.h"
#include <GLES2/gl2.h>

#define STB_IMAGE_IMPLEMENTATION
#include "thirdParty/stb/stb_image.h"

DRAGONBONES_NAMESPACE_BEGIN

namespace opengl
{

OpenGLFactory::OpenGLFactory() {}
OpenGLFactory::~OpenGLFactory() {}

TextureAtlasData* OpenGLFactory::_buildTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const
{
    if (textureAtlasData == nullptr) {
        textureAtlasData = BaseObject::borrowObject<OpenGLTextureAtlasData>();
    }
    
    if (textureAtlas)
    {
        // In this implementation, textureAtlas is a pointer to a struct containing png data and size
        auto* atlasInfo = static_cast<std::pair<void*, int>*>(textureAtlas);
        unsigned char* pngData = static_cast<unsigned char*>(atlasInfo->first);
        int pngSize = atlasInfo->second;

        int width, height, nrChannels;
        unsigned char *data = stbi_load_from_memory(pngData, pngSize, &width, &height, &nrChannels, 0);

        if (data)
        {
            auto* openGLAtlas = static_cast<OpenGLTextureAtlasData*>(textureAtlasData);
            
            glGenTextures(1, &openGLAtlas->textureID);
            glBindTexture(GL_TEXTURE_2D, openGLAtlas->textureID);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
            if (nrChannels == 4) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            } else if (nrChannels == 3) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }

            stbi_image_free(data);
        }
    }

    return textureAtlasData;
}

Armature* OpenGLFactory::_buildArmature(const BuildArmaturePackage& dataPackage) const
{
    const auto armature = BaseObject::borrowObject<Armature>();
    armature->init(dataPackage.armature, const_cast<OpenGLFactory*>(this), nullptr, _dragonBones);
    return armature;
}

Slot* OpenGLFactory::_buildSlot(const BuildArmaturePackage& dataPackage, const SlotData* slotData, Armature* armature) const
{
    const auto slot = BaseObject::borrowObject<OpenGLSlot>();
    slot->init(slotData, armature, slot, slot);
    return slot;
}

}  // namespace opengl

DRAGONBONES_NAMESPACE_END