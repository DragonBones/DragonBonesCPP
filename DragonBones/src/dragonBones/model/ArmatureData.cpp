#include "ArmatureData.h"
#include "UserData.h"
#include "DragonBonesData.h"
#include "ConstraintData.h"
#include "DisplayData.h"
#include "AnimationData.h"

DRAGONBONES_NAMESPACE_BEGIN

void ArmatureData::_onClear()
{
    for (const auto action : defaultActions)
    {
        action->returnToPool();
    }

    for (const auto action : actions)
    {
        action->returnToPool();
    }

    for (const auto& pair : bones)
    {
        pair.second->returnToPool();
    }

    for (const auto& pair : slots)
    {
        pair.second->returnToPool();
    }

    for (const auto& pair : skins)
    {
        pair.second->returnToPool();
    }

    for (const auto& pair : animations)
    {
        pair.second->returnToPool();
    }

    if (userData != nullptr)
    {
        userData->returnToPool();
    }

    type = ArmatureType::Armature;
    frameRate = 0;
    cacheFrameRate = 0;
    scale = 1.0f;
    name = "";
    aabb.clear();
    defaultActions.clear();
    actions.clear();
    bones.clear();
    slots.clear();
    skins.clear();
    animations.clear();
    parent = nullptr;
    defaultSkin = nullptr;
    defaultAnimation = nullptr;
    userData = nullptr;
    // TODO canvas
}

void ArmatureData::sortBones()
{
    const auto total = sortedBones.size();
    if (total <= 0)
    {
        return;
    }

    const auto sortHelper = sortedBones; // Copy.
    unsigned index = 0;
    unsigned count = 0;
    sortedBones.clear();
    while (count < total)
    {
        const auto bone = sortHelper[index++];
        if (index >= total)
        {
            index = 0;
        }

        if (std::find(sortedBones.cbegin(), sortedBones.cend(), bone) != sortedBones.cend())
        {
            continue;
        }

        if(!bone->constraints.empty())
        {
            auto flag = false;
            for(const auto constrait : bone->constraints)
            {
                if(std::find(sortedBones.cbegin(), sortedBones.cend(), constrait->target) == sortedBones.cend())
                {
                    flag = true;
                     break;
                }
            }

            if(flag)
            {
                continue;
            }
        }

        if (bone->parent != nullptr && std::find(sortedBones.cbegin(), sortedBones.cend(), bone->parent) == sortedBones.cend())
        {
            continue;
        }

        sortedBones.push_back(bone);
        count++;
    }
}

void ArmatureData::cacheFrames(unsigned value)
{
    if (cacheFrameRate > value) // TODO clear cache.
    {
        return;
    }

    cacheFrameRate = value;
    for (const auto& pair : animations)
    {
        pair.second->cacheFrames(cacheFrameRate);
    }
}

int ArmatureData::setCacheFrame(const Matrix& globalTransformMatrix, const Transform& transform)
{
    auto& dataArray = *&parent->cachedFrames;
    auto arrayOffset = dataArray.size();

    dataArray.resize(arrayOffset + 10);
    dataArray[arrayOffset] = globalTransformMatrix.a;
    dataArray[arrayOffset + 1] = globalTransformMatrix.b;
    dataArray[arrayOffset + 2] = globalTransformMatrix.c;
    dataArray[arrayOffset + 3] = globalTransformMatrix.d;
    dataArray[arrayOffset + 4] = globalTransformMatrix.tx;
    dataArray[arrayOffset + 5] = globalTransformMatrix.ty;
    dataArray[arrayOffset + 6] = transform.rotation;
    dataArray[arrayOffset + 7] = transform.skew;
    dataArray[arrayOffset + 8] = transform.scaleX;
    dataArray[arrayOffset + 9] = transform.scaleY;

    return arrayOffset;
}

void ArmatureData::getCacheFrame(Matrix& globalTransformMatrix, Transform& transform, unsigned arrayOffset)
{
    auto& dataArray = *&parent->cachedFrames;
    globalTransformMatrix.a = dataArray[arrayOffset];
    globalTransformMatrix.b = dataArray[arrayOffset + 1];
    globalTransformMatrix.c = dataArray[arrayOffset + 2];
    globalTransformMatrix.d = dataArray[arrayOffset + 3];
    globalTransformMatrix.tx = dataArray[arrayOffset + 4];
    globalTransformMatrix.ty = dataArray[arrayOffset + 5];
    transform.rotation = dataArray[arrayOffset + 6];
    transform.skew = dataArray[arrayOffset + 7];
    transform.scaleX = dataArray[arrayOffset + 8];
    transform.scaleY = dataArray[arrayOffset + 9];
    transform.x = globalTransformMatrix.tx;
    transform.y = globalTransformMatrix.ty;
}

void ArmatureData::addBone(BoneData* value)
{
    if (bones.find(value->name) != bones.cend()) 
    {
        DRAGONBONES_ASSERT(false, "Replace bone: " + value->name);
        bones[value->name]->returnToPool();
    }

    bones[value->name] = value;
    sortedBones.push_back(value);
}

void ArmatureData::addSlot(SlotData* value)
{
    if (slots.find(value->name) != slots.cend())
    {
        DRAGONBONES_ASSERT(false, "Replace slot: " + value->name);
        slots[value->name]->returnToPool();
    }

    slots[value->name] = value;
    sortedSlots.push_back(value);
}

void ArmatureData::addSkin(SkinData* value)
{
    if (skins.find(value->name) != skins.cend())
    {
        DRAGONBONES_ASSERT(false, "Replace skin: " + value->name);
        skins[value->name]->returnToPool();
    }

    skins[value->name] = value;
    if (defaultSkin == nullptr)
    {
        defaultSkin = value;
    }
}

void ArmatureData::addAnimation(AnimationData* value)
{
    if (animations.find(value->name) != animations.cend())
    {
        DRAGONBONES_ASSERT(false, "Replace animation: " + value->name);
        animations[value->name]->returnToPool();
    }

    value->parent = this;
    animations[value->name] = value;
    animationNames.push_back(value->name);
    if (defaultAnimation == nullptr)
    {
        defaultAnimation = value;
    }
}

void BoneData::_onClear()
{
    for (const auto constraint : constraints)
    {
        constraint->returnToPool();
    }

    if (userData != nullptr)
    {
        userData->returnToPool();
    }

    inheritTranslation = false;
    inheritRotation = false;
    inheritScale = false;
    inheritReflection = false;
    length = 0.0f;
    name = "";
    transform.identity();
    constraints.clear();
    parent = nullptr;
    userData = nullptr;
}

ColorTransform SlotData::DEFAULT_COLOR;
ColorTransform* SlotData::createColor()
{
    return new ColorTransform();
}

void SlotData::_onClear()
{
    if (userData != nullptr)
    {
        userData->returnToPool();
    }

    if (color != nullptr && color != &DEFAULT_COLOR)
    {
        delete color;
    }

    blendMode = BlendMode::Normal;
    displayIndex = 0;
    zOrder = 0;
    name = "";
    parent = nullptr;
    color = nullptr;
    userData = nullptr;
}

void SkinData::_onClear()
{
    for (const auto& pair : displays)
    {
        for (const auto display : pair.second)
        {
            if (display != nullptr) 
            {
                display->returnToPool();
            }
        }
    }

    name = "";
    displays.clear();
    skinSlotNames.clear();
}

void SkinData::addDisplay(const std::string& slotName, DisplayData* value)
{
    skinSlotNames.push_back(slotName);
    displays[slotName].push_back(value); // TODO clear prev
}

DisplayData* SkinData::getDisplay(const std::string& slotName, const std::string& displayName)
{
    const auto slotDisplays = getDisplays(slotName);
    if (slotDisplays != nullptr) 
    {
        for (const auto display : *slotDisplays)
        {
            if (display != nullptr && display->name == displayName)
            {
                return display;
            }
        }
    }

    return nullptr;
}

DRAGONBONES_NAMESPACE_END
