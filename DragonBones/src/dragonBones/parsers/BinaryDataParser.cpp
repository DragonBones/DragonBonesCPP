#include "BinaryDataParser.h"

DRAGONBONES_NAMESPACE_BEGIN

TimelineData* BinaryDataParser::_parseBinaryTimeline(TimelineType type, unsigned offset, TimelineData* timelineData)
{
    const auto timeline = timelineData != nullptr ? timelineData : BaseObject::borrowObject<TimelineData>();
    timeline->type = type;
    timeline->offset = offset;

    _timeline = timeline;

    const auto keyFrameCount = _timelineArray[timeline->offset + (unsigned)BinaryOffset::TimelineKeyFrameCount];
    if (keyFrameCount == 1) 
    {
        timeline->frameIndicesOffset = -1;
    }
    else 
    {
        const auto totalFrameCount = _animation->frameCount + 1; // One more frame than animation.
        const auto& frameIndices = &_data->frameIndices;
        timeline->frameIndicesOffset = frameIndices->size();
        frameIndices->resize(frameIndices->size() + totalFrameCount);

        for (
            std::size_t i = 0, iK = 0, frameStart = 0, frameCount = 0;
            i < totalFrameCount;
            ++i
        ) 
        {
            if (frameStart + frameCount <= i && iK < keyFrameCount) 
            {
                frameStart = _frameArray[_animation->frameOffset + _timelineArray[timeline->offset + (unsigned)BinaryOffset::TimelineFrameOffset + iK]];
                if (iK == keyFrameCount - 1) 
                {
                    frameCount = _animation->frameCount - frameStart;
                }
                else 
                {
                    frameCount = _frameArray[_animation->frameOffset + _timelineArray[timeline->offset + (unsigned)BinaryOffset::TimelineFrameOffset + iK + 1]] - frameStart;
                }

                iK++;
            }

            (*frameIndices)[timeline->frameIndicesOffset + i] = iK - 1;
        }
    }

    _timeline = nullptr;

    return timeline;
}

void BinaryDataParser::_parseMesh(const rapidjson::Value& rawData, MeshDisplayData& mesh)
{
    mesh.offset = rawData[OFFSET].GetUint();

    const auto weightOffset = _intArray[mesh.offset + (unsigned)BinaryOffset::MeshWeightOffset];
    if (weightOffset >= 0) 
    {
        const auto weight = BaseObject::borrowObject<WeightData>();
        const auto vertexCount = _intArray[mesh.offset + (unsigned)BinaryOffset::MeshVertexCount];
        const auto boneCount = (unsigned)_intArray[weightOffset + (unsigned)BinaryOffset::WeigthBoneCount];
        weight->offset = weightOffset;
        weight->bones.resize(boneCount);

        for (std::size_t i = 0; i < boneCount; ++i) 
        {
            const auto boneIndex = _intArray[weightOffset + (unsigned)BinaryOffset::WeigthBoneIndices + i];
            weight->bones[i] = _rawBones[boneIndex];
        }

        auto boneIndicesOffset = weightOffset + (unsigned)BinaryOffset::WeigthBoneIndices + boneCount;
        for (std::size_t i = 0, l = vertexCount; i < l; ++i)
        {
            const auto vertexBoneCount = (unsigned)_intArray[boneIndicesOffset++];
            weight->count += vertexBoneCount;
            boneIndicesOffset += vertexBoneCount;
        }

        mesh.weight = weight;
    }
}

BoundingBoxData* BinaryDataParser::_parseBoundingBox(const rapidjson::Value& rawData)
{
    const auto polygonBoundingBox = BaseObject::borrowObject<PolygonBoundingBoxData>();
    polygonBoundingBox->offset = rawData[OFFSET].GetUint();
    polygonBoundingBox->vertices = _floatArray;

    return polygonBoundingBox;
}

AnimationData* BinaryDataParser::_parseAnimation(const rapidjson::Value& rawData)
{
    const auto animation =  BaseObject::borrowObject<AnimationData>();
    animation->frameCount = std::max(_getNumber(rawData, DURATION, 1), 1);
    animation->playTimes = _getNumber(rawData, PLAY_TIMES, 1);
    animation->duration = (float)(animation->frameCount) / _armature->frameRate;
    animation->fadeInTime = _getNumber(rawData, FADE_IN_TIME, 0.0f);
    animation->scale = _getNumber(rawData, SCALE, 1.0f);
    animation->name = _getString(rawData, NAME, DEFAULT_NAME);
    if (animation->name.empty()) 
    {
        animation->name = DEFAULT_NAME;
    }

    // Offsets.
    const auto& offsets = rawData[OFFSET];
    animation->frameIntOffset = offsets[0].GetUint();
    animation->frameFloatOffset = offsets[1].GetUint();
    animation->frameOffset = offsets[2].GetUint();

    _animation = animation;

    if (rawData.HasMember(ACTION))
    {
        animation->actionTimeline = _parseBinaryTimeline(TimelineType::Action, rawData[ACTION].GetUint());
    }

    if (rawData.HasMember(Z_ORDER))
    {
        animation->zOrderTimeline = _parseBinaryTimeline(TimelineType::ZOrder, rawData[Z_ORDER].GetUint());
    }

    if (rawData.HasMember(BONE))
    {
        const auto& rawTimeliness = rawData[BONE];
        for (auto iterator = rawTimeliness.MemberBegin(); iterator != rawTimeliness.MemberEnd(); ++iterator)
        {
            const auto bone = _armature->getBone(iterator->name.GetString());
            if (bone == nullptr)
            {
                continue;
            }

            const auto& rawTimelines = *&(iterator->value);
            for (std::size_t i = 0, l = rawTimelines.Size(); i < l; i += 2) 
            {
                const auto timelineType = (TimelineType)rawTimelines[i].GetInt();
                const auto timelineOffset = rawTimelines[i + 1].GetUint();
                const auto timeline = _parseBinaryTimeline(timelineType, timelineOffset);
                _animation->addBoneTimeline(bone, timeline);
            }
        }
    }

    if (rawData.HasMember(SLOT))
    {
        const auto& rawTimeliness = rawData[SLOT];
        for (auto iterator = rawTimeliness.MemberBegin(); iterator != rawTimeliness.MemberEnd(); ++iterator)
        {
            const auto slot = _armature->getSlot(iterator->name.GetString());
            if (slot == nullptr) 
            {
                continue;
            }

            const auto& rawTimelines = *&(iterator->value);
            for (std::size_t i = 0, l = rawTimelines.Size(); i < l; i += 2)
            {
                const auto timelineType = (TimelineType)rawTimelines[i].GetInt();
                const auto timelineOffset = rawTimelines[i + 1].GetUint();
                const auto timeline = _parseBinaryTimeline(timelineType, timelineOffset);
                _animation->addSlotTimeline(slot, timeline);
            }
        }
    }

    _animation = nullptr;

    return animation;
}

void BinaryDataParser::_parseArray(const rapidjson::Value& rawData)
{
    const auto& offsets = rawData[OFFSET];

    _data->intArray = _intArray = (int16_t*)(_binary + offsets[0].GetUint());
    _data->floatArray = _floatArray = (float*)(_binary + offsets[2].GetUint());
    _data->frameIntArray = _frameIntArray = (int16_t*)(_binary + offsets[4].GetUint());
    _data->frameFloatArray = _frameFloatArray = (float*)(_binary + offsets[6].GetUint());
    _data->frameArray = _frameArray = (int16_t*)(_binary + offsets[8].GetUint());
    _data->timelineArray = _timelineArray = (uint16_t*)(_binary + offsets[10].GetUint());
}

DragonBonesData* BinaryDataParser::parseDragonBonesData(const char* rawData, float scale)
{
    DRAGONBONES_ASSERT(rawData != nullptr, "");

    if (
        rawData[0] != 'D' ||
        rawData[1] != 'B' ||
        rawData[2] != 'D' ||
        rawData[3] != 'T'
    ) 
    {
        DRAGONBONES_ASSERT(false, "Nonsupport data.");
        return nullptr;
    }

    const auto headerLength = (unsigned)(((uint32_t*)(rawData + 8))[0]);
    const auto headerBytes = rawData + 8 + 4;
    rapidjson::Document document;
    document.Parse(headerBytes, headerLength);

    _binary = rawData + 8 + 4 + headerLength;

    return JSONDataParser::_parseDragonBonesData(document, scale);
}

DRAGONBONES_NAMESPACE_END
