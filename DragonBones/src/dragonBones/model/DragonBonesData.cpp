#include "DragonBonesData.h"
#include "UserData.h"
#include "ArmatureData.h"

DRAGONBONES_NAMESPACE_BEGIN

void DragonBonesData::_onClear()
{
    for (const auto& pair : armatures)
    {
        pair.second->returnToPool();
    }

    if (buffer != nullptr)
    {
        delete buffer;
    }

    if (userData != nullptr)
    {
        userData->returnToPool();
    }

    autoSearch = false;
    frameRate = 0;
    version = "";
    name = "";
    frameIndices.clear();
    cachedFrames.clear();
    armatureNames.clear();
    armatures.clear();
    buffer = nullptr;
    intArray = nullptr;
    floatArray = nullptr;
    frameIntArray = nullptr;
    frameFloatArray = nullptr;
    frameArray = nullptr;
    timelineArray = nullptr;
    userData = nullptr;

    // TODO ÐèÒªÊÍ·Å buffer
	// TODO lsc JS call delete
}

void DragonBonesData::addArmature(ArmatureData* value)
{
    if (armatures.find(value->name) != armatures.end()) 
    {
        DRAGONBONES_ASSERT(false, "Replace armature: " + value->name);
        armatures[value->name]->returnToPool();
    }

    value->parent = this;
    armatures[value->name] = value;
    armatureNames.push_back(value->name);
}

// For WebAssembly.
DragonBonesData* DragonBonesData::_dragonBoneData = nullptr;

void DragonBonesData::setDragonBoneData(DragonBonesData* value)
{
    _dragonBoneData = value;
}

DragonBonesData* DragonBonesData::getDragonBoneData()
{
    return _dragonBoneData;
}

void DragonBonesData::parseArrays(int16_t* intArray, float* floatArray, int16_t* frameIntArray, float* frameFloatArray, int16_t* frameArray, uint16_t* timelineArray)
{
    this->intArray = intArray;
    this->floatArray = floatArray;
    this->frameIntArray = frameIntArray;
    this->frameFloatArray = frameFloatArray;
    this->frameArray = frameArray;
    this->timelineArray = timelineArray;
}

void DragonBonesData::setBufferPtr(int16_t* intArray, float* floatArray, int16_t* frameIntArray, float* frameFloatArray, int16_t* frameArray, uint16_t* timelineArray)
{
    this->intArray = intArray;
    this->floatArray = floatArray;
    this->frameIntArray = frameIntArray;
    this->frameFloatArray = frameFloatArray;
    this->frameArray = frameArray;
    this->timelineArray = timelineArray;
}

DRAGONBONES_NAMESPACE_END
