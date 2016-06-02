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

FrameData::FrameData() {}
FrameData::~FrameData() {}

void FrameData::_onClear()
{
    position = 0;
    duration = 0;
    prev = nullptr;
    next = nullptr;
}

const float TweenFrameData::NO_TWEEN = 100.f;

void TweenFrameData::samplingCurve(std::vector<float>& curve, unsigned frameCount, std::vector<float>& sampling)
{
    if (curve.empty() || frameCount == 0)
    {
        return;
    }

    const auto samplingTimes = frameCount + 2;
    const auto samplingStep = 1.f / samplingTimes;
    sampling.reserve((samplingTimes - 1) * 2);

    curve.insert(curve.begin(), 0.f);
    curve.insert(curve.begin(), 0.f);
    curve.push_back(1.f);
    curve.push_back(1.f);

    std::size_t stepIndex = 0;
    for (std::size_t i = 0; i < samplingTimes - 1; ++i)
    {
        const auto step = samplingStep * (i + 1);
        while (curve[stepIndex + 6] < step) // stepIndex + 3 * 2
        {
            stepIndex += 6; // stepIndex += 3 * 2
        }

        const auto x1 = curve[stepIndex];
        const auto x4 = curve[stepIndex + 6];

        const auto t = (step - x1) / (x4 - x1);
        const auto l_t = 1.f - t;

        const auto powA = l_t * l_t;
        const auto powB = t * t;

        const auto kA = l_t * powA;
        const auto kB = 3.f * t * powA;
        const auto kC = 3.f * l_t * powB;
        const auto kD = t * powB;

        sampling[i * 2] = kA * x1 + kB * curve[stepIndex + 2] + kC * curve[stepIndex + 4] + kD * x4;
        sampling[i * 2 + 1] = kA * curve[stepIndex + 1] + kB * curve[stepIndex + 3] + kC * curve[stepIndex + 5] + kD * curve[stepIndex + 7];
    }
}

TweenFrameData::TweenFrameData() {}
TweenFrameData::~TweenFrameData() {}

void TweenFrameData::_onClear()
{
    FrameData::_onClear();

    tweenEasing = 0.f;
    clearVector(curve);
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
