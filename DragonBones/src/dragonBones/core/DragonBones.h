#ifndef DRAGONBONES_H
#define DRAGONBONES_H

#include <cmath>
#include <cstdint>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <tuple>
#include <functional>
#include <sstream>
#include <assert.h>

// dragonBones assert
#define DRAGONBONES_ASSERT(cond, msg) \
do { \
    assert(cond); \
} while (0)

// namespace dragonBones {}
#define DRAGONBONES_NAMESPACE_BEGIN namespace dragonBones {
#define DRAGONBONES_NAMESPACE_END }

// using dragonBones namespace
#define DRAGONBONES_USING_NAME_SPACE using namespace dragonBones

#define ABSTRACT_CLASS(CLASS) \
public:\
    CLASS(){}\
    virtual ~CLASS(){};

#define BIND_CLASS_TYPE(CLASS) \
public:\
    static std::size_t getTypeIndex()\
    {\
        static const auto typeIndex = typeid(CLASS).hash_code();\
        return typeIndex;\
    }\
    virtual std::size_t getClassTypeIndex() const override\
    {\
        return CLASS::getTypeIndex();\
    }\

#define BIND_CLASS_TYPE_A(CLASS) \
public:\
    static std::size_t getTypeIndex()\
    {\
        static const auto typeIndex = typeid(CLASS).hash_code();\
        return typeIndex;\
    }\
    virtual std::size_t getClassTypeIndex() const override\
    {\
        return CLASS::getTypeIndex();\
    }\
public:\
    CLASS(){_onClear();}\
    ~CLASS(){_onClear();}\
private:\
    CLASS(const CLASS&);\
    void operator=(const CLASS&)

#define BIND_CLASS_TYPE_B(CLASS) \
public:\
    static std::size_t getTypeIndex()\
    {\
        static const auto typeIndex = typeid(CLASS).hash_code();\
        return typeIndex;\
    }\
    virtual std::size_t getClassTypeIndex() const override\
    {\
        return CLASS::getTypeIndex();\
    }\
private:\
    CLASS(const CLASS&);\
    void operator=(const CLASS&)

#define DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(CLASS) \
private:\
    CLASS(const CLASS&);\
    void operator=(const CLASS&);

DRAGONBONES_NAMESPACE_BEGIN

enum class BinaryOffset
{
    WeigthBoneCount = 0,
    WeigthFloatOffset = 1,
    WeigthBoneIndices = 2,

    MeshVertexCount = 0,
    MeshTriangleCount = 1,
    MeshFloatOffset = 2,
    MeshWeightOffset = 3,
    MeshVertexIndices = 4,

    TimelineScale = 0,
    TimelineOffset = 1,
    TimelineKeyFrameCount = 2,
    TimelineFrameValueCount = 3,
    TimelineFrameValueOffset = 4,
    TimelineFrameOffset = 5,

    FramePosition = 0,
    FrameTweenType = 1,
    FrameTweenEasingOrCurveSampleCount = 2,
    FrameCurveSamples = 3,

    FFDTimelineMeshOffset = 0,
    FFDTimelineFFDCount = 1,
    FFDTimelineValueCount = 2,
    FFDTimelineValueOffset = 3,
    FFDTimelineFloatOffset = 4
};

enum class ArmatureType
{
    Armature = 0,
    MovieClip = 1,
    Stage = 2
};

enum class OffsetMode {
    None,
    Additive,
    Override
};

enum class DisplayType 
{
    Image = 0, 
    Armature = 1, 
    Mesh = 2,
    BoundingBox = 3
};

enum class BoundingBoxType
{
    Rectangle = 0,
    Ellipse = 1,
    Polygon = 2
};

enum class ActionType {
    Play = 0,
    Frame = 10,
    Sound = 11
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

enum class TweenType {
    None = 0,
    Line = 1,
    Curve = 2,
    QuadIn = 3,
    QuadOut = 4,
    QuadInOut = 5
};

enum class TimelineType {
    Action = 0,
    ZOrder = 1,

    BoneAll = 10,

    BoneTranslate = 11,
    BoneRotate = 12,
    BoneScale = 13,

    SlotDisplayIndex = 20,
    SlotColor = 21,
    SlotFFD = 22,

    IKConstraint = 30,

    AnimationTime = 40,
    AnimationWeight = 41
};

enum class AnimationFadeOutMode {
    None,
    SameLayer,
    SameGroup,
    SameLayerAndGroup,
    All,
    Single
};

enum class TextureFormat
{
    DEFAULT,
    RGBA8888,
    BGRA8888,
    RGBA4444,
    RGB888,
    RGB565,
    RGBA5551
};

template <class T>
std::string to_string(const T& value)
{
    std::ostringstream stream;
    stream << value;
    return stream.str();
}

template<class T>
inline int indexOf(const std::vector<T>& vector, const T& value)
{
    for (std::size_t i = 0, l = vector.size(); i < l; ++i)
    {
        if (vector[i] == value)
        {
            return (int) i;
        }
    }
    
    return -1;
}

template<class T>
inline T* mapFind(const std::map<std::string, T*>& map, const std::string& key)
{
    auto iterator = map.find(key);
    return (iterator != map.end()) ? iterator->second : nullptr;
}

template<class T>
inline T* mapFindB(std::map<std::string, T>& map, const std::string& key)
{
    auto iterator = map.find(key);
    return (iterator != map.end()) ? &iterator->second : nullptr;
}

class Matrix;
class Transform;
class ColorTransform;
class Point;
class Rectangle;

class BaseObject;

class UserData;
class ActionData;
class DragonBonesData;
class ArmatureData;
class CanvasData;
class BoneData;
class SlotData;
class SkinData;
class ConstraintData;
class IKConstraintData;
class DisplayData;
class ImageDisplayData;
class ArmatureDisplayData;
class MeshDisplayData;
class BoundingBoxDisplayData;
class BoundingBoxData;
class RectangleBoundingBoxData;
class EllipseBoundingBoxData;
class PolygonBoundingBoxData;
class WeightData;
class AnimationData;
class TimelineData;
class AnimationConfig;
class TextureAtlasData;
class TextureData;

class IArmatureProxy;
class Armature;
class TransformObject;
class Bone;
class Slot;
class Constraint;
class IKConstraint;

class IAnimatable;
class WorldClock;
class Animation;
class BonePose;
class AnimationState;
class TimelineState;
class TweenTimelineState;
class BoneTimelineState;
class SlotTimelineState;
class ConstraintTimelineState;
class ActionTimelineState;
class ZOrderTimelineState;
class BoneAllTimelineState;
class SlotDislayTimelineState;
class SlotColorTimelineState;
class SlotFFDTimelineState;
class IKConstraintTimelineState;

class IEventDispatcher;
class EventObject;

class DataParser;
class JSONDataParser;
class BinaryDataParser;

class BaseFactory;
class BuildArmaturePackage;

/**
* @private
*/
class DragonBones 
{
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(DragonBones)

public:
    static bool yDown;
    static bool debug;
    static bool debugDraw;
    static bool webAssembly;
    static const std::string version;

private:
    std::vector<BaseObject*> _objects;
    std::vector<EventObject*> _events;
    WorldClock* _clock;
    IEventDispatcher* _eventManager;

public:
    DragonBones(IEventDispatcher* eventManager);
    virtual ~DragonBones();

    void advanceTime(float passedTime);
    void bufferEvent(EventObject* value);
    void bufferObject(BaseObject* object);
    WorldClock* getClock();
    IEventDispatcher* getEventManager() const
    {
        return _eventManager;
    }
};

DRAGONBONES_NAMESPACE_END

#endif // DRAGONBONES_H
