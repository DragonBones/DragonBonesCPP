#ifndef PARSERS_XML_DATA_PARSER_H
#define PARSERS_XML_DATA_PARSER_H

#include "../DragonBones.h"
#include "BaseDataParser.h"

#include "dbtinyxml2.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class XMLDataParser : public BaseDataParser
{
private:
    static bool getBoolean(const XMLElement &data, const char *key, bool defaultValue);
    static float getNumber(const XMLElement &data, const char *key, float defaultValue, float nanValue);
    
private:
    mutable float _textureScale;
    mutable float _armatureScale;
    mutable int _frameRate;
    
public:
    XMLDataParser();
    virtual ~XMLDataParser();
    
    virtual TextureAtlasData* parseTextureAtlasData(const void *rawTextureAtlasData, float scale = 1.f) const;
    
    virtual DragonBonesData* parseDragonBonesData(const void *rawDragonBonesData, float scale = 1.f) const;
    
private:
    TextureData* parseTextureData(const XMLElement *textureXML) const;
    
    ArmatureData* parseArmatureData(const XMLElement *armatureXML) const;
    BoneData* parseBoneData(const XMLElement *boneXML) const;
    SkinData* parseSkinData(const XMLElement *skinXML) const;
    SlotData* parseSlotData(const XMLElement *slotXML) const;
    DisplayData* parseDisplayData(const XMLElement *displayXML) const;
    AnimationData* parseAnimationData(const XMLElement *animationXML, const ArmatureData *armatureData) const;
    TransformTimeline* parseTransformTimeline(const XMLElement *timelineXML, int duration) const;
    Frame* parseMainFrame(const XMLElement *frameXML) const;
    TransformFrame* parseTransformFrame(const XMLElement *frameXML) const;
    RectangleData* parseRectangleData(const XMLElement *rectangleXML) const;
    EllipseData* parseEllipseData(const XMLElement *ellipseXML) const;
    
    void parseTimeline(const XMLElement &timelineXML, Timeline &timeline) const;
    void parseFrame(const XMLElement &frameXML, Frame &frame) const;
    void parseTransform(const XMLElement &transformXML, Transform &transform) const;
    void parsePivot(const XMLElement &transformXML, Point &pivot) const;
    void parseColorTransform(const XMLElement &colorTransformXML, ColorTransform &colorTransform) const;
    void parseTextData(const XMLElement &textXML, TextData &textData) const;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(XMLDataParser);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // PARSERS_XML_DATA_PARSER_H
