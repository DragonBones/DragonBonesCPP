#ifndef __DRAGON_BONES_H__
#define __DRAGON_BONES_H__

#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <cmath>
//#include <cfloat>
//#include <list>
//#include <cstdlib>
//#include <stdexcept>

#include "cocos2d.h"

// namespace dragonBones {}
#define NAME_SPACE_DRAGON_BONES_BEGIN namespace dragonBones {
#define NAME_SPACE_DRAGON_BONES_END }

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&); \
    void operator=(const TypeName&)

#define DRAGON_BONES_VERSION "3.0.0"
#define DRAGON_BONES_DATA_VERSION "3.0"

#define DRAGON_BONES "dragonBones"
#define ARMATURE "armature"
#define SKIN "skin"
#define BONE "bone"
#define SLOT "slot"
#define DISPLAY "display"
#define ANIMATION "animation"
#define TIMELINE "timeline"
#define FRAME "frame"
#define TRANSFORM "transform"
#define COLOR_TRANSFORM "colorTransform"
#define RECTANGLE "rectangle"
#define ELLIPSE "ellipse"

#define TEXTURE_ATLAS "TextureAtlas"
#define SUB_TEXTURE "SubTexture"

#define A_ROTATED "rotated"
#define A_FRAME_X "frameX"
#define A_FRAME_Y "frameY"
#define A_FRAME_WIDTH "frameWidth"
#define A_FRAME_HEIGHT "frameHeight"

#define A_VERSION "version"
#define A_IMAGE_PATH "imagePath"
#define A_FRAME_RATE "frameRate"
#define A_NAME "name"
#define A_PARENT "parent"
#define A_LENGTH "length"
#define A_TYPE "type"
#define A_FADE_IN_TIME "fadeInTime"
#define A_DURATION "duration"
#define A_SCALE "scale"
#define A_OFFSET "offset"
#define A_LOOP "loop"
#define A_EVENT "event"
#define A_SOUND "sound"
#define A_ACTION "action"
#define A_HIDE "hide"
#define A_AUTO_TWEEN "autoTween"
#define A_TWEEN_EASING "tweenEasing"
#define A_TWEEN_ROTATE "tweenRotate"
#define A_TWEEN_SCALE "tweenScale"
#define A_DISPLAY_INDEX "displayIndex"
#define A_Z_ORDER "z"
#define A_BLENDMODE "blendMode"
#define A_WIDTH "width"
#define A_HEIGHT "height"
#define A_INHERIT_ROTATION "inheritRotation"
#define A_INHERIT_SCALE "inheritScale"
#define A_X "x"
#define A_Y "y"
#define A_SKEW_X "skX"
#define A_SKEW_Y "skY"
#define A_SCALE_X "scX"
#define A_SCALE_Y "scY"
#define A_PIVOT_X "pX"
#define A_PIVOT_Y "pY"
#define A_ALPHA_OFFSET "aO"
#define A_RED_OFFSET "rO"
#define A_GREEN_OFFSET "gO"
#define A_BLUE_OFFSET "bO"
#define A_ALPHA_MULTIPLIER "aM"
#define A_RED_MULTIPLIER "rM"
#define A_GREEN_MULTIPLIER "gM"
#define A_BLUE_MULTIPLIER "bM"

// add scale offset to frame
#define A_SCALE_X_OFFSET "scXOffset"
#define A_SCALE_Y_OFFSET "scYOffset"


NAME_SPACE_DRAGON_BONES_BEGIN

typedef std::string String;

const float PI = 3.14159265358979323846f;
const float ANGLE_TO_RADIAN = PI / 180.f;
const float RADIAN_TO_ANGLE = 180.f / PI;

const float AUTO_TWEEN_EASING = 10.f;
const float NO_TWEEN_EASING = 20.f;
const float USE_FRAME_TWEEN_EASING = 30.f;


template <typename EnumType>
struct EnumName
{
    static const char *list[];
};


inline float round(float value)
{
    return (value > 0.0f) ? floor(value + 0.5f) : ceil(value - 0.5f);
}

inline float formatRadian(float radian)
{
    radian = fmod(radian, PI * 2.f);
    if (radian > PI)
    {
        radian -= PI * 2.f;
    }
    if (radian < -PI)
    {
        radian += PI * 2.f;
    }
    return radian;
}

inline float getEaseValue(float value, float easing)
{
    float valueEase = 1.f;
    if (easing > 1)    // ease in out
    {
        valueEase = 0.5f * (1.f - cos(value * PI));
        easing -= 1.f;
    }
    else if (easing > 0)    // ease out
    {
        valueEase = 1.f - pow(1.f - value, 2);
    }
    else if (easing < 0)    // ease in
    {
        easing *= -1.f;
        valueEase =  pow(value, 2);
    }
    return (valueEase - value) * easing + value;
}

template <typename T>
inline T getEnumByString(const char *string)
{
    T enumValue = static_cast<T>(-1);
    int count = sizeof(EnumName<T>::list) / sizeof(EnumName<T>::list[0]);
    for (int i = 0; i < count; ++i)
    {
        if (strcmp(string, EnumName<T>::list[i]) == 0)
        {
            enumValue = static_cast<T>(i);
            break;
        }
    }
    return enumValue;
}

template<typename T>
inline int indexOf(std::vector<T> &vector, const T &value)
{
    for (size_t i = 0, l = vector.size(); i < l; ++i)
    {
        if (vector[i] == value)
        {
            return i;
        }
    }
    return -1;
}


enum class DisplayType {DT_IMAGE, DT_ARMATURE, DT_1, DT_2, DT_3, DT_4, DT_5, DT_6};
const char *EnumName<DisplayType>::list[] = {"image", "armature"};

enum class BlendMode {BM_AUTO, BM_ADD, BM_ALPHA, BM_DARKEN, BM_DIFFERENCE, BM_ERASE, BM_HARDLIGHT, BM_INVERT, BM_LAYER, BM_LIGHTEN, BM_MULTIPLY, BM_NORMAL, BM_OVERLAY, BM_SCREEN, BM_SHADER, BM_SUBTRACT};
const char *EnumName<BlendMode>::list[] = {"auto", "add", "alpha", "darken", "difference", "erase", "hardlight", "invert", "layer", "lighten", "multiply", "normal", "overlay", "screen", "shader", "subtract"};

NAME_SPACE_DRAGON_BONES_END

#endif  // __DRAGON_BONES_H__
