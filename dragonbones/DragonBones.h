#ifndef DRAGON_BONES_H
#define DRAGON_BONES_H

#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include <assert.h>
//#include <cfloat>
//#include <stdexcept>

#ifndef DBASSERT
#define DBASSERT(cond, msg) do { \
        assert(cond); \
    } while (0)
#endif

// namespace dragonBones {}
#define NAME_SPACE_DRAGON_BONES_BEGIN namespace dragonBones {
#define NAME_SPACE_DRAGON_BONES_END }
#define USING_NS_DB                   using namespace dragonBones

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&); \
    void operator=(const TypeName&)

NAME_SPACE_DRAGON_BONES_BEGIN

const float PI = 3.14159265358979323846f;
const float ANGLE_TO_RADIAN = PI / 180.f;
const float RADIAN_TO_ANGLE = 180.f / PI;

const float AUTO_TWEEN_EASING = 10.f;
const float NO_TWEEN_EASING = 20.f;
const float USE_FRAME_TWEEN_EASING = 30.f;


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

enum class DisplayType {DT_IMAGE, DT_ARMATURE, DT_FRAME, DT_TEXT, DT_1, DT_2, DT_3, DT_4, DT_5};
inline DisplayType getDisplayTypeByString(std::string displayType)
{
    if (displayType == "image")
    {
        return DisplayType::DT_IMAGE;
    }
    else if (displayType == "armature")
    {
        return DisplayType::DT_ARMATURE;
    }
    else if (displayType == "frame")
    {
        return DisplayType::DT_FRAME;
    }
    else if (displayType == "text")
    {
        return DisplayType::DT_TEXT;
    }

    return DisplayType::DT_IMAGE;
}

enum class BlendMode {BM_ADD, BM_ALPHA, BM_DARKEN, BM_DIFFERENCE, BM_ERASE, BM_HARDLIGHT, BM_INVERT, BM_LAYER, BM_LIGHTEN, BM_MULTIPLY, BM_NORMAL, BM_OVERLAY, BM_SCREEN, BM_SHADER, BM_SUBTRACT};
inline BlendMode getBlendModeByString(const std::string &blendMode)
{
    if (blendMode == "normal")
    {
        return BlendMode::BM_NORMAL;
    }
    else if (blendMode == "add")
    {
        return BlendMode::BM_ADD;
    }
    else if (blendMode == "alpha")
    {
        return BlendMode::BM_ALPHA;
    }
    else if (blendMode == "darken")
    {
        return BlendMode::BM_DARKEN;
    }
    else if (blendMode == "difference")
    {
        return BlendMode::BM_DIFFERENCE;
    }
    else if (blendMode == "erase")
    {
        return BlendMode::BM_ERASE;
    }
    else if (blendMode == "hardLight")
    {
        return BlendMode::BM_HARDLIGHT;
    }
    else if (blendMode == "invert")
    {
        return BlendMode::BM_INVERT;
    }
    else if (blendMode == "layer")
    {
        return BlendMode::BM_LAYER;
    }
    else if (blendMode == "lighten")
    {
        return BlendMode::BM_LIGHTEN;
    }
    else if (blendMode == "multiply")
    {
        return BlendMode::BM_MULTIPLY;
    }
    else if (blendMode == "overlay")
    {
        return BlendMode::BM_OVERLAY;
    }
    else if (blendMode == "screen")
    {
        return BlendMode::BM_SCREEN;
    }
    else if (blendMode == "shader")
    {
        return BlendMode::BM_SHADER;
    }
    else if (blendMode == "subtract")
    {
        return BlendMode::BM_SUBTRACT;
    }

    return BlendMode::BM_NORMAL;
}

enum class PixelFormat
{
    //! auto detect the type
    AUTO, RGBA8888, BGRA8888, RGBA4444, RGB888, RGB565, RGBA5551
};

static const char* formatNames[] = {"", "RGBA8888", "BGRA8888", "RGBA4444", "RGB888", "RGB565", "RGBA5551"};

inline PixelFormat getPixelFormatByString(const std::string &format)
{
    size_t l = sizeof(formatNames)/sizeof(formatNames[0]);

    for (size_t i = 0; i < l; ++i)
    {
        if (format == formatNames[i])
        {
            return (PixelFormat) i;
        }
    }
    return PixelFormat::AUTO;
}

NAME_SPACE_DRAGON_BONES_END

#endif  // DRAGON_BONES_H
