#ifndef DRAGONBONES_DRAGON_BONES_H
#define DRAGONBONES_DRAGON_BONES_H

/*
#ifndef X_Y_Z_H
#define X_Y_Z_H
#include "../core/DragonBones.h"
NAMESPACE_DRAGONBONES_BEGIN
class xyz
{
BIND_CLASS_TYPE(xyz);
    xyz() {}
    ~xyz() {}
private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(xyz);
};

NAMESPACE_DRAGONBONES_END
#endif // X_Y_Z_H
*/

#include <algorithm>
#include <cmath>
#include <locale>
#include <vector>
#include <map>
#include <functional>
//#include <cstdlib>
#include <assert.h>

#ifndef DRAGONBONES_ASSERT
#define DRAGONBONES_ASSERT(cond, msg) do { \
        assert(cond); \
    } while (0)
#endif // DRAGONBONES_ASSERT

// namespace dragonBones {}
#define NAMESPACE_DRAGONBONES_BEGIN namespace dragonBones {
#define NAMESPACE_DRAGONBONES_END }
#define USING_NAME_SPACE_DRAGON_BONES using namespace dragonBones

/**
 * A macro to disallow the copy constructor and operator= functions
 * This should be used in the private: declarations for a class
 */
#define DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(CLASS)\
    CLASS(const CLASS&);\
    void operator=(const CLASS&)\

NAMESPACE_DRAGONBONES_BEGIN

enum class DisplayType 
{
    Image = 0, 
    Armature = 1, 
    Mesh = 2
};

enum class ArmatureType 
{
    Armature = 0,
    MovieClip = 1,
    Stage = 2
};

enum class ExtensionType {
    FFD = 0,
    AdjustColor = 10,
    BevelFilter = 11,
    BlurFilter = 12,
    DropShadowFilter = 13,
    GlowFilter = 14,
    GradientBevelFilter = 15,
    GradientGlowFilter = 16
};

enum class EventType {
    Frame = 0,
    Sound = 1
};

enum class ActionType {
    Play = 0,
    Stop = 1,
    GotoAndPlay = 2,
    GotoAndStop = 3,
    FadeIn = 4,
    FadeOut = 5
};

enum class BlendMode 
{
    Normal = 0,
    Add = 1,
    Alpha = 2,
    Darken = 3,
    Difference = 4,
    Erase = 5,
    HardLight = 6,
    Invert = 7,
    Layer = 8,
    Lighten = 9,
    Multiply = 10,
    Overlay = 11,
    Screen = 12,
    Subtract = 13
};

const float PI = 3.14159265358979323846f;
const float PI_D = PI * 2.0f;
const float PI_H = PI * 0.5f;
const float PI_Q = PI * 0.25f;
const float ANGLE_TO_RADIAN = PI / 180.f;
const float RADIAN_TO_ANGLE = 180.f / PI;
const float SECOND_TO_MILLISECOND = 1000.f;
const float SECOND_TO_MICROSECOND = 1000000.f;

template<class T>
inline void clearVector(std::vector<T>& value)
{
    std::vector<T>().swap(value);
}

template<class T>
inline T* mapFind(const std::map<std::string, T*>& map, const std::string& key)
{
    const auto iterator = map.find(key);
    return (iterator != map.end()) ? iterator->second : nullptr;
}

template<class T>
inline int indexOf(const std::vector<T>& vector, const T& value)
{
    for (std::size_t i = 0, l = vector.size(); i < l; ++i)
    {
        if (vector[i] == value)
        {
            return i;
        }
    }
    
    return -1;
}
/*
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
*/
NAMESPACE_DRAGONBONES_END

#endif // DRAGONBONES_DRAGON_BONES_H