#ifndef PARSERS_BASE_DATA_PARSER_H
#define PARSERS_BASE_DATA_PARSER_H

#include "../DragonBones.h"

// geoms
#include "../geoms/ColorTransform.h"
#include "../geoms/Matrix.h"
#include "../geoms/Point.h"
#include "../geoms/Rectangle.h"
#include "../geoms/Transform.h"

// objects
#include "../objects/IAreaData.h"
#include "../objects/RectangleData.h"
#include "../objects/EllipseData.h"
#include "../objects/Frame.h"
#include "../objects/TransformFrame.h"
#include "../objects/Timeline.h"
#include "../objects/TransformTimeline.h"
#include "../objects/AnimationData.h"
#include "../objects/TextData.h"
#include "../objects/DisplayData.h"
#include "../objects/SlotData.h"
#include "../objects/SkinData.h"
#include "../objects/BoneData.h"
#include "../objects/ArmatureData.h"
#include "../objects/DragonBonesData.h"

// textures
#include "../textures/TextureData.h"
#include "../textures/TextureAtlasData.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class BaseDataParser
{
protected:
    static TextData::AlignHType getAlignHType(const char *alignHString);
    static TextData::AlignVType getAlignVType(const char *alignVString);
    static TextData::TextType getTextType(const char *textTypeString);
    static TextData::LineType getLineType(const char *lineTypeString);
    static void transformArmatureData(ArmatureData *armatureData);
    static void transformArmatureDataAnimations(ArmatureData *armatureData);
    static void transformAnimationData(AnimationData *animationData, const ArmatureData *armatureData);
    static void addHideTimeline(AnimationData *animationData, const ArmatureData *armatureData);
    
private:
    static void setFrameTransform(AnimationData *animationData, const ArmatureData *armatureData, const BoneData *boneData, TransformFrame *frame);
    static void getTimelineTransform(const TransformTimeline *timeline, int position, Transform *retult, bool isGlobal);
    
public:
    BaseDataParser();
    virtual ~BaseDataParser();
    
    virtual TextureAtlasData *parseTextureAtlasData(const void *rawTextureAtlasData, float scale = 1.f) const = 0;
    
    virtual DragonBonesData *parseDragonBonesData(const void *rawDragonBonesData, float scale = 1.f) const = 0;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(BaseDataParser);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // PARSERS_BASE_DATA_PARSER_H
