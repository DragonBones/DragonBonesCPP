#ifndef DRAGONBONES_OPENGL_SLOT_H
#define DRAGONBONES_OPENGL_SLOT_H

#include "dragonBones/armature/Slot.h"

DRAGONBONES_NAMESPACE_BEGIN

namespace opengl
{

class OpenGLSlot : public Slot
{
    BIND_CLASS_TYPE_A(OpenGLSlot);

public:
    std::vector<float> vertices;
    std::vector<unsigned short> indices;
    unsigned int textureID;
    bool isSkinned = false;

protected:
    void _updateVisible() override;
    void _updateBlendMode() override;
    void _updateColor() override;

    void _onClear() override;
    void _initDisplay(void* value, bool isRetain) override;
    void _disposeDisplay(void* value, bool isRetain) override;
    void _onUpdateDisplay() override;
    void _addDisplay() override;
    void _replaceDisplay(void* value, bool isRetain) override;
    void _removeDisplay() override;
    void _updateTransform() override;
    void _updateZOrder() override;
    
    // 添加缺失的纯虚函数
    void _updateFrame() override;
    void _updateMesh() override;
    void _identityTransform() override;
};

}  // namespace opengl

DRAGONBONES_NAMESPACE_END

#endif // DRAGONBONES_OPENGL_SLOT_H