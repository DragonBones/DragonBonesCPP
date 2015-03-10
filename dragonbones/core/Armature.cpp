#include "Armature.h"

NAME_SPACE_DRAGON_BONES_BEGIN

IEventDispatcher *Armature::soundEventDispatcher = nullptr;

bool Armature::sortBone(const std::pair<int, Bone*> &a, const std::pair<int, Bone*> &b)
{
    return a.first > b.first;
}

bool Armature::sortSlot(const Slot *a, const Slot *b)
{
    return a->getZOrder() < b->getZOrder();
}

const std::vector<Bone*>& Armature::getBones() const
{
    return _boneList;
}

const std::vector<Slot*>& Armature::getSlots() const
{
    return _slotList;
}

const ArmatureData* Armature::getArmatureData() const
{
    return _armatureData;
}

Animation* Armature::getAnimation() const
{
    return _animation;
}

void* Armature::getDisplay() const
{
    return _display;
}

IEventDispatcher* Armature::getEventDispatcher() const
{
    return _eventDispatcher;
}

Armature::Armature(ArmatureData *armatureData, Animation *animation, IEventDispatcher *eventDispatcher, void *display)
    :_isInheritAnimation(true)
{
    _armatureData = armatureData;
    _animation = animation;
    _eventDispatcher = eventDispatcher;
    _display = display;
    _animation->_armature = this;
    _needUpdate = false;
    _slotsZOrderChanged = false;
    _delayDispose = false;
    _lockDispose = false;
    userData = nullptr;
}
Armature::~Armature()
{
    dispose();
}
void Armature::dispose()
{
    _delayDispose = true;
    if(!_animation || _lockDispose)
    {
        return;
    }

    if (_animation)
    {
        _animation->dispose();
        delete _animation;
        _animation = nullptr;
    }
    
    //
    for (size_t i = 0, l = _boneList.size(); i < l; ++i)
    {
        if (_boneList[i])
        {
            _boneList[i]->dispose();
            delete _boneList[i];
            _boneList[i] = nullptr;
        }
    }
    
    //
    for (size_t i = 0, l = _slotList.size(); i < l; ++i)
    {
        if (_slotList[i])
        {
            _slotList[i]->dispose();
            delete _slotList[i];
            _slotList[i] = nullptr;
        }
    }
    
    //
    for (size_t i = 0, l = _eventDataList.size(); i < l; ++i)
    {
        if (_eventDataList[i])
        {
            EventData::returnObject(_eventDataList[i]);
        }
    }
    
    //
    _boneList.clear();
    _slotList.clear();
    _eventDataList.clear();
    
    if (_eventDispatcher)
    {
        _eventDispatcher->dispose();
        delete _eventDispatcher;
        _eventDispatcher = nullptr;
    }
    
    if (_display)
    {
        _display = nullptr;
    }
    
    if (userData)
    {
        delete userData;
        userData = nullptr;
    }
}

Bone* Armature::getBone(const std::string &boneName) const
{
    if (boneName.empty())
    {
        // throw
    }
    
    for (size_t i = 0, l = _boneList.size(); i < l; ++i)
    {
        if (_boneList[i]->name == boneName)
        {
            return _boneList[i];
        }
    }
    
    return nullptr;
}

Bone* Armature::getBoneByDisplay(const void *display) const
{
    if (!display)
    {
        // throw
    }
    
    Slot *slot = getSlotByDisplay(display);
    return slot ? slot->_parent : nullptr;
}

void Armature::addBone(Bone *bone)
{
    if (!bone)
    {
        // throw
    }
    
    if (bone->_parent)
    {
        bone->_parent->removeChild(bone);
    }
    
    bone->setArmature(this);
}

void Armature::addBone(Bone *bone, const std::string &parentBoneName)
{
    if (parentBoneName.empty())
    {
        // throw
    }
    
    Bone *boneParent = getBone(parentBoneName);
    
    if (!boneParent)
    {
        // throw
    }
    
    boneParent->addChild(bone);
}

void Armature::removeBone(Bone *bone)
{
    if (!bone || bone->_armature != this)
    {
        // throw
    }
    
    if (bone->_parent)
    {
        bone->_parent->removeChild(bone);
    }
    else
    {
        bone->setArmature(nullptr);
    }
}

Bone *Armature::removeBone(const std::string &boneName)
{
    if (boneName.empty())
    {
        // throw
    }
    
    Bone *bone = getBone(boneName);
    
    if (bone)
    {
        removeBone(bone);
    }
    
    return bone;
}

Slot* Armature::getSlot(const std::string &slotName) const
{
    if (slotName.empty())
    {
        // throw
    }
    
    for (size_t i = 0, l = _slotList.size(); i < l; ++i)
    {
        if (_slotList[i]->name == slotName)
        {
            return _slotList[i];
        }
    }
    
    return nullptr;
}

Slot* Armature::getSlotByDisplay(const void *display) const
{
    if (!display)
    {
        // throw
    }
    
    for (size_t i = 0, l = _slotList.size(); i < l; ++i)
    {
        if (_slotList[i]->_display == display)
        {
            return _slotList[i];
        }
    }
    
    return nullptr;
}

void Armature::addSlot(Slot *slot, const std::string &parentBoneName)
{
    Bone *bone = getBone(parentBoneName);
    
    if (!bone)
    {
        // throw
    }
    
    bone->addChild(slot);
}

void Armature::removeSlot(Slot *slot)
{
    if (!slot || slot->_armature != this)
    {
        // throw
    }
    
    slot->_parent->removeChild(slot);
}

Slot* Armature::removeSlot(const std::string &slotName)
{
    Slot *slot = getSlot(slotName);
    
    if (slot)
    {
        removeSlot(slot);
    }
    
    return slot;
}

void Armature::replaceSlot(const std::string &boneName, const std::string &oldSlotName, Slot* newSlot)
{
    auto bone = getBone(boneName);
    if (!bone) return;

    auto slots = bone->getSlots();
    auto it = std::find_if(slots.begin(), slots.end(), 
        [&oldSlotName](Slot* tmp){return oldSlotName == tmp->name;});
    if (it != slots.end())
    {
        auto oldSlog = *it;
        newSlot->_tweenZOrder = oldSlog->_tweenZOrder;
        newSlot->_originZOrder = oldSlog->_originZOrder;
        newSlot->_offsetZOrder = oldSlog->_offsetZOrder;
        newSlot->_blendMode = oldSlog->_blendMode;
        removeSlot(oldSlog);
    }

    newSlot->name = oldSlotName;
    bone->addChild(newSlot);
}

void Armature::sortSlotsByZOrder()
{
    /*
    for (size_t i = 0, l = _slotList.size(); i < l; ++i)
    {
        Slot *slot = _slotList[i];

        if (slot->_isShowDisplay)
        {
            slot->addDisplayToContainer(_display, i);
        }
    }
    */

    std::sort(_slotList.begin() , _slotList.end() , sortSlot);
    
    for (size_t i = 0, l = _slotList.size(); i < l; ++i)
    {
        Slot *slot = _slotList[i];
        
        if (slot->_isShowDisplay)
        {
            slot->removeDisplayFromContainer();
        }
    }
    
    for (size_t i = 0, l = _slotList.size(); i < l; ++i)
    {
        Slot *slot = _slotList[i];
        
        if (slot->_isShowDisplay)
        {
            slot->addDisplayToContainer(_display, -1);
        }
    }
    
    _slotsZOrderChanged = false;
}

void Armature::invalidUpdate()
{
    for (size_t i = 0, l = _boneList.size(); i < l; ++i)
    {
        _boneList[i]->invalidUpdate();
    }
}

void Armature::invalidUpdate(const std::string &boneName)
{
    if (boneName.empty())
    {
        // throw
    }
    
    Bone *bone = getBone(boneName);
    
    if (bone)
    {
        bone->invalidUpdate();
    }
}

void Armature::advanceTime(float passedTime)
{
    _lockDispose = true;
    _animation->advanceTime(passedTime);
    passedTime *= _animation->_timeScale;
    const bool isFading = _animation->_isFading;
    
    for (size_t i = _boneList.size(); i--;)
    {
        _boneList[i]->update(isFading);
    }
    
    for (size_t i = _slotList.size(); i--;)
    {
        Slot *slot = _slotList[i];
        slot->update();
        
        if (slot->_isShowDisplay && slot->_childArmature)
        {
            slot->_childArmature->advanceTime(passedTime);
        }
    }
    
    if (_slotsZOrderChanged)
    {
        sortSlotsByZOrder();
        
#ifdef NEED_Z_ORDER_UPDATED_EVENT
        if (_eventDispatcher->hasEvent(EventData::EventType::Z_ORDER_UPDATED))
        {
            EventData *eventData = new EventData(EventData::EventType::Z_ORDER_UPDATED, this);
            _eventDataList.push_back(eventData);
        }
#endif
    }
    
    if (!_eventDataList.empty())
    {
        for (size_t i = 0, l = _eventDataList.size(); i < l; ++i)
        {
            _eventDispatcher->dispatchEvent(_eventDataList[i]);
            EventData::returnObject(_eventDataList[i]);
        }
        
        _eventDataList.clear();
    }
    
    _lockDispose = false;
    
    if (_delayDispose)
    {
        dispose();
    }
}

void Armature::addObject(Object *object)
{
    Bone *bone = dynamic_cast<Bone*>(object);
    Slot *slot = dynamic_cast<Slot*>(object);
    
    if (bone)
    {
        auto iterator = std::find(_boneList.cbegin(), _boneList.cend(), bone);
        
        if (iterator == _boneList.cend())
        {
            _boneList.push_back(bone);
            sortBones();
            _animation->updateAnimationStates();
        }
    }
    else if (slot)
    {
        auto iterator = std::find(_slotList.cbegin(), _slotList.cend(), slot);
        
        if (iterator == _slotList.cend())
        {
            _slotList.push_back(slot);
        }
    }
}

void Armature::removeObject(Object *object)
{
    Bone *bone = dynamic_cast<Bone*>(object);
    Slot *slot = dynamic_cast<Slot*>(object);
    
    if (bone)
    {
        auto iterator = std::find(_boneList.begin(), _boneList.end(), bone);
        
        if (iterator != _boneList.end())
        {
            _boneList.erase(iterator);
            _animation->updateAnimationStates();
        }
    }
    else if (slot)
    {
        auto iterator = std::find(_slotList.begin(), _slotList.end(), slot);
        
        if (iterator != _slotList.end())
        {
            _slotList.erase(iterator);
        }
    }
}

void Armature::sortBones()
{
    if (_boneList.empty())
    {
        return;
    }
    
    std::vector<std::pair<int , Bone*>> sortedList;
    
    for (size_t i = 0, l = _boneList.size(); i < l; ++i)
    {
        Bone *bone = _boneList[i];
        Bone *parentBone = bone;
        int level = 0;
        
        while (parentBone)
        {
            parentBone = parentBone->_parent;
            ++level;
        }
        
        sortedList.push_back(std::make_pair(level , bone));
    }
    
    std::sort(sortedList.begin() , sortedList.end() , sortBone);
    
    for (size_t i = 0, l = sortedList.size(); i < l; ++i)
    {
        _boneList[i] = sortedList[i].second;
    }
}

void Armature::arriveAtFrame(Frame *frame, AnimationState *animationState, bool isCross)
{
    if (!frame->event.empty() && _eventDispatcher->hasEvent(EventData::EventType::ANIMATION_FRAME_EVENT))
    {
        EventData *eventData = EventData::borrowObject(EventData::EventType::ANIMATION_FRAME_EVENT);
        eventData->armature = this;
        eventData->animationState = animationState;
        eventData->frameLabel = frame->event;
        eventData->frame = frame;
        _eventDataList.push_back(eventData);
    }
    
    if (!frame->sound.empty() && soundEventDispatcher && soundEventDispatcher->hasEvent(EventData::EventType::SOUND))
    {
        EventData *eventData = EventData::borrowObject(EventData::EventType::SOUND);
        eventData->armature = this;
        eventData->animationState = animationState;
        eventData->sound = frame->sound;
        soundEventDispatcher->dispatchEvent(eventData);
        EventData::returnObject(eventData);
    }
    
    if (!frame->action.empty())
    {
        if (animationState->displayControl)
        {
            _animation->gotoAndPlay(frame->action);
        }
    }
}
NAME_SPACE_DRAGON_BONES_END
