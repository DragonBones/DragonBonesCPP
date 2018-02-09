#ifndef DRAGONBONES_DEFORMVERTICES_H
#define DRAGONBONES_DEFORMVERTICES_H

#include "../core/BaseObject.h"

DRAGONBONES_NAMESPACE_BEGIN

class DeformVertices : public BaseObject
{
    BIND_CLASS_TYPE_A(DeformVertices);

public:
    bool verticesDirty;
    std::vector<float> vertices;
    std::vector<Bone*> bones;
    const VerticesData* verticesData;

protected:
    virtual void _onClear() override;

public:
    void init(const VerticesData* weightData, Armature* armature);
    bool isBonesUpdate() const;
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_DEFORMVERTICES_H