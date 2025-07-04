#ifndef DRAGONBONES_OPENGL_FACTORY_H
#define DRAGONBONES_OPENGL_FACTORY_H

#include "dragonBones/factory/BaseFactory.h"
#include "dragonBones/model/TextureAtlasData.h"
#include "dragonBones/armature/IArmatureProxy.h"

DRAGONBONES_NAMESPACE_BEGIN

namespace opengl
{

// OpenGL specific texture data
class OpenGLTextureData : public TextureData
{
    BIND_CLASS_TYPE_A(OpenGLTextureData);

public:
    unsigned int textureID;

protected:
    void _onClear() override {
        TextureData::_onClear();
        textureID = 0;
    }
};

// OpenGL specific texture atlas data
class OpenGLTextureAtlasData : public TextureAtlasData
{
    BIND_CLASS_TYPE_A(OpenGLTextureAtlasData);

public:
    unsigned int textureID; // To hold the OpenGL texture ID

protected:
    void _onClear() override {
        TextureAtlasData::_onClear();
        textureID = 0;
    }

    TextureData* createTexture() const override {
        return BaseObject::borrowObject<OpenGLTextureData>();
    }
};

// OpenGL factory that also implements IArmatureProxy
class OpenGLFactory : public BaseFactory, public IArmatureProxy
{
protected:
    TextureAtlasData* _buildTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const override;
    Armature* _buildArmature(const BuildArmaturePackage& dataPackage) const override;
    Slot* _buildSlot(const BuildArmaturePackage& dataPackage, const SlotData* slotData, Armature* armature) const override;

public:
    OpenGLFactory();
    ~OpenGLFactory();

    void setDragonBones(DragonBones* dragonBones)
    {
        _dragonBones = dragonBones;
    }

    // IEventDispatcher interface
    void addDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener) override {}
    void removeDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener) override {}
    bool hasDBEventListener(const std::string& type) const override { return false; }
    void dispatchDBEvent(const std::string& type, EventObject* value) override {}

    // IArmatureProxy interface implementation
    void dbInit(Armature* armature) override {}
    void dbClear() override {}
    void dbUpdate() override {}
    void dispose(bool disposeProxy) override {}
    Armature* getArmature() const override { return nullptr; }
    Animation* getAnimation() const override { return nullptr; }
};

}  // namespace opengl

DRAGONBONES_NAMESPACE_END

#endif // DRAGONBONES_OPENGL_FACTORY_H 