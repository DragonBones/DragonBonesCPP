#ifndef DRAGONBONES_JSON_DATA_PARSER_H
#define DRAGONBONES_JSON_DATA_PARSER_H

#include "DataParser.h"
#include "./rapidjson/document.h"

NAMESPACE_DRAGONBONES_BEGIN

class JSONDataParser : public DataParser
{
protected:
    inline static bool _getBoolean(const rapidjson::Value& rawData, const char*& key, bool defaultValue)
    {
        if (rawData.HasMember(key))
        {
            const auto&value = rawData[key];
            if (value.IsBool())
            {
                value.GetBool();
            }
            else if (value.IsNumber())
            {
                return value.GetInt() != 0;
            }
        }

        return defaultValue;
    }

    inline static unsigned _getNumber(const rapidjson::Value& rawData, const char*& key, unsigned defaultValue)
    {
        if (rawData.HasMember(key))
        {
            return rawData[key].GetUint();
        }

        return defaultValue;
    }

    inline static int _getNumber(const rapidjson::Value& rawData, const char*& key, int defaultValue)
    {
        if (rawData.HasMember(key))
        {
            return rawData[key].GetInt();
        }

        return defaultValue;
    }

    inline static float _getNumber(const rapidjson::Value& rawData, const char*& key, float defaultValue)
    {
        if (rawData.HasMember(key))
        {
            const auto& value = rawData[key];
            return value.IsNull() ? defaultValue : value.GetDouble();
        }

        return defaultValue;
    }

    inline static const char* _getString(const rapidjson::Value& rawData, const char*& key, const char* defaultValue)
    {
        if (rawData.HasMember(key))
        {
            return rawData[key].GetString();
        }

        return defaultValue;
    }

public:
    JSONDataParser();
    ~JSONDataParser();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(JSONDataParser);

protected:
    virtual ArmatureData* _parseArmature(const rapidjson::Value& rawData);
    virtual BoneData* _parseBone(const rapidjson::Value& rawData);
    virtual void _parseIK(const rapidjson::Value& rawData);
    virtual SlotData* _parseSlot(const rapidjson::Value& rawData);
    virtual SkinData* _parseSkin(const rapidjson::Value& rawData);
    virtual SlotDisplayDataSet* _parseSlotDisplaySet(const rapidjson::Value& rawData);
    virtual DisplayData* _parseDisplay(const rapidjson::Value& rawData);
    virtual MeshData* _parseMesh(const rapidjson::Value& rawData);
    virtual AnimationData* _parseAnimation(const rapidjson::Value& rawData);
    virtual BoneTimelineData* _parseBoneTimeline(const rapidjson::Value& rawData) const;
    virtual SlotTimelineData* _parseSlotTimeline(const rapidjson::Value& rawData) const;
    virtual FFDTimelineData* _parseFFDTimeline(const rapidjson::Value& rawData);
    virtual FrameData* _parseAnimationFrame(const rapidjson::Value& rawData, unsigned frameStart, unsigned frameCount) const;
    virtual FrameData* _parseBoneFrame(const rapidjson::Value& rawData, unsigned frameStart, unsigned frameCount) const;
    virtual FrameData* _parseSlotFrame(const rapidjson::Value& rawData, unsigned frameStart, unsigned frameCount) const;
    virtual FrameData* _parseFFDFrame(const rapidjson::Value& rawData, unsigned frameStart, unsigned frameCount);
    virtual void _parseTweenFrame(const rapidjson::Value& rawData, TweenFrameData& frame, unsigned frameStart, unsigned frameCount) const;
    virtual void _parseFrame(const rapidjson::Value& rawData, FrameData& frame, unsigned frameStart, unsigned frameCount) const;
    virtual void _parseTimeline(const rapidjson::Value& rawData, TimelineData& timeline, const std::function<FrameData*(const rapidjson::Value& rawData, unsigned frameStart, unsigned frameCount)>& frameParser) const;
    virtual void _parseTransform(const rapidjson::Value& rawData, Transform& transform) const;
    virtual void _parseColorTransform(const rapidjson::Value& rawData, ColorTransform& color) const;

public:
    virtual TextureAtlasData& parseTextureAtlasData(const char* rawData, TextureAtlasData& textureAtlasData, float scale) override;
    virtual DragonBonesData* parseDragonBonesData(const char* rawData) override;
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_JSON_DATA_PARSER_H