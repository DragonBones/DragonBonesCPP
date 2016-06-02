#ifndef DRAGONBONES_ANIMATION_DATA_H
#define DRAGONBONES_ANIMATION_DATA_H

#include "TimelineData.h"

NAMESPACE_DRAGONBONES_BEGIN

class AnimationData final : public TimelineData
{
    BIND_CLASS_TYPE(AnimationData);

public:
    bool hasAsynchronyTimeline;
    unsigned frameCount;
    unsigned playTimes;
    unsigned position;
    unsigned duration;
    float fadeInTime;
    float cacheTimeToFrameScale;
    std::string name;
    AnimationData* animation;
    std::map<std::string, BoneTimelineData*> boneTimelines;
    std::map<std::string, SlotTimelineData*> slotTimelines;
    //std::map<std::string, std::map<std::string, std::map<std::string, FFDTimelineData*>>> ffdTimelines; // skin slot displayIndex
    std::vector<bool> cacheFrames;

    AnimationData();
    ~AnimationData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(AnimationData);

protected:
    void _onClear() override;

public:
    void cacheFrame(float value);
    void addBoneTimeline(BoneTimelineData* value);
    void addSlotTimeline(SlotTimelineData* value);
    void addFFDTimeline(FFDTimelineData* value);

    inline BoneTimelineData* getBoneTimeline(const std::string& name) const
    {
        return mapFind(boneTimelines, name);
    }

    inline SlotTimelineData* getSlotTimeline(const std::string& name) const
    {
        return mapFind(slotTimelines, name);
    }

    inline FFDTimelineData* getFFDTimeline(const std::string& skinName, const std::string& slotName, unsigned displayIndex) const
    {
        /*if (ffdTimelines.find(skinName) != ffdTimelines.end())
        {
            const auto &skin = ffdTimelines.at(skinName);
            if (skin.find(slotName) != skin.end())
            {
                const auto &slot = skin.at(slotName);
                if (slot.find(slotName) != slot.end())
                {
                    return slot.at(std::to_string(displayIndex));
                }
            }
        }*/

        return nullptr;
    }
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_ANIMATION_DATA_H
