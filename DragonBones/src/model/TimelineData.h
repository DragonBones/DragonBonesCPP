#ifndef DRAGONBONES_TIMELINE_DATA_H
#define DRAGONBONES_TIMELINE_DATA_H

#include "../core/BaseObject.h"
#include "FrameData.h"

NAMESPACE_DRAGONBONES_BEGIN

class SkinData;
class SlotDisplayDataSet;

template<class T>
class TimelineData : public BaseObject
{
public:
    float scale;
    float offset;

    std::vector<T*> frames;

    TimelineData() {}
    virtual ~TimelineData() {}

protected:
    virtual void _onClear() override
    {
        scale = 0.f;
        offset = 0.f;

        T* prevFrame = nullptr;
        for (const auto frame : frames)
        {
            if (prevFrame && frame != prevFrame)
            {
                prevFrame->returnToPool();
            }

            prevFrame = frame;
        }

        clearVector(frames);
    }
};

class BoneTimelineData final : public TimelineData<BoneFrameData>
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

class SlotTimelineData final : public TimelineData<SlotFrameData>
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

class FFDTimelineData final : public TimelineData<ExtensionFrameData>
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
