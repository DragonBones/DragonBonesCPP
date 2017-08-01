#ifndef DRAGONBONES_H
#define DRAGONBONES_H

#include <cmath>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <tuple>
#include <functional>
#include <sstream>
#include <assert.h>

// dragonBones assert
#ifndef DRAGONBONES_ASSERT
#define DRAGONBONES_ASSERT(cond, msg) do { \
        assert(cond); \
    } while (0)
#endif

// namespace dragonBones {}
#define DRAGONBONES_NAMESPACE_BEGIN namespace dragonBones {
#define DRAGONBONES_NAMESPACE_END }

// using dragonBones namespace
#define DRAGONBONES_USING_NAME_SPACE using namespace dragonBones

#ifndef ABSTRACT_CLASS
#define ABSTRACT_CLASS(CLASS) \
public:\
    CLASS(){}\
    virtual ~CLASS(){};
#endif

#ifndef BIND_CLASS_TYPE_A
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
#endif

#ifndef BIND_CLASS_TYPE_B
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
#endif

#ifndef DRAGONBONES_DISALLOW_COPY_AND_ASSIGN
#define DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(CLASS) \
private:\
    CLASS(const CLASS&);\
    void operator=(const CLASS&);
#endif

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

    BoneT = 11,
    BoneR = 12,
    BoneS = 13,

    BoneX = 14,
    BoneY = 15,
    BoneRotation = 16,
    BoneSkew = 17,
    BoneScaleX = 18,
    BoneScaleY = 19,

    SlotDisplayIndex = 20,
    SlotColor = 21,
    SlotFFD = 22,

    AnimationTime = 40,
    AnimationWeight = 41
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

enum class OffsetMode {
    None,
    Additive,
    Override
};

enum class AnimationFadeOutMode {
    None,
    SameLayer,
    SameGroup,
    SameLayerAndGroup,
    All,
    Single
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
class WeightData;
class BoundingBoxData;
class RectangleBoundingBoxData;
class EllipseBoundingBoxData;
class PolygonBoundingBoxData;
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
class ActionTimelineState;
class ZOrderTimelineState;
class BoneAllTimelineState;
class SlotDislayIndexTimelineState;
class SlotColorTimelineState;
class SlotFFDTimelineState;

class IEventDispatcher;
class EventObject;

class BaseFactory;
class BuildArmaturePackage;
/**
* VERSION 5.1.0
*/
class DragonBones 
{
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(DragonBones)

public:
    static bool yDown;
    static bool debug;
    static bool debugDraw;

private:
    std::vector<BaseObject*> _objects;
    std::vector<EventObject*> _events;
    WorldClock* _clock;
    IEventDispatcher* _eventManager;

public:

    DragonBones() :
        _events(),
        _objects(),
        _clock(nullptr),
        _eventManager(nullptr)
    {}
    DragonBones(IEventDispatcher* value) :
            _events(),
            _objects(),
            _clock(nullptr),
            _eventManager(value)
    {}

    virtual ~DragonBones() {}

    void advanceTime(float passedTime);
    void bufferEvent(EventObject* value);
    void bufferObject(BaseObject* object);
    WorldClock* getClock();
    IEventDispatcher* getEventManager() const
    {
        return _eventManager;
    }
    void setEventManager(IEventDispatcher* value )
    {
        _eventManager = value;
    }


public: // For WebAssembly.
    std::vector<BaseObject*>* getObjects() { return &_objects; }
    std::vector<EventObject*>* getEvents() { return &_events; }
};

DRAGONBONES_NAMESPACE_END

#endif // DRAGONBONES_H
