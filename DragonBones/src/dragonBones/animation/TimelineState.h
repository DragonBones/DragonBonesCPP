#ifndef DRAGONBONES_TIMELINE_STATE_H
#define DRAGONBONES_TIMELINE_STATE_H

#include "BaseTimelineState.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* @private
*/
class ActionTimelineState : public TimelineState
{
    BIND_CLASS_TYPE_A(ActionTimelineState);

    void _onCrossFrame(unsigned frameIndex) const;

protected:
    virtual void _onArriveAtFrame() override {}
    virtual void _onUpdateFrame() override {}

public:
    void update(float passedTime) override;
    void setCurrentTime(float value);
};
/**
* @private
*/
class ZOrderTimelineState : public TimelineState 
{
    BIND_CLASS_TYPE_A(ZOrderTimelineState);

protected:
    virtual void _onArriveAtFrame() override;
    virtual void _onUpdateFrame() override {}
};
/**
* @private
*/
class BoneAllTimelineState : public BoneTimelineState
{
    BIND_CLASS_TYPE_A(BoneAllTimelineState);

protected:
    virtual void _onArriveAtFrame() override;
    virtual void _onUpdateFrame() override;

public:
    virtual void fadeOut() override;
};
/**
* @private
*/
class SlotDislayIndexTimelineState : public SlotTimelineState
{
    BIND_CLASS_TYPE_A(SlotDislayIndexTimelineState);

protected:
    virtual void _onArriveAtFrame() override;
};
/**
* @private
*/
class SlotColorTimelineState : public SlotTimelineState
{
    BIND_CLASS_TYPE_B(SlotColorTimelineState);

private:
    bool _dirty;
    int* _current;
    int* _delta;
    float* _result;

public:
    SlotColorTimelineState() :
        _current(new int[8]()),
        _delta(new int[8]()),
        _result(new float[8]())
    { 
        _onClear(); 
    }
    ~SlotColorTimelineState()
    {
        _onClear();

        delete _current;
        delete _delta;
        delete _result;
    }

protected:
    void _onClear() override;
    void _onArriveAtFrame() override;
    void _onUpdateFrame() override;

public:
    void fadeOut() override;
    void update(float passedTime) override;
};
/**
* @private
*/
class SlotFFDTimelineState : public SlotTimelineState
{
    BIND_CLASS_TYPE_A(SlotFFDTimelineState);

public:
    unsigned meshOffset;

private:
    bool _dirty;
    unsigned _frameFloatOffset;
    unsigned _ffdCount;
    unsigned _valueCount;
    unsigned _valueOffset;
    std::vector<float> _current;
    std::vector<float> _delta;
    std::vector<float> _result;

protected:
    virtual void _onClear() override;
    virtual void _onArriveAtFrame() override;
    virtual void _onUpdateFrame() override;

public:
    virtual void init(Armature* armature, AnimationState* animationState, TimelineData* timelineData) override;
    virtual void fadeOut() override;
    virtual void update(float passedTime) override;
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_TIMELINE_STATE_H
