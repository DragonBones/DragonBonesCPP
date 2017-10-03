#ifndef DRAGONBONES_TIMELINE_DATA_H
#define DRAGONBONES_TIMELINE_DATA_H

#include "../core/BaseObject.h"
#include "FrameData.h"

DRAGONBONES_NAMESPACE_BEGIN

class SkinData;
class SlotDisplayDataSet;

/**
 * @private
 */
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
        scale = 1.f;
        offset = 0.f;

        std::vector<T*> deleteFrames;
        for (const auto frame : frames) {
            auto i = std::find(deleteFrames.begin(), deleteFrames.end(), frame);
            if (i == deleteFrames.end()) {
                deleteFrames.push_back(frame);
            }
        }

        for (const auto frame : deleteFrames) {
            frame->returnToPool();
        }

        frames.clear();
    }
};

/**
 * @private
 */
class BoneTimelineData final : public TimelineData<BoneFrameData>
{
    BIND_CLASS_TYPE(BoneTimelineData);

public:
    static Matrix* cacheFrame(std::vector<Matrix*>& cacheFrames, std::size_t cacheFrameIndex, const Matrix& globalTransformMatrix);

public:
    BoneData* bone;
    Transform originTransform;
    std::vector<Matrix*> cachedFrames;

    BoneTimelineData();
    ~BoneTimelineData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(BoneTimelineData);

protected:
    void _onClear() override;

public:
    /** @private */
    void cacheFrames(std::size_t cacheFrameCount);
};

/**
 * @private
 */
class SlotTimelineData final : public TimelineData<SlotFrameData>
{
    BIND_CLASS_TYPE(SlotTimelineData);

public:
    static Matrix* cacheFrame(std::vector<Matrix*>& cacheFrames, std::size_t cacheFrameIndex, const Matrix& globalTransformMatrix);

public:
    SlotData* slot;
    std::vector<Matrix*> cachedFrames;

    SlotTimelineData();
    ~SlotTimelineData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(SlotTimelineData);

protected:
    void _onClear() override;

public:
    /** @private */
    void cacheFrames(std::size_t cacheFrameCount);
};

/**
 * @private
 */
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

/**
* @private
*/
class ZOrderTimelineData final : public TimelineData<ZOrderFrameData>
{
    BIND_CLASS_TYPE(ZOrderTimelineData);

public:
    ZOrderTimelineData();
    ~ZOrderTimelineData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(ZOrderTimelineData);
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_TIMELINE_DATA_H
