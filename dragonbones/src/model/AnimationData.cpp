#include "AnimationData.h"
#include "ArmatureData.h"

NAMESPACE_DRAGONBONES_BEGIN

AnimationData::AnimationData() 
{
    _onClear();
}
AnimationData::~AnimationData() 
{
    _onClear();
}

void AnimationData::_onClear()
{
    TimelineData::_onClear();

    hasAsynchronyTimeline = false;
    frameCount = 0;
    playTimes = 0;
    position = 0;
    duration = 0;
    fadeInTime = 0.f;
    cacheTimeToFrameScale = 0.f;
    name.clear();
    animation = nullptr;

    for (const auto& pair : boneTimelines)
    {
        pair.second->returnToPool();
    }

    for (const auto& pair : slotTimelines)
    {
        pair.second->returnToPool();
    }

    /*for (const auto& skinPair : ffdTimelines)
    {
        for (const auto& slotPair : skinPair.second)
        {
            for (const auto pair : slotPair.second)
            {
                pair.second->returnToPool();
            }
        }
    }*/

    boneTimelines.clear();
    slotTimelines.clear();
    //ffdTimelines.clear();
    clearVector(cacheFrames);
}

void AnimationData::cacheFrame(float value)
{
    if (animation)
    {
        return;
    }

    const auto cacheFrameCount = (unsigned)(frameCount * scale * value);

    cacheTimeToFrameScale = (float)cacheFrameCount / (duration + 1);
    cacheFrames.resize(cacheFrameCount, false);

    for (const auto& pair : boneTimelines)
    {
        pair.second->cacheFrames.resize(cacheFrameCount, nullptr); // TODO
    }

    for (const auto& pair : slotTimelines)
    {
        pair.second->cacheFrames.resize(cacheFrameCount, nullptr); // TODO
    }
}

void AnimationData::addBoneTimeline(BoneTimelineData* value)
{
    if (value && value->bone && boneTimelines.find(value->bone->name) == boneTimelines.end())
    {
        boneTimelines[value->bone->name] = value;
    }
    else
    {
        // throw new Error();
    }
}

void AnimationData::addSlotTimeline(SlotTimelineData* value)
{
    if (value && value->slot && slotTimelines.find(value->slot->name) == slotTimelines.end())
    {
        slotTimelines[value->slot->name] = value;
    }
    else
    {
        // throw new Error();
    }
}

void AnimationData::addFFDTimeline(FFDTimelineData* value)
{
    if (value && value->skin && value->slot)
    {
        const auto& skinName = value->skin->name;
        const auto& slotName = value->slot->slot->name;
        const auto& displayIndex = std::to_string(value->displayIndex);

        /*if (ffdTimelines.find(skinName) == ffdTimelines.end())
        {
            std::make_pair(ffdTimelines, skinName);
        }*/
        /*if (skin.find(slotName) == skin.end())
        {
            std::make_pair(skin, slotName);
        }*/

        /*
        auto& skin = ffdTimelines[skinName];
        auto& slot = skin[slotName];
        if (slot.find(displayIndex) == slot.end())
        {
            slot[displayIndex] = value;
        }
        else
        {
            //throw new Error();
        }*/
    }
    else
    {
        //throw new Error();
    }
}

NAMESPACE_DRAGONBONES_END