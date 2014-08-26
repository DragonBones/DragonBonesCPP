#ifndef __DRAGON_BONES_H__
#define __DRAGON_BONES_H__

#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <cmath>
//#include <cfloat>
//#include <cstdlib>
//#include <stdexcept>

// namespace dragonBones {}
#define NAME_SPACE_DRAGON_BONES_BEGIN namespace dragonBones {
#define NAME_SPACE_DRAGON_BONES_END }

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&); \
    void operator=(const TypeName&)

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

enum class DisplayType {DT_IMAGE, DT_ARMATURE, DT_FRAME, DT_1, DT_2, DT_3, DT_4, DT_5};
enum class BlendMode {BM_ADD, BM_ALPHA, BM_DARKEN, BM_DIFFERENCE, BM_ERASE, BM_HARDLIGHT, BM_INVERT, BM_LAYER, BM_LIGHTEN, BM_MULTIPLY, BM_NORMAL, BM_OVERLAY, BM_SCREEN, BM_SHADER, BM_SUBTRACT};

inline DisplayType getDisplayTypeByString(String displayType)
{
    if (displayType == "image")
    {
        return DisplayType::DT_IMAGE;
    }

    if (displayType == "armature")
    {
        return DisplayType::DT_ARMATURE;
    }

    if (displayType == "frame")
    {
        return DisplayType::DT_FRAME;
    }

    return DisplayType::DT_IMAGE;
}

inline BlendMode getBlendModeByString(String blendMode)
{
    if (blendMode == "add")
    {
        return BlendMode::BM_ADD;
    }

    if (blendMode == "alpha")
    {
        return BlendMode::BM_ALPHA;
    }

    if (blendMode == "darken")
    {
        return BlendMode::BM_DARKEN;
    }

    if (blendMode == "difference")
    {
        return BlendMode::BM_DIFFERENCE;
    }

    if (blendMode == "erase")
    {
        return BlendMode::BM_ERASE;
    }

    if (blendMode == "hardlight")
    {
        return BlendMode::BM_HARDLIGHT;
    }

    if (blendMode == "invert")
    {
        return BlendMode::BM_INVERT;
    }

    if (blendMode == "layer")
    {
        return BlendMode::BM_LAYER;
    }

    if (blendMode == "lighten")
    {
        return BlendMode::BM_LIGHTEN;
    }

    if (blendMode == "multiply")
    {
        return BlendMode::BM_MULTIPLY;
    }

    if (blendMode == "normal")
    {
        return BlendMode::BM_NORMAL;
    }

    if (blendMode == "overlay")
    {
        return BlendMode::BM_OVERLAY;
    }

    if (blendMode == "screen")
    {
        return BlendMode::BM_SCREEN;
    }

    if (blendMode == "shader")
    {
        return BlendMode::BM_SHADER;
    }

    if (blendMode == "subtract")
    {
        return BlendMode::BM_SUBTRACT;
    }

    return BlendMode::BM_NORMAL;
}

NAME_SPACE_DRAGON_BONES_END

#endif  // __DRAGON_BONES_H__
