#ifndef DRAGONBONES_BINARY_DATA_PARSER_H
#define DRAGONBONES_BINARY_DATA_PARSER_H

#include "JSONDataParser.h"

DRAGONBONES_NAMESPACE_BEGIN

class BinaryDataParser : public JSONDataParser
{
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(BinaryDataParser)

private:
    const char* _binary;
    int16_t* _intArray;
    float* _floatArray;
    int16_t* _frameIntArray;
    float* _frameFloatArray;
    int16_t* _frameArray;
    uint16_t* _timelineArray;

    TimelineData* _parseBinaryTimeline(TimelineType type, unsigned offset, TimelineData* timelineData = nullptr);

protected:
    virtual void _parseMesh(const rapidjson::Value& rawData, MeshDisplayData& mesh) override;
    virtual BoundingBoxData* _parseBoundingBox(const rapidjson::Value& rawData) override;
    virtual AnimationData* _parseAnimation(const rapidjson::Value& rawData) override;
    virtual void _parseArray(const rapidjson::Value& rawData) override;

public:
    BinaryDataParser() :
        _binary(nullptr),
        _intArray(nullptr),
        _floatArray(nullptr),
        _frameIntArray(nullptr),
        _frameFloatArray(nullptr),
        _frameArray(nullptr),
        _timelineArray(nullptr)
    {}
    virtual ~BinaryDataParser() {}

    virtual DragonBonesData* parseDragonBonesData(const char* rawData, float scale = 1.0f) override;
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_BINARY_DATA_PARSER_H
