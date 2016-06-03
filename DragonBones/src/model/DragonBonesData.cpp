#include "DragonBonesData.h"

NAMESPACE_DRAGONBONES_BEGIN

DragonBonesData::DragonBonesData()
{
    _onClear();
}
DragonBonesData::~DragonBonesData()
{
    _onClear();
}

void DragonBonesData::_onClear()
{
    autoSearch = false;
    frameRate = 0;
    name.clear();

    for (const auto& pair : armatures)
    {
        pair.second->returnToPool();
    }

    armatures.clear();
}

void DragonBonesData::addArmature(ArmatureData * value)
{
    if (value && !value->name.empty() && armatures.find(value->name) == armatures.end())
    {
        armatures[value->name] = value;
    }
    else
    {
        //throw new Error();
    }
}

NAMESPACE_DRAGONBONES_END
