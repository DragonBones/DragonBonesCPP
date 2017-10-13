#ifndef DRAGONBONES_ANIMATION_DATA_H
#define DRAGONBONES_ANIMATION_DATA_H

#include "ArmatureData.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* 动画数据。
* @version DragonBones 3.0
* @language zh_CN
*/
class AnimationData : public BaseObject
{
    BIND_CLASS_TYPE_B(AnimationData);

public:
    /**
    * @private
    */
    unsigned frameIntOffset;
    /**
    * @private
    */
    unsigned frameFloatOffset;
    /**
    * @private
    */
    unsigned frameOffset;
    /**
    * 持续的帧数。 ([1~N])
    * @version DragonBones 3.0
    * @language zh_CN
    */
    unsigned frameCount;
    /**
    * 播放次数。 [0: 无限循环播放, [1~N]: 循环播放 N 次]
    * @version DragonBones 3.0
    * @language zh_CN
    */
    unsigned playTimes;
    /**
    * 持续时间。 (以秒为单位)
    * @version DragonBones 3.0
    * @language zh_CN
    */
    float duration;
    /**
    * @private
    */
    float scale;
    /**
    * 淡入时间。 (以秒为单位)
    * @version DragonBones 3.0
    * @language zh_CN
    */
    float fadeInTime;
    /**
    * @private
    */
    float cacheFrameRate;
    /**
    * 数据名称。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::string name;
    /**
    * @private
    */
    std::vector<bool> cachedFrames;
    /**
    * @private
    */
    std::map<std::string, std::vector<TimelineData*>> boneTimelines;
    /**
    * @private
    */
    std::map<std::string, std::vector<TimelineData*>> slotTimelines;
    /**
    * @private
    */
    std::map<std::string, std::vector<int>> boneCachedFrameIndices;
    /**
    * @private
    */
    std::map<std::string, std::vector<int>> slotCachedFrameIndices;
    /**
    * @private
    */
    TimelineData* actionTimeline;
    /**
    * @private
    */
    TimelineData* zOrderTimeline;
    /**
    * @private
    */
    ArmatureData* parent;
    /**
    * @private
    */
    AnimationData() :
        actionTimeline(nullptr),
        zOrderTimeline(nullptr)
    {
        _onClear();
    }
    ~AnimationData()
    {
        _onClear();
    }

protected:
    virtual void _onClear() override;

public:
    /**
    * @private
    */
    void cacheFrames(unsigned frameRate);
    /**
    * @private
    */
    void addBoneTimeline(BoneData* bone, TimelineData* value);
    /**
    * @private
    */
    void addSlotTimeline(SlotData* slot, TimelineData* value);
    /**
    * @private
    */
    std::vector<TimelineData*>* getBoneTimelines(const std::string& name)
    {
        return mapFindB(boneTimelines, name);
    }
    /**
    * @private
    */
    inline std::vector<TimelineData*>* getSlotTimeline(const std::string& name)
    {
        return mapFindB(slotTimelines, name);
    }
    /**
    * @private
    */
    inline std::vector<int>* getBoneCachedFrameIndices(const std::string& name)
    {
        return mapFindB(boneCachedFrameIndices, name);
    }
    /**
    * @private
    */
    inline std::vector<int>* getSlotCachedFrameIndices(const std::string& name)
    {
        return mapFindB(slotCachedFrameIndices, name);
    }

public: // For WebAssembly.
    TimelineData* getActionTimeline(const std::string& name) const { return actionTimeline; }
    void setActionTimeline(TimelineData* pactionTimeline) { actionTimeline = pactionTimeline; }

    TimelineData* getZOrderTimeline(const std::string& name) const { return zOrderTimeline; }
    void setZOrderTimeline(TimelineData* value) { zOrderTimeline = value; }

    ArmatureData* getParent() const { return parent; }
    void setParent(ArmatureData* value) { parent = value; }
};
/**
* @private
*/
class TimelineData : public BaseObject
{
    BIND_CLASS_TYPE_A(TimelineData);

public:
    TimelineType type;
    unsigned offset;
    int frameIndicesOffset;

protected:
    virtual void _onClear() override;

public: // For WebAssembly.
    int getType() const { return (int)type; }
    void setType(int value) { type = (TimelineType)value; }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_ANIMATION_DATA_H
