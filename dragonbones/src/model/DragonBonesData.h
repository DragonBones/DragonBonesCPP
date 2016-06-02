#ifndef DRAGONBONES_DRAGON_BONES_DATA_H
#define DRAGONBONES_DRAGON_BONES_DATA_H

#include "../core/BaseObject.h"
#include "ArmatureData.h"

NAMESPACE_DRAGONBONES_BEGIN

class DragonBonesData final : public BaseObject
{
    BIND_CLASS_TYPE(DragonBonesData);

public:
    bool autoSearch;
    unsigned frameRate;
    std::string name;
    std::map<std::string, ArmatureData*> armatures;

    DragonBonesData();
    ~DragonBonesData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(DragonBonesData);

protected:
    void _onClear() override;

public:
    void addArmature(ArmatureData* value);

    inline ArmatureData* getArmature(const std::string& name) const
    {
        return mapFind(armatures, name);
    }
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_DRAGON_BONES_DATA_H