#ifndef DRAGONBONES_ARMATURE_H
#define DRAGONBONES_ARMATURE_H

#include "../core/BaseObject.h"
#include "../model/ArmatureData.h"
#include "../animation/IAnimateble.h"
#include "../events/EventObject.h"
#include "../events/IEventDispatcher.h"
#include "IArmatureDisplayContainer.h"

NAMESPACE_DRAGONBONES_BEGIN

class Bone;
class Slot;
class Animation;

class Armature : public BaseObject, public IAnimateble
{
    BIND_CLASS_TYPE(Armature);

public:
    static const IEventDispatcher* soundEventManager;

public:
    void* userData;

public:
    bool _bonesDirty;
    int _cacheFrameIndex;
    ArmatureData* _armatureData;
    SkinData* _skinData;
    Animation* _animation;
    IArmatureDisplayContainer* _display;
    Slot* _parent;

protected:
    bool _delayDispose;
    bool _lockDispose;
    bool _lockEvent;
    bool _slotsDirty;
    std::vector<Bone*> _bones;
    std::vector<Slot*> _slots;
    std::vector<EventObject*> _events;

public:
    Armature();
    ~Armature();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(Armature);

protected:
    void _onClear() override;
    void _sortBones();
    void _sortSlots();

public:
    void _addBoneToBoneList(Bone* value);
    void _removeBoneFromBoneList(Bone* value);
    void _addSlotToSlotList(Slot* value);
    void _removeSlotFromSlotList(Slot* value);
    void _bufferEvent(EventObject* value, const std::string& type);

public:
    virtual void advanceTime(float passedTime) override;
    void invalidUpdate(const std::string& boneName = "");
    Slot* getSlot(const std::string& name) const;
    Slot* getSlotByDisplay(void* display) const;
    void addSlot(Slot* value, const std::string& boneName);
    void removeSlot(Slot* value);
    Bone* getBone(const std::string& name) const;
    Bone* getBoneByDisplay(void* display) const;
    void addBone(Bone* value, const std::string& parentName = "");
    void removeBone(Bone* value);

    inline const std::vector<Bone*>& getBones() const
    {
        return _bones;
    }

    inline const std::vector<Slot*>& getSlots() const
    {
        return _slots;
    }

    inline const std::string& getName() const
    {
        static const auto DEFAULT_NAME = "";
        return _armatureData? _armatureData->name: DEFAULT_NAME;
    }

    inline const ArmatureData& getArmatureData() const
    {
        return *_armatureData;
    }

    inline IArmatureDisplayContainer* getDisplay() const
    {
        return _display;
    }

    inline Slot* getParent() const
    {
        return _parent;
    }

    inline Animation& getAnimation() const
    {
        return *_animation;
    }

    inline unsigned getCacheFrameRate()
    {
        return _armatureData->cacheFrameRate;
    }
    void setCacheFrameRate(unsigned value);
};

NAMESPACE_DRAGONBONES_END
#endif // DRAGONBONES_ARMATURE_H
