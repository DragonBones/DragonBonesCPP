#ifndef CORE_BONE_H
#define CORE_BONE_H

#include "../DragonBones.h"
#include "Object.h"
#include "../objects/TransformFrame.h"
#include "Slot.h"
#include "../animation/AnimationState.h"
#include "../animation/TimelineState.h"
#include "Armature.h"

NAME_SPACE_DRAGON_BONES_BEGIN

class AnimationState;
class TimelineState;
class Slot;

class Bone : public Object
{
    friend class Armature;
    friend class Slot;
    friend class AnimationState;
    friend class TimelineState;
    
private:
    static bool sortState(const TimelineState *a, const TimelineState *b);
    
public:
    std::string displayController;
    
protected:
    bool _isColorChanged;
    int _needUpdate;
    
    Point _tweenPivot;
    Transform _tween;
    std::vector<Bone*> _boneList;
    std::vector<Slot*> _slotList;
    std::vector<TimelineState*> _timelineStateList;
    
public:
    virtual Slot* getSlot() const;
    virtual const std::vector<Slot*>& getSlots() const;
    virtual const std::vector<Bone*>& getBones() const;
    
    virtual void setVisible(bool vislble) override;
    
    
    
protected:
    virtual void setArmature(Armature *armature) override;
    
public:
    Bone();
    virtual ~Bone();
    virtual void dispose() override;
    
    virtual void invalidUpdate();
    virtual bool contains(const Object *object) const;
    virtual void addChild(Object *object);
    virtual void removeChild(Object *object);
    
protected:
    virtual void update(bool needUpdate);
    virtual void updateColor(
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
    virtual void hideSlots();
    virtual void arriveAtFrame(TransformFrame *frame, const TimelineState *timelineState, AnimationState *animationState, bool isCross);
    virtual void addState(TimelineState *timelineState);
    virtual void removeState(TimelineState *timelineState);
    virtual void blendingTimeline();
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(Bone);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // CORE_BONE_H
