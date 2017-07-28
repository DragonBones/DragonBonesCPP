//
// Created by liangshuochen on 08/06/2017.
//

#include "DisplayData.h"
#include "UserData.h"
#include "BoundingBoxData.h"

DRAGONBONES_NAMESPACE_BEGIN

void DisplayData::_onClear()
{
    name = "";
    path = "";
    transform.identity();
    parent = nullptr;
}

void ImageDisplayData::_onClear()
{
    DisplayData::_onClear();

    type = DisplayType::Image;
    pivot.clear();
    texture = nullptr;
}

void ArmatureDisplayData::_onClear()
{
    DisplayData::_onClear();

    for(const auto action : actions)
    {
        action->returnToPool();
    }

    type = DisplayType::Armature;
    inheritAnimation = false;
    actions.clear();
    armature = nullptr;
}

void MeshDisplayData::_onClear()
{
    ImageDisplayData::_onClear();

    if(weight != nullptr)
    {
        weight->returnToPool();
    }

    type = DisplayType::Mesh;
    inheritAnimation = false;
    offset = 0;
    weight = nullptr;
}

void BoundingBoxDisplayData::_onClear()
{
    DisplayData::_onClear();

    if(boundingBox != nullptr)
    {
        boundingBox->returnToPool();
    }

    type = DisplayType::BoundingBox;
    boundingBox = nullptr;
}

void WeightData::_onClear()
{
    count = 0;
    offset = 0;
    bones.clear();
}

DRAGONBONES_NAMESPACE_END
