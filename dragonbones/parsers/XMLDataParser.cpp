#include "XMLDataParser.h"
#include "ConstValues.h"

NAME_SPACE_DRAGON_BONES_BEGIN
bool XMLDataParser::getBoolean(const XMLElement &data, const char *key, bool defaultValue)
{
    if (data.FindAttribute(key))
    {
        const char *value = data.Attribute(key);
        
        if (
            strcmp(value, "0") == 0 ||
            strcmp(value, "NaN") == 0 ||
            strcmp(value, "") == 0 ||
            strcmp(value, "false") == 0 ||
            strcmp(value, "null") == 0 ||
            strcmp(value, "undefined") == 0
        )
        {
            return false;
        }
        else
        {
            return data.BoolAttribute(key);
        }
    }
    
    return defaultValue;
}

float XMLDataParser::getNumber(const XMLElement &data, const char *key, float defaultValue, float nanValue)
{
    if (data.FindAttribute(key))
    {
        const char *value = data.Attribute(key);
        
        if (
            strcmp(value, "NaN") == 0 ||
            strcmp(value, "") == 0 ||
            strcmp(value, "false") == 0 ||
            strcmp(value, "null") == 0 ||
            strcmp(value, "undefined") == 0
        )
        {
            return nanValue;
        }
        else
        {
            return data.FloatAttribute(key);
        }
    }
    
    return defaultValue;
}

XMLDataParser::XMLDataParser()
{
    _textureScale = 1.f;
    _armatureScale = 1.f;
    _frameRate = 30;
}
XMLDataParser::~XMLDataParser() {}

TextureAtlasData* XMLDataParser::parseTextureAtlasData(const void *rawTextureAtlasData, float scale) const
{
    _textureScale = scale;
    const XMLElement *textureAtlasXML = static_cast<const XMLElement*>(rawTextureAtlasData);
    TextureAtlasData *textureAtlasData = new TextureAtlasData();
    const char *name = textureAtlasXML->Attribute(ConstValues::A_NAME.c_str());
    textureAtlasData->name = name ? name : "";
    textureAtlasData->imagePath = textureAtlasXML->Attribute(ConstValues::A_IMAGE_PATH.c_str());
    const char *format = textureAtlasXML->Attribute(ConstValues::A_IMAGE_FORMAT.c_str());
    textureAtlasData->format = getPixelFormatByString(format ? format : "");
    
    for (const XMLElement *textureXML = textureAtlasXML->FirstChildElement(ConstValues::SUB_TEXTURE.c_str()); textureXML; textureXML = textureXML->NextSiblingElement(ConstValues::SUB_TEXTURE.c_str()))
    {
        TextureData *textureData = parseTextureData(textureXML);
        textureAtlasData->textureDataList.push_back(textureData);
    }
    
    return textureAtlasData;
}

TextureData* XMLDataParser::parseTextureData(const XMLElement *textureXML) const
{
    TextureData *textureData = new TextureData();
    textureData->name = textureXML->Attribute(ConstValues::A_NAME.c_str());
    textureData->rotated = textureXML->BoolAttribute(ConstValues::A_ROTATED.c_str());
    textureData->region.x = textureXML->FloatAttribute(ConstValues::A_X.c_str()) / _textureScale;
    textureData->region.y = textureXML->FloatAttribute(ConstValues::A_Y.c_str()) / _textureScale;
    textureData->region.width = textureXML->FloatAttribute(ConstValues::A_WIDTH.c_str()) / _textureScale;
    textureData->region.height = textureXML->FloatAttribute(ConstValues::A_HEIGHT.c_str()) / _textureScale;
    const float frameWidth = textureXML->FloatAttribute(ConstValues::A_FRAME_WIDTH.c_str()) / _textureScale;
    const float frameHeight = textureXML->FloatAttribute(ConstValues::A_FRAME_HEIGHT.c_str()) / _textureScale;
    
    if (frameWidth > 0 && frameHeight > 0)
    {
        textureData->frame = new Rectangle();
        textureData->frame->x = textureXML->FloatAttribute(ConstValues::A_FRAME_X.c_str()) / _textureScale;
        textureData->frame->y = textureXML->FloatAttribute(ConstValues::A_FRAME_Y.c_str()) / _textureScale;
        textureData->frame->width = frameWidth;
        textureData->frame->height = frameHeight;
    }
    
    return textureData;
}

DragonBonesData* XMLDataParser::parseDragonBonesData(const void *rawDragonBonesData, float scale) const
{
    _armatureScale = scale;
    const XMLElement *dragonBonesXML = static_cast<const XMLElement*>(rawDragonBonesData);
    std::string version = dragonBonesXML->Attribute(ConstValues::A_VERSION.c_str());
    // TODO
    /*
    switch(version)
    {
    
    }
    */
    _frameRate = dragonBonesXML->IntAttribute(ConstValues::A_FRAME_RATE.c_str());
    DragonBonesData *dragonBonesData = new DragonBonesData();
    dragonBonesData->name = dragonBonesXML->Attribute(ConstValues::A_NAME.c_str());
    
    for (const XMLElement *armatureXML = dragonBonesXML->FirstChildElement(ConstValues::ARMATURE.c_str()); armatureXML; armatureXML = armatureXML->NextSiblingElement(ConstValues::ARMATURE.c_str()))
    {
        ArmatureData *armatureData = parseArmatureData(armatureXML);
        dragonBonesData->armatureDataList.push_back(armatureData);
    }
    
    return dragonBonesData;
}

ArmatureData* XMLDataParser::parseArmatureData(const XMLElement *armatureXML) const
{
    ArmatureData *armatureData = new ArmatureData();
    armatureData->name = armatureXML->Attribute(ConstValues::A_NAME.c_str());
    
    for (const XMLElement *boneXML = armatureXML->FirstChildElement(ConstValues::BONE.c_str()); boneXML; boneXML = boneXML->NextSiblingElement(ConstValues::BONE.c_str()))
    {
        BoneData *boneData = parseBoneData(boneXML);
        armatureData->boneDataList.push_back(boneData);
    }
    
    for (const XMLElement *skinXML = armatureXML->FirstChildElement(ConstValues::SKIN.c_str()); skinXML; skinXML = skinXML->NextSiblingElement(ConstValues::SKIN.c_str()))
    {
        SkinData *skinData = parseSkinData(skinXML);
        armatureData->skinDataList.push_back(skinData);
    }
    
    transformArmatureData(armatureData);
    armatureData->sortBoneDataList();
    
    for (const XMLElement *animationXML = armatureXML->FirstChildElement(ConstValues::ANIMATION.c_str()); animationXML; animationXML = animationXML->NextSiblingElement(ConstValues::ANIMATION.c_str()))
    {
        AnimationData *animationData = parseAnimationData(animationXML, armatureData);
        armatureData->animationDataList.push_back(animationData);
    }
    
    for (const XMLElement *rectangleXML = armatureXML->FirstChildElement(ConstValues::RECTANGLE.c_str()); rectangleXML; rectangleXML = rectangleXML->NextSiblingElement(ConstValues::RECTANGLE.c_str()))
    {
        RectangleData *rectangleData = parseRectangleData(rectangleXML);
        armatureData->areaDataList.push_back(rectangleData);
    }
    
    for (const XMLElement *ellipseXML = armatureXML->FirstChildElement(ConstValues::ELLIPSE.c_str()); ellipseXML; ellipseXML = ellipseXML->NextSiblingElement(ConstValues::ELLIPSE.c_str()))
    {
        EllipseData *ellipseData = parseEllipseData(ellipseXML);
        armatureData->areaDataList.push_back(ellipseData);
    }
    
    return armatureData;
}

BoneData* XMLDataParser::parseBoneData(const XMLElement *boneXML) const
{
    BoneData *boneData = new BoneData();
    boneData->name = boneXML->Attribute(ConstValues::A_NAME.c_str());
    const char *parent = boneXML->Attribute(ConstValues::A_PARENT.c_str());
    
    if (parent)
    {
        boneData->parent = parent;
    }
    
    boneData->length = boneXML->FloatAttribute(ConstValues::A_LENGTH.c_str());
    boneData->inheritRotation = getBoolean(*boneXML, ConstValues::A_INHERIT_ROTATION.c_str(), true);
    boneData->inheritScale = getBoolean(*boneXML, ConstValues::A_INHERIT_SCALE.c_str(), false);

    const XMLElement *transformXML = boneXML->FirstChildElement(ConstValues::TRANSFORM.c_str());
    if (transformXML)
    {
        parseTransform(*transformXML, boneData->global);
    }

    boneData->transform = boneData->global;
    
    for (const XMLElement *rectangleXML = boneXML->FirstChildElement(ConstValues::RECTANGLE.c_str()); rectangleXML; rectangleXML = rectangleXML->NextSiblingElement(ConstValues::RECTANGLE.c_str()))
    {
        RectangleData *rectangleData = parseRectangleData(rectangleXML);
        boneData->areaDataList.push_back(rectangleData);
    }
    
    for (const XMLElement *ellipseXML = boneXML->FirstChildElement(ConstValues::ELLIPSE.c_str()); ellipseXML; ellipseXML = ellipseXML->NextSiblingElement(ConstValues::ELLIPSE.c_str()))
    {
        EllipseData *ellipseData = parseEllipseData(ellipseXML);
        boneData->areaDataList.push_back(ellipseData);
    }
    
    return boneData;
}

SkinData* XMLDataParser::parseSkinData(const XMLElement *skinXML) const
{
    SkinData *skinData = new SkinData();
    skinData->name = skinXML->Attribute(ConstValues::A_NAME.c_str());
    
    for (const XMLElement *slotXML = skinXML->FirstChildElement(ConstValues::SLOT.c_str()); slotXML; slotXML = slotXML->NextSiblingElement(ConstValues::SLOT.c_str()))
    {
        SlotData *slotData = parseSlotData(slotXML);
        skinData->slotDataList.push_back(slotData);
    }
    
    return skinData;
}

SlotData* XMLDataParser::parseSlotData(const XMLElement *slotXML) const
{
    SlotData *slotData = new SlotData();
    slotData->name = slotXML->Attribute(ConstValues::A_NAME.c_str());
    slotData->parent = slotXML->Attribute(ConstValues::A_PARENT.c_str());
    slotData->zOrder = slotXML->FloatAttribute(ConstValues::A_Z_ORDER.c_str());
    
    if (slotXML->FindAttribute(ConstValues::A_BLENDMODE.c_str()))
    {
        slotData->blendMode = getBlendModeByString(slotXML->Attribute(ConstValues::A_BLENDMODE.c_str()));
    }
    
    for (const XMLElement *displayXML = slotXML->FirstChildElement(ConstValues::DISPLAY.c_str()); displayXML; displayXML = displayXML->NextSiblingElement(ConstValues::DISPLAY.c_str()))
    {
        DisplayData *displayData = parseDisplayData(displayXML);
        slotData->displayDataList.push_back(displayData);
    }
    
    return slotData;
}

DisplayData* XMLDataParser::parseDisplayData(const XMLElement *displayXML) const
{
    DisplayData *displayData = new DisplayData();
    displayData->name = displayXML->Attribute(ConstValues::A_NAME.c_str());
    displayData->type = getDisplayTypeByString(displayXML->Attribute(ConstValues::A_TYPE.c_str()));

    const XMLElement *scalingGridXML = displayXML->FirstChildElement(ConstValues::SCALING_GRID.c_str());
    if (scalingGridXML)
    {
        displayData->scalingGrid = true;
        displayData->scalingGridLeft = scalingGridXML->IntAttribute(ConstValues::A_LEFT.c_str());
        displayData->scalingGridRight = scalingGridXML->IntAttribute(ConstValues::A_RIGHT.c_str());
        displayData->scalingGridTop = scalingGridXML->IntAttribute(ConstValues::A_TOP.c_str());
        displayData->scalingGridBottom = scalingGridXML->IntAttribute(ConstValues::A_BOTTOM.c_str());
    }
    else
    {
        displayData->scalingGrid = false;
    }

    const XMLElement *transformXML = displayXML->FirstChildElement(ConstValues::TRANSFORM.c_str());
    if (transformXML)
    {
        parseTransform(*transformXML, displayData->transform);
        parsePivot(*transformXML, displayData->pivot);
    }

    const XMLElement *textXML = displayXML->FirstChildElement(ConstValues::TEXT.c_str());
    if (textXML)
    {
        displayData->textData = new TextData();
        parseTextData(*textXML, *displayData->textData);
    }

    return displayData;
}

AnimationData* XMLDataParser::parseAnimationData(const XMLElement *animationXML, const ArmatureData *armatureData) const
{
    AnimationData *animationData = new AnimationData();
    animationData->name = animationXML->Attribute(ConstValues::A_NAME.c_str());
    animationData->frameRate = _frameRate;
    animationData->duration = (int)(round(animationXML->IntAttribute(ConstValues::A_DURATION.c_str()) * 1000.f / _frameRate));
    animationData->playTimes = animationXML->IntAttribute(ConstValues::A_LOOP.c_str());
    animationData->fadeTime = animationXML->FloatAttribute(ConstValues::A_FADE_IN_TIME.c_str());
    animationData->scale = getNumber(*animationXML, ConstValues::A_SCALE.c_str(), 1.f, 1.f);
    // use frame tweenEase, NaN
    // overwrite frame tweenEase, [-1, 0):ease in, 0:line easing, (0, 1]:ease out, (1, 2]:ease in out
    animationData->tweenEasing = getNumber(*animationXML, ConstValues::A_TWEEN_EASING.c_str(), USE_FRAME_TWEEN_EASING, USE_FRAME_TWEEN_EASING);
    animationData->autoTween = getBoolean(*animationXML, ConstValues::A_AUTO_TWEEN.c_str(), true);
    
    for (const XMLElement *frameXML = animationXML->FirstChildElement(ConstValues::FRAME.c_str()); frameXML; frameXML = frameXML->NextSiblingElement(ConstValues::FRAME.c_str()))
    {
        Frame *frame = parseMainFrame(frameXML);
        animationData->frameList.push_back(frame);
    }
    
    parseTimeline(*animationXML, *animationData);
    
    for (const XMLElement *timelineXML = animationXML->FirstChildElement(ConstValues::TIMELINE.c_str()); timelineXML; timelineXML = timelineXML->NextSiblingElement(ConstValues::TIMELINE.c_str()))
    {
        TransformTimeline *timeline = parseTransformTimeline(timelineXML, animationData->duration);
        animationData->timelineList.push_back(timeline);
    }
    
    addHideTimeline(animationData, armatureData);
    transformAnimationData(animationData, armatureData);
    return animationData;
}


TransformTimeline* XMLDataParser::parseTransformTimeline(const XMLElement *timelineXML, int duration) const
{
    TransformTimeline *timeline = new TransformTimeline();
    timeline->name = timelineXML->Attribute(ConstValues::A_NAME.c_str());
    timeline->scale = timelineXML->FloatAttribute(ConstValues::A_SCALE.c_str());
    timeline->offset = timelineXML->FloatAttribute(ConstValues::A_OFFSET.c_str());
    timeline->duration = duration;
    
    for (const XMLElement *frameXML = timelineXML->FirstChildElement(ConstValues::FRAME.c_str()); frameXML; frameXML = frameXML->NextSiblingElement(ConstValues::FRAME.c_str()))
    {
        TransformFrame *frame = parseTransformFrame(frameXML);
        timeline->frameList.push_back(frame);
    }
    
    parseTimeline(*timelineXML, *timeline);
    return timeline;
}

Frame* XMLDataParser::parseMainFrame(const XMLElement *frameXML) const
{
    Frame *frame = new Frame();
    parseFrame(*frameXML, *frame);
    return frame;
}

TransformFrame* XMLDataParser::parseTransformFrame(const XMLElement *frameXML) const
{
    TransformFrame *frame = new TransformFrame();
    parseFrame(*frameXML, *frame);

    frame->visible = !getBoolean(*frameXML, ConstValues::A_HIDE.c_str(), false);
    // NaN:no tween, 10:auto tween, [-1, 0):ease in, 0:line easing, (0, 1]:ease out, (1, 2]:ease in out
    frame->tweenEasing = getNumber(*frameXML, ConstValues::A_TWEEN_EASING.c_str(), AUTO_TWEEN_EASING, NO_TWEEN_EASING);
    frame->tweenRotate = frameXML->IntAttribute(ConstValues::A_TWEEN_ROTATE.c_str());
    frame->tweenScale = getBoolean(*frameXML, ConstValues::A_TWEEN_SCALE.c_str(), true);
    frame->displayIndex = frameXML->IntAttribute(ConstValues::A_DISPLAY_INDEX.c_str());
    frame->zOrder = getNumber(*frameXML, ConstValues::A_Z_ORDER.c_str(), 0.f, 0.f);

    const XMLElement *transformXML = frameXML->FirstChildElement(ConstValues::TRANSFORM.c_str());
    if (transformXML)
    {
        parseTransform(*transformXML, frame->global);
        parsePivot(*transformXML, frame->pivot);
    }

    // copy
    frame->transform = frame->global;
    frame->scaleOffset.x = getNumber(*frameXML, ConstValues::A_SCALE_X_OFFSET.c_str(), 0.f, 0.f);
    frame->scaleOffset.y = getNumber(*frameXML, ConstValues::A_SCALE_Y_OFFSET.c_str(), 0.f, 0.f);

    const XMLElement *colorTransformXML = frameXML->FirstChildElement(ConstValues::COLOR_TRANSFORM.c_str());
    if (colorTransformXML)
    {
        frame->color = new ColorTransform();
        parseColorTransform(*colorTransformXML, *frame->color);
    }
    
    return frame;
}

RectangleData* XMLDataParser::parseRectangleData(const XMLElement *rectangleXML) const
{
    RectangleData *rectangleData = new RectangleData();
    rectangleData->name = rectangleXML->Attribute(ConstValues::A_NAME.c_str());
    rectangleData->width = rectangleXML->FloatAttribute(ConstValues::A_WIDTH.c_str());
    rectangleData->height = rectangleXML->FloatAttribute(ConstValues::A_HEIGHT.c_str());

    const XMLElement *transformXML = rectangleXML->FirstChildElement(ConstValues::TRANSFORM.c_str());
    if (transformXML)
    {
        parseTransform(*transformXML, rectangleData->transform);
        parsePivot(*transformXML, rectangleData->pivot);
    }

    return rectangleData;
}

EllipseData* XMLDataParser:: parseEllipseData(const XMLElement *ellipseXML) const
{
    EllipseData *ellipseData = new EllipseData();
    ellipseData->name = ellipseXML->Attribute(ConstValues::A_NAME.c_str());
    ellipseData->width = ellipseXML->FloatAttribute(ConstValues::A_WIDTH.c_str());
    ellipseData->height = ellipseXML->FloatAttribute(ConstValues::A_HEIGHT.c_str());

    const XMLElement *transformXML = ellipseXML->FirstChildElement(ConstValues::TRANSFORM.c_str());
    if (transformXML)
    {
        parseTransform(*transformXML, ellipseData->transform);
        parsePivot(*transformXML, ellipseData->pivot);
    }

    return ellipseData;
}

void XMLDataParser::parseTimeline(const XMLElement &timelineXML, Timeline &timeline) const
{
    int position = 0;
    Frame *frame = nullptr;
    
    for (size_t i = 0, l = timeline.frameList.size(); i < l; ++i)
    {
        frame = timeline.frameList[i];
        frame->position = position;
        position += frame->duration;
    }
    
    if (frame)
    {
        frame->duration = timeline.duration - frame->position;
    }
}

void XMLDataParser::parseFrame(const XMLElement &frameXML, Frame &frame) const
{
    frame.duration = (int)(round(frameXML.IntAttribute(ConstValues::A_DURATION.c_str()) * 1000.f / _frameRate));
    
    if (frameXML.FindAttribute(ConstValues::A_ACTION.c_str()))
    {
        frame.action = frameXML.Attribute(ConstValues::A_ACTION.c_str());
    }
    
    if (frameXML.FindAttribute(ConstValues::A_EVENT.c_str()))
    {
        frame.event = frameXML.Attribute(ConstValues::A_EVENT.c_str());
    }
    
    if (frameXML.FindAttribute(ConstValues::A_SOUND.c_str()))
    {
        frame.sound = frameXML.Attribute(ConstValues::A_SOUND.c_str());
    }
}

void XMLDataParser::parseTransform(const XMLElement &transformXML, Transform &transform) const
{
    transform.x = transformXML.FloatAttribute(ConstValues::A_X.c_str()) / _armatureScale;
    transform.y = transformXML.FloatAttribute(ConstValues::A_Y.c_str()) / _armatureScale;
    transform.skewX = transformXML.FloatAttribute(ConstValues::A_SKEW_X.c_str()) * ANGLE_TO_RADIAN;
    transform.skewY = transformXML.FloatAttribute(ConstValues::A_SKEW_Y.c_str()) * ANGLE_TO_RADIAN;
    transform.scaleX = transformXML.FloatAttribute(ConstValues::A_SCALE_X.c_str());
    transform.scaleY = transformXML.FloatAttribute(ConstValues::A_SCALE_Y.c_str());
}

void XMLDataParser::parsePivot(const XMLElement &transformXML, Point &pivot) const
{
    pivot.x = transformXML.FloatAttribute(ConstValues::A_PIVOT_X.c_str()) / _armatureScale;
    pivot.y = transformXML.FloatAttribute(ConstValues::A_PIVOT_Y.c_str()) / _armatureScale;
}

void XMLDataParser::parseColorTransform(const XMLElement &colorTransformXML, ColorTransform &colorTransform) const
{
    colorTransform.alphaOffset = colorTransformXML.IntAttribute(ConstValues::A_ALPHA_OFFSET.c_str());
    colorTransform.redOffset = colorTransformXML.IntAttribute(ConstValues::A_RED_OFFSET.c_str());
    colorTransform.greenOffset = colorTransformXML.IntAttribute(ConstValues::A_GREEN_OFFSET.c_str());
    colorTransform.blueOffset = colorTransformXML.IntAttribute(ConstValues::A_BLUE_OFFSET.c_str());
    colorTransform.alphaMultiplier = colorTransformXML.FloatAttribute(ConstValues::A_ALPHA_MULTIPLIER.c_str()) * 0.01f;
    colorTransform.redMultiplier = colorTransformXML.FloatAttribute(ConstValues::A_RED_MULTIPLIER.c_str()) * 0.01f;
    colorTransform.greenMultiplier = colorTransformXML.FloatAttribute(ConstValues::A_GREEN_MULTIPLIER.c_str()) * 0.01f;
    colorTransform.blueMultiplier = colorTransformXML.FloatAttribute(ConstValues::A_BLUE_MULTIPLIER.c_str()) * 0.01f;
}

void XMLDataParser::parseTextData(const XMLElement &textXML, TextData &textData) const
{
    textData.bold = getBoolean(textXML, ConstValues::A_BOLD.c_str(), false);
    textData.italic = getBoolean(textXML, ConstValues::A_ITALIC.c_str(), false);

    textData.size = textXML.UnsignedAttribute(ConstValues::A_SIZE.c_str());

    const XMLElement *colorXML = textXML.FirstChildElement(ConstValues::COLOR.c_str());
    if (colorXML)
    {
        textData.alpha = colorXML->UnsignedAttribute(ConstValues::A_ALPHA.c_str());
        textData.red = colorXML->UnsignedAttribute(ConstValues::A_RED.c_str());
        textData.green = colorXML->UnsignedAttribute(ConstValues::A_GREEN.c_str());
        textData.blue = colorXML->UnsignedAttribute(ConstValues::A_BLUE.c_str());
    }

    textData.width = textXML.UnsignedAttribute(ConstValues::A_WIDTH.c_str());
    textData.height = textXML.UnsignedAttribute(ConstValues::A_HEIGHT.c_str());

    textData.face = textXML.Attribute(ConstValues::A_FACE.c_str());
    textData.text = textXML.Attribute(ConstValues::A_TEXT.c_str());

    textData.alignH = getAlignHType(textXML.Attribute(ConstValues::A_ALIGN_H.c_str()));
    textData.alignV = getAlignVType(textXML.Attribute(ConstValues::A_ALIGN_V.c_str()));
}
NAME_SPACE_DRAGON_BONES_END
