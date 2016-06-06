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
            const auto& value = rawData[key];
            if (value.IsBool())
            {
                return value.GetBool();
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
            return value.IsNull() ? defaultValue : value.GetFloat();
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

    inline static const int _getParameter(const rapidjson::Value& rawData, std::size_t index, int defaultValue)
    {
        if (rawData.Size() > index)
        {
            return rawData[index].GetInt();
        }

        return defaultValue;
    }

    inline static const float _getParameter(const rapidjson::Value& rawData, std::size_t index, float defaultValue)
    {
        if (rawData.Size() > index)
        {
            return rawData[index].GetFloat();
        }

        return defaultValue;
    }

    inline static const char* _getParameter(const rapidjson::Value& rawData, std::size_t index, char* defaultValue)
    {
        if (rawData.Size() > index)
        {
            return rawData[index].GetString();
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
    virtual AnimationFrameData* _parseAnimationFrame(const rapidjson::Value& rawData, unsigned frameStart, unsigned frameCount) const;
    virtual BoneFrameData* _parseBoneFrame(const rapidjson::Value& rawData, unsigned frameStart, unsigned frameCount) const;
    virtual SlotFrameData* _parseSlotFrame(const rapidjson::Value& rawData, unsigned frameStart, unsigned frameCount) const;
    virtual ExtensionFrameData* _parseFFDFrame(const rapidjson::Value& rawData, unsigned frameStart, unsigned frameCount);
    virtual void _parseActionData(const rapidjson::Value& rawData, std::vector<ActionData*>& actions, BoneData* bone, SlotData* slot) const;
    virtual void _parseEventData(const rapidjson::Value& rawData, std::vector<EventData*>& events, BoneData* bone, SlotData* slot) const;

    template<class T>
    void _parseTweenFrame(const rapidjson::Value& rawData, TweenFrameData<T>& frame, unsigned frameStart, unsigned frameCount) const
    {
        _parseFrame(rawData, frame, frameStart, frameCount);

        frame.tweenEasing = _getNumber(rawData, TWEEN_EASING, NO_TWEEN);

        if (rawData.HasMember(CURVE))
        {
            const auto rawCurve = rawData[CURVE].GetArray();

            std::vector<float> curve;
            curve.reserve(rawCurve.Size());
            for (const auto& curveValue : rawCurve)
            {
                curve.push_back(curveValue.GetFloat());
            }

            TweenFrameData<T>::samplingCurve(curve, frameCount, frame.curve);
        }
    }

    template<class T>
    void _parseFrame(const rapidjson::Value& rawData, FrameData<T>& frame, unsigned frameStart, unsigned frameCount) const
    {
        frame.position = (unsigned)(frameStart * SECOND_TO_MICROSECOND / this->_armature->frameRate);
        frame.duration = (unsigned)(frameCount * SECOND_TO_MICROSECOND / this->_armature->frameRate);
    }

    template<class T>
    void _parseTimeline(const rapidjson::Value& rawData, TimelineData<T>& timeline, const std::function<T*(const rapidjson::Value& rawData, unsigned frameStart, unsigned frameCount)>& frameParser) const
    {
        timeline.scale = _getNumber(rawData, SCALE, 1.f);
        timeline.offset = _getNumber(rawData, OFFSET, 0.f);

        if (rawData.HasMember(FRAME))
        {
            const auto& rawFrames = rawData[FRAME].GetArray();
            if (!rawFrames.Empty())
            {
                if (rawFrames.Size() == 1)
                {
                    const auto& frameObject = rawFrames[0];
                    const auto frame = frameParser(frameObject, 0, _getNumber(frameObject, DURATION, (unsigned)1));
                    timeline.frames.reserve(1);
                    timeline.frames.push_back(frame);
                }
                else
                {
                    timeline.frames.reserve(this->_animation->frameCount + 1);

                    unsigned frameStart = 0;
                    unsigned frameCount = 0;
                    T* frame = nullptr;
                    T* prevFrame = nullptr;

                    for (std::size_t i = 0, iW = 0, l = this->_animation->frameCount + 1; i < l; ++i)
                    {
                        if (frameStart + frameCount <= i && iW < rawFrames.Size())
                        {
                            const auto& frameObject = rawFrames[iW++];
                            frameStart = i;
                            frameCount = _getNumber(frameObject, DURATION, 1);
                            frame = frameParser(frameObject, frameStart, frameCount);

                            if (prevFrame)
                            {
                                prevFrame->next = frame;
                                frame->prev = prevFrame;
                            }

                            prevFrame = frame;
                        }

                        timeline.frames.push_back(frame);
                    }

                    frame->duration = this->_animation->duration - frame->position;

                    frame = timeline.frames[0];
                    prevFrame->next = frame;
                    frame->prev = prevFrame;
                }
            }
        }
    }
    virtual void _parseTransform(const rapidjson::Value& rawData, Transform& transform) const;
    virtual void _parseColorTransform(const rapidjson::Value& rawData, ColorTransform& color) const;

public:
    virtual TextureAtlasData& parseTextureAtlasData(const char* rawData, TextureAtlasData& textureAtlasData, float scale) override;
    virtual DragonBonesData* parseDragonBonesData(const char* rawData) override;
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_JSON_DATA_PARSER_H