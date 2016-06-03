#include "FrameData.h"

NAMESPACE_DRAGONBONES_BEGIN

EventData::EventData()
{
    _onClear();
}
EventData::~EventData()
{
    _onClear();
}

void EventData::_onClear()
{
    type = EventType::Frame;
    name.clear();
    //data = nullptr;
    bone = nullptr;
    slot = nullptr;
}

ActionData::ActionData()
{
    _onClear();
}
ActionData::~ActionData()
{
    _onClear();
}

void ActionData::_onClear()
{
    type = ActionType::Play;
    //params;
    bone = nullptr;
    slot = nullptr;
}

AnimationFrameData::AnimationFrameData()
{
    _onClear();
}
AnimationFrameData::~AnimationFrameData()
{
    _onClear();
}

void AnimationFrameData::_onClear()
{
    FrameData::_onClear();

    for (const auto action : actions)
    {
        action->returnToPool();
    }

    for (const auto event : events)
    {
        event->returnToPool();
    }

    actions.clear();
    events.clear();
}

BoneFrameData::BoneFrameData()
{
    _onClear();
}
BoneFrameData::~BoneFrameData()
{
    _onClear();
}

void BoneFrameData::_onClear()
{
    TweenFrameData::_onClear();

    tweenScale = false;
    tweenRotate = 0;
    parent = nullptr;
    transform.identity();
}

ColorTransform SlotFrameData::DEFAULT_COLOR;
ColorTransform * SlotFrameData::generateColor()
{
    return new ColorTransform();
}

SlotFrameData::SlotFrameData() :
    color(nullptr)
{
    _onClear();
}
SlotFrameData::~SlotFrameData()
{
    _onClear();
}

void SlotFrameData::_onClear()
{
    TweenFrameData::_onClear();

    displayIndex = 0;
    zOrder = 0;

    if (color)
    {
        if (color != &DEFAULT_COLOR)
        {
            delete color;
        }

        color = nullptr;
    }
}

ExtensionFrameData::ExtensionFrameData()
{
    _onClear();
}
ExtensionFrameData::~ExtensionFrameData()
{
    _onClear();
}

void ExtensionFrameData::_onClear()
{
    TweenFrameData::_onClear();

    type = ExtensionType::FFD;
    clearVector(tweens);
    clearVector(keys);
}

NAMESPACE_DRAGONBONES_END
