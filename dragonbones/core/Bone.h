#ifndef __CORE_BONE_H__
#define __CORE_BONE_H__

#include "../DragonBones.h"
#include "Object.h"
#include "../objects/TransformFrame.h"
#include "Slot.h"
#include "../animation/TimelineState.h"
#include "Armature.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class Bone : public Object
{
    friend class Armature;
    friend class Slot;
    friend class AnimationState;
    friend class TimelineState;
    
private:
    static bool sortState(const TimelineState *a, const TimelineState *b);
    
public:
    String displayController;
    
protected:
    bool _isColorChanged;
    int _needUpdate;
    
    Point _tweenPivot;
    Transform _tween;
    std::vector<Bone *> _boneList;
    std::vector<Slot *> _slotList;
    std::vector<TimelineState *> _timelineStateList;
    
public:
    Slot *getSlot() const;
    const std::vector<Slot *> &getSlots() const;
    const std::vector<Bone *> &getBones() const;
    
    virtual void setVisible(bool vislble) override;
    
protected:
    virtual void setArmature(Armature *armature) override;
    
public:
    Bone();
    virtual ~Bone();
    virtual void dispose() override;
    
    void invalidUpdate();
    bool contains(const Object *object) const;
    void addChild(Object *object);
    void removeChild(Object *object);
    
protected:
    void update(bool needUpdate);
    void updateColor(
        int aOffset,
        int rOffset,
        int gOffset,
        int bOffset,
        float aMultiplier,
        float rMultiplier,
        float gMultiplier,
        float bMultiplier,
        bool colorChanged
    );
    void hideSlots();
    void arriveAtFrame(const TransformFrame *frame, const TimelineState *timelineState, AnimationState *animationState, const bool isCross);
    void addState(TimelineState *timelineState);
    void removeState(TimelineState *timelineState);
    void blendingTimeline();
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(Bone);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __CORE_BONE_H__
