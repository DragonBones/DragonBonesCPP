#include "TimelineData.h"

NAMESPACE_DRAGONBONES_BEGIN

Matrix * BoneTimelineData::cacheFrame(std::vector<Matrix*>& cacheFrames, std::size_t cacheFrameIndex, const Matrix& globalTransformMatrix)
{
    const auto cacheMatrix = cacheFrames[cacheFrameIndex] = new Matrix();
    *cacheMatrix = globalTransformMatrix;

    return cacheMatrix;
}

BoneTimelineData::BoneTimelineData()
{
    _onClear();
}
BoneTimelineData::~BoneTimelineData()
{
    _onClear();
}

void BoneTimelineData::_onClear()
{
    TimelineData::_onClear();

    bone = nullptr;
    originTransform.identity();

    Matrix* prevMatrix = nullptr;
    for (const auto matrix : cacheFrames)
    {
        if (matrix)
        {
            if (prevMatrix && prevMatrix != matrix)
            {
                delete prevMatrix;
            }

            prevMatrix = matrix;
        }
    }

    clearVector(cacheFrames);
}

Matrix * SlotTimelineData::cacheFrame(std::vector<Matrix*>& cacheFrames, std::size_t cacheFrameIndex, const Matrix& globalTransformMatrix)
{
    const auto cacheMatrix = cacheFrames[cacheFrameIndex] = new Matrix();
    *cacheMatrix = globalTransformMatrix;

    return cacheMatrix;
}

SlotTimelineData::SlotTimelineData()
{
    _onClear();
}
SlotTimelineData::~SlotTimelineData()
{
    _onClear();
}

void SlotTimelineData::_onClear()
{
    TimelineData::_onClear();

    slot = nullptr;

    Matrix* prevMatrix = nullptr;
    for (const auto matrix : cacheFrames)
    {
        if (matrix)
        {
            if (prevMatrix && prevMatrix != matrix)
            {
                delete prevMatrix;
            }

            prevMatrix = matrix;
        }
    }

    clearVector(cacheFrames);
}

FFDTimelineData::FFDTimelineData()
{
    _onClear();
}
FFDTimelineData::~FFDTimelineData()
{
    _onClear();
}

void FFDTimelineData::_onClear()
{
    TimelineData::_onClear();

    displayIndex = 0;
    skin = nullptr;
    slot = nullptr;
}

NAMESPACE_DRAGONBONES_END
