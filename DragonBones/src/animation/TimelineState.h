#ifndef DRAGONBONES_TIMELINE_STATE_H
#define DRAGONBONES_TIMELINE_STATE_H

#include "BaseTimelineState.h"

NAMESPACE_DRAGONBONES_BEGIN

class Bone;
class Slot;

class AnimationTimelineState final : public TimelineState<AnimationFrameData, AnimationData>
{
    BIND_CLASS_TYPE(AnimationTimelineState);

private:
    bool _isStarted;

public:
    AnimationTimelineState();
    ~AnimationTimelineState();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(AnimationTimelineState);

protected:
    void _onClear() override;
    void _onCrossFrame(AnimationFrameData* frame) override;

public:
    void update(int time) override;
};

class BoneTimelineState final : public TweenTimelineState<BoneFrameData, BoneTimelineData>
{
    BIND_CLASS_TYPE(BoneTimelineState);

public:
    Bone* bone;

private:
    TweenType _tweenTransform;
    TweenType _tweenRotate;
    TweenType _tweenScale;
    Transform* _boneTransform;
    Transform* _originTransform;
    Transform _transform;
    Transform _currentTransform;
    Transform _durationTransform;

public:
    BoneTimelineState();
    ~BoneTimelineState();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(BoneTimelineState);

protected:
    void _onClear() override;
    void _onFadeIn() override;
    void _onArriveAtFrame(bool isUpdate) override;
    void _onUpdateFrame(bool isUpdate) override;

public:
    void fadeOut() override;
    void update(int time) override;
};

class SlotTimelineState final : public TweenTimelineState<SlotFrameData, SlotTimelineData>
{
    BIND_CLASS_TYPE(SlotTimelineState);

public:
    Slot* slot;

private:
    TweenType _tweenColor;
    ColorTransform* _slotColor;
    ColorTransform _durationColor;

public:
    SlotTimelineState();
    ~SlotTimelineState();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(SlotTimelineState);

protected:
    void _onClear() override;
    void _onFadeIn() override;
    void _onArriveAtFrame(bool isUpdate) override;
    void _onUpdateFrame(bool isUpdate) override;

public:
    void fadeOut() override;
    void update(int time) override;
};

class FFDTimelineState final : public TweenTimelineState<ExtensionFrameData, FFDTimelineData>
{
    BIND_CLASS_TYPE(FFDTimelineState);

public:
    Slot* slot;

private:
    TweenType _tweenFFD;
    std::vector<float>* _ffdVertices;
    ExtensionFrameData* _durationFFDFrame;

public:
    FFDTimelineState();
    ~FFDTimelineState();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(FFDTimelineState);

protected:
    void _onClear() override;
    void _onFadeIn() override;
    void _onArriveAtFrame(bool isUpdate) override;
    void _onUpdateFrame(bool isUpdate) override;

public:
    void fadeOut() override;
    void update(int time) override;
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_TIMELINE_STATE_H