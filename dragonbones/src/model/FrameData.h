#ifndef DRAGONBONES_FRAME_DATA_H
#define DRAGONBONES_FRAME_DATA_H

#include "../core/BaseObject.h"
#include "../geom/Transform.h"
#include "../geom/ColorTransform.h"

NAMESPACE_DRAGONBONES_BEGIN

class BoneData;
class SlotData;

class EventData final : public BaseObject
{
    BIND_CLASS_TYPE(EventData);

public:
    EventType type;
    std::string name;
    //void* data;
    BoneData* bone;
    SlotData* slot;

    EventData();
    ~EventData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(EventData);

protected:
    void _onClear() override;
};

class ActionData final : public BaseObject
{
    BIND_CLASS_TYPE(ActionData);

public:
    ActionType type;
    //params;
    BoneData* bone;
    SlotData* slot;

    ActionData();
    ~ActionData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(ActionData);

protected:
    void _onClear() override;
};

class FrameData : public BaseObject
{
public:
    unsigned position;
    unsigned duration;
    FrameData* prev;
    FrameData* next;

    FrameData();
    virtual ~FrameData() = 0;

protected:
    virtual void _onClear() override;
};

class TweenFrameData : public FrameData
{
public:
    static const float NO_TWEEN;
    static void samplingCurve(std::vector<float>& curve, unsigned frameCount, std::vector<float>& sampling);

public:
    float tweenEasing;
    std::vector<float> curve;

    TweenFrameData();
    virtual ~TweenFrameData() = 0;

protected:
    virtual void _onClear() override;
};

class AnimationFrameData final : public FrameData
{
    BIND_CLASS_TYPE(AnimationFrameData);

public:
    std::vector<ActionData*> actions;
    std::vector<EventData*> events;

    AnimationFrameData();
    ~AnimationFrameData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(AnimationFrameData);

protected:
    void _onClear() override;
};

class BoneFrameData final : public TweenFrameData
{
    BIND_CLASS_TYPE(BoneFrameData);

public:
    bool tweenScale;
    int tweenRotate;
    BoneData* parent;
    Transform transform;

    BoneFrameData();
    ~BoneFrameData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(BoneFrameData);

protected:
    void _onClear() override;
};

class SlotFrameData final : public TweenFrameData
{
    BIND_CLASS_TYPE(SlotFrameData);

public:
    static ColorTransform DEFAULT_COLOR;
    static ColorTransform* generateColor();

public:
    int displayIndex;
    int zOrder;
    ColorTransform* color;

    SlotFrameData();
    ~SlotFrameData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(SlotFrameData);

protected:
    void _onClear() override;
};

class ExtensionFrameData final : public TweenFrameData
{
    BIND_CLASS_TYPE(ExtensionFrameData);

public:
    ExtensionType type;
    std::vector<float> tweens;
    std::vector<float> keys;

    ExtensionFrameData();
    ~ExtensionFrameData();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(ExtensionFrameData);

protected:
    void _onClear() override;
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_FRAME_DATA_H