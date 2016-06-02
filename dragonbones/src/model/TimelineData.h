#ifndef DRAGONBONES_TIMELINE_DATA_H
#define DRAGONBONES_TIMELINE_DATA_H

#include "../core/BaseObject.h"
#include "FrameData.h"

NAMESPACE_DRAGONBONES_BEGIN

class SkinData;
class SlotDisplayDataSet;

class TimelineData : public BaseObject
{
public:
    float scale;
    float offset;

    std::vector<FrameData*> frames;

    TimelineData();
    virtual ~TimelineData() = 0;

protected:
    virtual void _onClear() override;
};

class BoneTimelineData final : public TimelineData
{
    BIND_CLASS_TYPE(BoneTimelineData);

public:
    static Matrix* cacheFrame(std::vector<Matrix*>& cacheFrames, std::size_t cacheFrameIndex, const Matrix& globalTransformMatrix);

public:
    BoneData* bone;
    Transform originTransform;
    std::vector<Matrix*> cacheFrames;

    BoneTimelineData();
    ~BoneTimelineData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(BoneTimelineData);

protected:
    void _onClear() override;
};

class SlotTimelineData final : public TimelineData
{
    BIND_CLASS_TYPE(SlotTimelineData);

public:
    static Matrix* cacheFrame(std::vector<Matrix*>& cacheFrames, std::size_t cacheFrameIndex, const Matrix& globalTransformMatrix);

public:
    SlotData* slot;
    std::vector<Matrix*> cacheFrames;

    SlotTimelineData();
    ~SlotTimelineData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(SlotTimelineData);

protected:
    void _onClear() override;
};

class FFDTimelineData final : public TimelineData
{
    BIND_CLASS_TYPE(FFDTimelineData);

public:
    std::size_t displayIndex;
    SkinData* skin;
    SlotDisplayDataSet* slot;

    FFDTimelineData();
    ~FFDTimelineData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(FFDTimelineData);

protected:
    void _onClear() override;
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_TIMELINE_DATA_H
