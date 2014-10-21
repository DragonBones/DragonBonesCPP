#ifndef ANIMATION_TIMELINE_STATE_H
#define ANIMATION_TIMELINE_STATE_H

#include "../DragonBones.h"
#include "../geoms/ColorTransform.h"
#include "../geoms/Point.h"
#include "../geoms/Transform.h"
#include "../objects/TransformTimeline.h"
#include "../core/Bone.h"
#include "../animation/AnimationState.h"

NAME_SPACE_DRAGON_BONES_BEGIN

class AnimationState;

class TimelineState
{
    friend class AnimationState;
    friend class Bone;
    
private:
    enum class UpdateState {UPDATE, UPDATE_ONCE, UNUPDATE};

    static std::vector<TimelineState*> _pool;
    static TimelineState* borrowObject();
    static void returnObject(TimelineState *timelineState);
    static void clearObjects();
    
public:
    std::string name;
    
private:
    bool _blendEnabled;
    bool _isComplete;
    bool _tweenTransform;
    bool _tweenScale;
    bool _tweenColor;
    int _currentTime;
    int _currentFrameIndex;
    int _currentFramePosition;
    int _currentFrameDuration;
    int _totalTime;
    float _weight;
    float _tweenEasing;
    
    UpdateState _updateState;
    Transform _transform;
    Transform _durationTransform;
    Transform _originTransform;
    Point _pivot;
    Point _durationPivot;
    Point _originPivot;
    ColorTransform _durationColor;
    
    Bone *_bone;
    AnimationState *_animationState;
    TransformTimeline *_timeline;
    
public:
    TimelineState();
    virtual ~TimelineState();
    
private:
    void fadeIn(Bone *bone, AnimationState *animationState, TransformTimeline *timeline);
    void fadeOut();
    void update(float progress);
    void updateMultipleFrame(float progress);
    void updateToNextFrame(int currentPlayTimes);
    void updateTween();
    void updateSingleFrame();
    void clear();
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(TimelineState);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // ANIMATION_TIMELINE_STATE_H
