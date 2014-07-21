#include "XMLDataParser.h"

NAME_SPACE_DRAGON_BONES_BEGIN
bool XMLDataParser::getBoolean(const XMLElement *data, const String &key, bool defaultValue)
{
    return false;
}

float XMLDataParser::getNumber(const XMLElement *data, const String &key, float defaultValue, float nanValue)
{
    return 0.f;
}

/*
private static function getBoolean(data:XML, key:String, defaultValue:Boolean):Boolean
{
    if(data.@[key].length() > 0)
    {
        switch(String(data.@[key]))
        {
        case "0":
        case "NaN":
        case "":
        case "false":
        case "null":
        case "undefined":
            return false;

        case "1":
        case "true":
        default:
            return true;
        }
    }
    return defaultValue;
}

private static function getNumber(data:XML, key:String, defaultValue:Number):Number
{
    if(data.@[key].length() > 0)
    {
        switch(String(data.@[key]))
        {
        case "NaN":
        case "":
        case "false":
        case "null":
        case "undefined":
            return NaN;

        default:
            return Number(data.@[key]);
        }
    }
    return defaultValue;
}*/


XMLDataParser::XMLDataParser()
{
    _textureScale = 1;
    _skeletonScale = 1;
    _frameRate = 30;
}
XMLDataParser::~XMLDataParser() {}

TextureAtlasData *XMLDataParser::parseTextureAtlasData(const void *rawTextureAtlasData, float scale) const
{
    _textureScale = scale;
    const XMLElement *textureAtlasXML = static_cast<const XMLElement *>(rawTextureAtlasData);
    TextureAtlasData *textureAtlasData = new TextureAtlasData();
    textureAtlasData->name = textureAtlasXML->Attribute(A_NAME);
    textureAtlasData->imagePath = textureAtlasXML->Attribute(A_IMAGE_PATH);
    
    for (const XMLElement *textureXML = textureAtlasXML->FirstChildElement(SUB_TEXTURE); textureXML; textureXML = textureXML->NextSiblingElement(SUB_TEXTURE))
    {
        TextureData *textureData = parseTextureData(textureXML);
        textureAtlasData->textureDataList.push_back(textureData);
    }
    
    return textureAtlasData;
}

TextureData *XMLDataParser::parseTextureData(const XMLElement *textureXML) const
{
    TextureData *textureData = new TextureData();
    textureData->name = textureXML->Attribute(A_NAME);
    textureData->rotated = textureXML->BoolAttribute(A_ROTATED);
    textureData->region.x = (float)(textureXML->DoubleAttribute(A_X) / _textureScale);
    textureData->region.y = (float)(textureXML->DoubleAttribute(A_Y) / _textureScale);
    textureData->region.width = (float)(textureXML->DoubleAttribute(A_WIDTH) / _textureScale);
    textureData->region.height = (float)(textureXML->DoubleAttribute(A_HEIGHT) / _textureScale);
    float frameWidth = (float)(textureXML->DoubleAttribute(A_FRAME_WIDTH) / _textureScale);
    float frameHeight = (float)(textureXML->DoubleAttribute(A_FRAME_HEIGHT) / _textureScale);
    
    if (frameWidth > 0 && frameHeight > 0)
    {
        textureData->frame = new Rectangle();
        textureData->frame->x = (float)(textureXML->DoubleAttribute(A_FRAME_X) / _textureScale);
        textureData->frame->y = (float)(textureXML->DoubleAttribute(A_FRAME_Y) / _textureScale);
        textureData->frame->width = frameWidth;
        textureData->frame->height = frameHeight;
    }
    
    return textureData;
}

SkeletonData *XMLDataParser::parseSkeletonData(const void *rawSkeletonData, float scale) const
{
    _skeletonScale = scale;
    const XMLElement *skeletonXML = static_cast<const XMLElement *>(rawSkeletonData);
    String version = skeletonXML->Attribute(A_VERSION);
    // TODO
    /*
    switch(version)
    {
    
    }
    */
    _frameRate = skeletonXML->IntAttribute(A_FRAME_RATE);
    SkeletonData *skeletonData = new SkeletonData();
    skeletonData->name = skeletonXML->Attribute(A_NAME);
    
    for (const XMLElement *armatureXML = skeletonXML->FirstChildElement(ARMATURE); armatureXML; armatureXML = armatureXML->NextSiblingElement(ARMATURE))
    {
        ArmatureData *armatureData = parseArmatureData(armatureXML);
        skeletonData->armatureDataList.push_back(armatureData);
    }
    
    return skeletonData;
}

ArmatureData *XMLDataParser::parseArmatureData(const XMLElement *armatureXML) const
{
    ArmatureData *armatureData = new ArmatureData();
    armatureData->name = armatureXML->Attribute(A_NAME);
    
    for (const XMLElement *boneXML = armatureXML->FirstChildElement(BONE); boneXML; boneXML = boneXML->NextSiblingElement(BONE))
    {
        BoneData *boneData = parseBoneData(boneXML);
        armatureData->boneDataList.push_back(boneData);
    }
    
    for (const XMLElement *skinXML = armatureXML->FirstChildElement(SKIN); skinXML; skinXML = skinXML->NextSiblingElement(SKIN))
    {
        SkinData *skinData = parseSkinData(skinXML);
        armatureData->skinDataList.push_back(skinData);
    }
    
    transformArmatureData(armatureData);
    armatureData->sortBoneDataList();
    
    for (const XMLElement *animationXML = armatureXML->FirstChildElement(ANIMATION); animationXML; animationXML = animationXML->NextSiblingElement(ANIMATION))
    {
        AnimationData *animationData = parseAnimationData(animationXML, armatureData);
        armatureData->animationDataList.push_back(animationData);
    }
    
    for (const XMLElement *rectangleXML = armatureXML->FirstChildElement(RECTANGLE); rectangleXML; rectangleXML = rectangleXML->NextSiblingElement(RECTANGLE))
    {
        RectangleData *rectangleData = parseRectangleData(rectangleXML);
        armatureData->areaDataList.push_back(rectangleData);
    }
    
    for (const XMLElement *ellipseXML = armatureXML->FirstChildElement(ELLIPSE); ellipseXML; ellipseXML = ellipseXML->NextSiblingElement(ELLIPSE))
    {
        RectangleData *ellipseData = parseRectangleData(ellipseXML);
        armatureData->areaDataList.push_back(ellipseData);
    }
    
    return armatureData;
}

BoneData *XMLDataParser::parseBoneData(const XMLElement *boneXML) const
{
    BoneData *boneData = new BoneData();
    boneData->name = boneXML->Attribute(A_NAME);
    boneData->parent = boneXML->Attribute(A_PARENT);
    boneData->length = (float)(boneXML->DoubleAttribute(A_LENGTH));
    boneData->inheritRotation = getBoolean(boneXML, A_INHERIT_ROTATION, true);
    boneData->inheritScale = getBoolean(boneXML, A_INHERIT_SCALE, false);
    parseTransform(boneXML->FirstChildElement(TRANSFORM), &boneData->global, nullptr);
    boneData->transform = boneData->global;
    
    for (const XMLElement *rectangleXML = boneXML->FirstChildElement(RECTANGLE); rectangleXML; rectangleXML = rectangleXML->NextSiblingElement(RECTANGLE))
    {
        RectangleData *rectangleData = parseRectangleData(rectangleXML);
        boneData->areaDataList.push_back(rectangleData);
    }
    
    for (const XMLElement *ellipseXML = boneXML->FirstChildElement(ELLIPSE); ellipseXML; ellipseXML = ellipseXML->NextSiblingElement(ELLIPSE))
    {
        RectangleData *ellipseData = parseRectangleData(ellipseXML);
        boneData->areaDataList.push_back(ellipseData);
    }
    
    return boneData;
}

SkinData *XMLDataParser::parseSkinData(const XMLElement *skinXML) const
{
    SkinData *skinData = new SkinData();
    skinData->name = skinXML->Attribute(A_NAME);
    
    for (const XMLElement *slotXML = skinXML->FirstChildElement(SLOT); slotXML; slotXML = slotXML->NextSiblingElement(SLOT))
    {
        SlotData *slotData = parseSlotData(slotXML);
        skinData->slotDataList.push_back(slotData);
    }
    
    return skinData;
}

SlotData *XMLDataParser::parseSlotData(const XMLElement *slotXML) const
{
    SlotData *slotData = new SlotData();
    slotData->name = slotXML->Attribute(A_NAME);
    slotData->parent = slotXML->Attribute(A_PARENT);
    slotData->zOrder = (float)(slotXML->DoubleAttribute(A_Z_ORDER));
    slotData->blendMode = getEnumByString<BlendMode>(slotXML->Attribute(A_BLENDMODE));
    
    for (const XMLElement *displayXML = slotXML->FirstChildElement(DISPLAY); displayXML; displayXML = displayXML->NextSiblingElement(DISPLAY))
    {
        DisplayData *displayData = parseDisplayData(displayXML);
        slotData->displayDataList.push_back(displayData);
    }
    
    return slotData;
}

DisplayData *XMLDataParser::parseDisplayData(const XMLElement *displayXML) const
{
    DisplayData *displayData = new DisplayData();
    displayData->name = displayXML->Attribute(A_NAME);
    displayData->type = getEnumByString<DisplayType>(displayXML->Attribute(A_TYPE));
    parseTransform(displayXML->FirstChildElement(TRANSFORM), &displayData->transform, &displayData->pivot);
    return displayData;
}

AnimationData *XMLDataParser::parseAnimationData(const XMLElement *animationXML, ArmatureData *armatureData) const
{
    AnimationData *animationData = new AnimationData();
    animationData->name = animationXML->Attribute(A_NAME);
    animationData->frameRate = _frameRate;
    animationData->playTimes = animationXML->IntAttribute(A_LOOP);
    animationData->fadeTime = (float)(animationXML->DoubleAttribute(A_FADE_IN_TIME));
    animationData->scale = getNumber(animationXML, A_SCALE, 1, 1);
    //use frame tweenEase, NaN
    //overwrite frame tweenEase, [-1, 0):ease in, 0:line easing, (0, 1]:ease out, (1, 2]:ease in out
    animationData->tweenEasing = getNumber(animationXML, A_TWEEN_EASING, USE_FRAME_TWEEN_EASING, USE_FRAME_TWEEN_EASING);
    animationData->autoTween = getBoolean(animationXML, A_AUTO_TWEEN, true);
    
    for (const XMLElement *frameXML = animationXML->FirstChildElement(FRAME); frameXML; frameXML = frameXML->NextSiblingElement(FRAME))
    {
        Frame *frame = parseMainFrame(frameXML);
        animationData->frameList.push_back(frame);
    }
    
    parseTimeline(animationXML, animationData);
    
    for (const XMLElement *timelineXML = animationXML->FirstChildElement(TIMELINE); timelineXML; timelineXML = timelineXML->NextSiblingElement(TIMELINE))
    {
        TransformTimeline *timeline = parseTransformTimeline(timelineXML);
        animationData->timelineList.push_back(timeline);
        //
        timeline->duration = animationData->duration;
    }
    
    addHideTimeline(animationData, armatureData);
    transformAnimationData(animationData, armatureData);
    return animationData;
}


TransformTimeline *XMLDataParser::parseTransformTimeline(const XMLElement *timelineXML) const
{
    TransformTimeline *timeline = new TransformTimeline();
    timeline->scale = (float)(timelineXML->DoubleAttribute(A_SCALE));
    timeline->offset = (float)(timelineXML->DoubleAttribute(A_OFFSET));
    
    for (const XMLElement *frameXML = timelineXML->FirstChildElement(FRAME); frameXML; frameXML = frameXML->NextSiblingElement(FRAME))
    {
        TransformFrame *frame = parseTransformFrame(frameXML);
        timeline->frameList.push_back(frame);
    }
    
    parseTimeline(timelineXML, timeline);
    return timeline;
}

Frame *XMLDataParser::parseMainFrame(const XMLElement *frameXML) const
{
    Frame *frame = new Frame();
    parseFrame(frameXML, frame);
    return frame;
}

TransformFrame *XMLDataParser::parseTransformFrame(const XMLElement *frameXML) const
{
    TransformFrame *frame = new TransformFrame();
    parseFrame(frameXML, frame);
    frame->visible = !getBoolean(frameXML, A_HIDE, false);
    //NaN:no tween, 10:auto tween, [-1, 0):ease in, 0:line easing, (0, 1]:ease out, (1, 2]:ease in out
    frame->tweenEasing = getNumber(frameXML, A_TWEEN_EASING, NO_TWEEN_EASING, NO_TWEEN_EASING);
    frame->tweenRotate = frameXML->IntAttribute(A_TWEEN_ROTATE);
    frame->tweenScale = getBoolean(frameXML, A_TWEEN_SCALE, true);
    frame->displayIndex = frameXML->IntAttribute(A_DISPLAY_INDEX);
    frame->zOrder = getNumber(frameXML, A_Z_ORDER, 0.f, 0.f);
    parseTransform(frameXML->FirstChildElement(TRANSFORM), &frame->global, &frame->pivot);
    //copy
    frame->transform = frame->global;
    frame->scaleOffset.x = getNumber(frameXML, A_SCALE_X_OFFSET, 0.f, 0.f);
    frame->scaleOffset.y = getNumber(frameXML, A_SCALE_Y_OFFSET, 0.f, 0.f);
    const XMLElement *colorTransformXML = frameXML->FirstChildElement(COLOR_TRANSFORM);
    
    if (colorTransformXML)
    {
        frame->color = new ColorTransform();
        parseColorTransform(colorTransformXML, frame->color);
    }
    
    return frame;
}

RectangleData *XMLDataParser::parseRectangleData(const XMLElement *rectangleXML) const
{
    return nullptr;
}

EllipseData *XMLDataParser:: parseEllipseData(const XMLElement *ellipseXML) const
{
    return nullptr;
}

void XMLDataParser::parseTimeline(const XMLElement *timelineXML, Timeline *timeline) const
{
    timeline->duration = (int)(round((float)(timelineXML->DoubleAttribute(A_DURATION)) / _frameRate)) * 1000;
    
    if (!timeline->frameList.empty())
    {
        int position = 0;
        Frame *frame = nullptr;
        
        for (size_t i = 0, l = timeline->frameList.size(); i < l; ++i)
        {
            frame = timeline->frameList[i];
            frame->position = position;
            position += frame->duration;
        }
        
        if (frame)
        {
            frame->duration = timeline->duration - frame->position;
        }
    }
}

void XMLDataParser::parseFrame(const XMLElement *frameXML, Frame *frame) const
{
    frame->duration = (int)(round((float)(frameXML->DoubleAttribute(A_DURATION))) / _frameRate * 1000);
    frame->action = frameXML->Attribute(A_ACTION);
    frame->event = frameXML->Attribute(A_EVENT);
    frame->sound = frameXML->Attribute(A_SOUND);
}

void XMLDataParser::parseTransform(const XMLElement *transformXML, Transform *transform, Point *pivot) const
{
    if (transformXML)
    {
        if (transform)
        {
            transform->x = (float)(transformXML->DoubleAttribute(A_X));
            transform->y = (float)(transformXML->DoubleAttribute(A_Y));
            transform->skewX = (float)(transformXML->DoubleAttribute(A_SKEW_X) * ANGLE_TO_RADIAN);
            transform->skewY = (float)(transformXML->DoubleAttribute(A_SKEW_Y) * ANGLE_TO_RADIAN);
            transform->scaleX = (float)(transformXML->DoubleAttribute(A_SCALE_X));
            transform->scaleY = (float)(transformXML->DoubleAttribute(A_SCALE_Y));
        }
        
        if (pivot)
        {
            pivot->x = (float)(transformXML->DoubleAttribute(A_PIVOT_X));
            pivot->y = (float)(transformXML->DoubleAttribute(A_PIVOT_Y));
        }
    }
}

void XMLDataParser::parseColorTransform(const XMLElement *colorTransformXML, ColorTransform *colorTransform) const
{
    if (colorTransformXML)
    {
        if (colorTransform)
        {
            colorTransform->alphaOffset = colorTransformXML->IntAttribute(A_ALPHA_OFFSET);
            colorTransform->redOffset = colorTransformXML->IntAttribute(A_RED_OFFSET);
            colorTransform->greenOffset = colorTransformXML->IntAttribute(A_GREEN_OFFSET);
            colorTransform->blueOffset = colorTransformXML->IntAttribute(A_BLUE_OFFSET);
            colorTransform->alphaMultiplier = (float)(colorTransformXML->DoubleAttribute(A_ALPHA_MULTIPLIER)) * 0.01f;
            colorTransform->redMultiplier = (float)(colorTransformXML->DoubleAttribute(A_RED_MULTIPLIER)) * 0.01f;
            colorTransform->greenMultiplier = (float)(colorTransformXML->DoubleAttribute(A_GREEN_MULTIPLIER)) * 0.01f;
            colorTransform->blueMultiplier = (float)(colorTransformXML->DoubleAttribute(A_BLUE_MULTIPLIER)) * 0.01f;
        }
    }
}
NAME_SPACE_DRAGON_BONES_END
