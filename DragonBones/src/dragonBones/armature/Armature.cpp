#include "Armature.h"
#include "../model/TextureAtlasData.h"
#include "../model/UserData.h"
#include "../animation/WorldClock.h"
#include "../animation/Animation.h"
#include "../events/EventObject.h"
#include "IArmatureProxy.h"
#include "Bone.h"
#include "Slot.h"
#include "Constraint.h"

DRAGONBONES_NAMESPACE_BEGIN

bool Armature::_onSortSlots(Slot* a, Slot* b)
{
    return a->_zOrder > b->_zOrder ? true : false;
}

void Armature::_onClear()
{
    if (_clock != nullptr) // Remove clock first.
    {
        _clock->remove(this);
    }

    for (const auto bone : _bones)
    {
        bone->returnToPool();
    }

    for (const auto slot : _slots)
    {
        slot->returnToPool();
    }

    if(_animation != nullptr)
    {
        _animation->returnToPool();
    }

    if(_proxy != nullptr)
    {
        _proxy->clear();
    }

    if (_replaceTextureAtlasData != nullptr)
    {
        _replaceTextureAtlasData->returnToPool();
    }

    inheritAnimation = true;
    debugDraw = false;
    armatureData = nullptr;
    userData = nullptr;

    _debugDraw = false;
    _delayDispose = false;
    _bonesDirty = false;
    _slotsDirty = false;
    _zOrderDirty = false;
    _flipX = false;
    _flipY = false;
    _cacheFrameIndex = -1;
    _bones.clear();
    _slots.clear();
    _dragonBones = nullptr;
    _animation = nullptr;
    _proxy = nullptr;
    _display = nullptr;
    _clock = nullptr;
    _parent = nullptr;
    _replaceTextureAtlasData = nullptr;
    _replacedTexture = nullptr;
}

void Armature::_sortBones()
{
    const auto total = _bones.size();
    if (total <= 0) 
    {
        return;
    }

    auto sortHelper = _bones; // Copy
    unsigned index = 0;
    unsigned count = 0;

    _bones.resize(0);

    while (count < total) 
    {
        auto bone = sortHelper[index++];
        if (index >= total) 
        {
            index = 0;
        }

        if (std::find(_bones.cbegin(), _bones.cend(), bone) != _bones.cend()) 
        {
            continue;
        }

        if (bone->constraints.size() > 0) // Wait constraint.
        {
            auto flag = false;
            for (const auto constraint : bone->constraints) 
            {
                if (std::find(_bones.cbegin(), _bones.cend(), constraint->target) == _bones.cend()) 
                {
                    flag = true;
                    break;
                }
            }

            if(flag)
            {
                continue;
            }
        }

        if (bone->getParent() != nullptr && std::find(_bones.cbegin(), _bones.cend(), bone->getParent()) == _bones.cend()) // Wait parent.
        {
            continue;
        }

        _bones.push_back(bone);
        count++;
    }
}

void Armature::_sortSlots()
{
    std::sort(_slots.begin(), _slots.end(), Armature::_onSortSlots);
}

void Armature::_sortZOrder(int16_t* slotIndices, unsigned offset)
{
    const auto& slotDatas = armatureData->sortedSlots;
    const auto isOriginal = slotIndices == nullptr;

    if (_zOrderDirty || !isOriginal) 
    {
        for (std::size_t i = 0, l = slotDatas.size(); i < l; ++i) 
        {
            const auto slotIndex = isOriginal ? i : (std::size_t)slotIndices[offset + i];
            if (slotIndex < 0 || slotIndex >= l)
            {
                continue;
            }

            const auto slotData = slotDatas[slotIndex];
            const auto slot = getSlot(slotData->name);
            if (slot != nullptr) 
            {
                slot->_setZorder(i);
            }
        }

        _slotsDirty = true;
        _zOrderDirty = !isOriginal;
    }
}

void Armature::_addBoneToBoneList(Bone* value)
{
    if (std::find(_bones.begin(), _bones.end(), value) == _bones.end())
    {
        _bonesDirty = true;
        _bones.push_back(value);
        _animation->_timelineDirty = true;
    }
}

void Armature::_removeBoneFromBoneList(Bone* value)
{
    const auto iterator = std::find(_bones.begin(), _bones.end(), value);
    if (iterator != _bones.end())
    {
        _bones.erase(iterator);
        _animation->_timelineDirty = true;
    }
}

void Armature::_addSlotToSlotList(Slot* value)
{
    if (std::find(_slots.begin(), _slots.end(), value) == _slots.end())
    {
        _slotsDirty = true;
        _slots.push_back(value);
        _animation->_timelineDirty = true;
    }
}

void Armature::_removeSlotFromSlotList(Slot* value)
{
    const auto iterator = std::find(_slots.begin(), _slots.end(), value);
    if (iterator != _slots.end())
    {
        _slots.erase(iterator);
        _animation->_timelineDirty = true;
    }
}

void Armature::dispose()
{
    if (armatureData != nullptr) 
    {
        _delayDispose = true;
        _dragonBones->bufferObject(this);
    }
}

void Armature::init(ArmatureData *parmatureData, IArmatureProxy* pproxy, void* display, DragonBones* dragonBones)
{
    if (armatureData != nullptr)
    {
        return;
    }

    armatureData = parmatureData;
    _animation = BaseObject::borrowObject<Animation>();
    _proxy = pproxy;
    _display = display;
    _dragonBones = dragonBones;

    _animation->init(this);
    _animation->setAnimations(armatureData->animations);
    _proxy->_init(this);
}

void Armature::advanceTime(float passedTime)
{
    if (_delayDispose)
    {
        return;
    }

    if (armatureData == nullptr)
    {
        DRAGONBONES_ASSERT(false, "The armature has been disposed.");
        return;
    }
    else if(armatureData->parent == nullptr)
    {
        DRAGONBONES_ASSERT(false, "The armature data has been disposed.");
        return;
    }

    const auto prevCacheFrameIndex = _cacheFrameIndex;

    _animation->advanceTime(passedTime);

    //
    if (_bonesDirty)
    {
        _bonesDirty = false;
        _sortBones();
    }

    if (_slotsDirty)
    {
        _slotsDirty = false;
        _sortSlots();
    }

    if (_cacheFrameIndex < 0 || _cacheFrameIndex != prevCacheFrameIndex)
    {
        for (const auto bone : _bones)
        {
            bone->update(_cacheFrameIndex);
        }

        for (const auto slot : _slots)
        {
            slot->update(_cacheFrameIndex);
        }
    }

    const auto drawed = debugDraw || DragonBones::debugDraw;
    if (drawed || _debugDraw) 
    {
        _debugDraw = drawed;
        _proxy->debugUpdate(_debugDraw);
    }
}

void Armature::invalidUpdate(const std::string& boneName, bool updateSlotDisplay)
{
    if (!boneName.empty())
    {
        const auto bone = getBone(boneName);
        if (bone != nullptr)
        {
            bone->invalidUpdate();

            if (updateSlotDisplay)
            {
                for (const auto slot : _slots)
                {
                    if (slot->getParent() == bone)
                    {
                        slot->invalidUpdate();
                    }
                }
            }
        }
    }
    else
    {
        for (const auto bone : _bones)
        {
            bone->invalidUpdate();
        }

        if (updateSlotDisplay)
        {
            for (const auto slot : _slots)
            {
                slot->invalidUpdate();
            }
        }
    }
}

Slot* Armature::containsPoint(float x, float y) const
{
    for(const auto slot : _slots)
    {
        if(slot->containsPoint(x,y)) {
            return slot;
        }
    }

    return nullptr;
}

Slot* Armature::intersectsSegment(
    float xA, float yA, float xB, float yB,
    Point* intersectionPointA,
    Point* intersectionPointB,
    Point* normalRadians
) const
{
    const auto isV = xA == xB;
    auto dMin = 0.0f;
    auto dMax = 0.0f;
    auto intXA = 0.0f;
    auto intYA = 0.0f;
    auto intXB = 0.0f;
    auto intYB = 0.0f;
    auto intAN = 0.0f;
    auto intBN = 0.0f;
    Slot* intSlotA = nullptr;
    Slot* intSlotB = nullptr;

    for (const auto & slot : _slots) 
    {
        auto intersectionCount = slot->intersectsSegment(xA, yA, xB, yB, intersectionPointA, intersectionPointB, normalRadians);
        if (intersectionCount > 0) 
        {
            if (intersectionPointA != nullptr || intersectionPointB != nullptr) 
            {
                if (intersectionPointA != nullptr) 
                {
                    float d = isV ? intersectionPointA->y - yA : intersectionPointA->x - xA;
                    if (d < 0.0f) 
                    {
                        d = -d;
                    }

                    if (intSlotA == nullptr || d < dMin) 
                    {
                        dMin = d;
                        intXA = intersectionPointA->x;
                        intYA = intersectionPointA->y;
                        intSlotA = slot;

                        if (normalRadians) {
                            intAN = normalRadians->x;
                        }
                    }
                }

                if (intersectionPointB != nullptr) 
                {
                    float d = intersectionPointB->x - xA;
                    if (d < 0.0f) 
                    {
                        d = -d;
                    }

                    if (intSlotB == nullptr || d > dMax) 
                    {
                        dMax = d;
                        intXB = intersectionPointB->x;
                        intYB = intersectionPointB->y;
                        intSlotB = slot;

                        if (normalRadians != nullptr) 
                        {
                            intBN = normalRadians->y;
                        }
                    }
                }
            }
            else 
            {
                intSlotA = slot;
                break;
            }
        }
    }

    if (intSlotA != nullptr && intersectionPointA != nullptr)
    {
        intersectionPointA->x = intXA;
        intersectionPointA->y = intYA;

        if (normalRadians != nullptr) 
        {
            normalRadians->x = intAN;
        }
    }

    if (intSlotB != nullptr && intersectionPointB != nullptr) 
    {
        intersectionPointB->x = intXB;
        intersectionPointB->y = intYB;

        if (normalRadians != nullptr) 
        {
            normalRadians->y = intBN;
        }
    }

    return intSlotA;
}

Bone* Armature::getBone(const std::string& name) const
{
    for (const auto& bone : _bones)
    {
        if (bone->name == name)
        {
            return bone;
        }
    }

    return nullptr;
}

Bone* Armature::getBoneByDisplay(void* display) const
{
    const auto slot = getSlotByDisplay(display);

    return slot != nullptr ? slot->getParent() : nullptr;
}

Slot* Armature::getSlot(const std::string& name) const
{
    for (const auto slot : _slots)
    {
        if (slot->name == name)
        {
            return slot;
        }
    }

    return nullptr;
}

Slot* Armature::getSlotByDisplay(void* display) const
{
    if (display != nullptr)
    {
        for (const auto slot : _slots)
        {
            if (slot->getDisplay() == display)
            {
                return slot;
            }
        }
    }

    return nullptr;
}

void Armature::addBone(Bone* value, const std::string& parentName)
{
    DRAGONBONES_ASSERT(value != nullptr, "addBone argument error");

    value->_setArmature(this);
    value->_setParent(parentName.empty() ? nullptr : getBone(parentName));
}

void Armature::removeBone(Bone* value)
{
    DRAGONBONES_ASSERT(value != nullptr && value->getArmature() == this, "removeBone argument error");

    value->_setParent(nullptr);
    value->_setArmature(nullptr);
}

void Armature::addSlot(Slot* value, const std::string& boneName)
{
    const auto bone = getBone(boneName);

    DRAGONBONES_ASSERT(value != nullptr && bone != nullptr, "addSlot argument error");

    value->_setArmature(this);
    value->_setParent(bone);
}

void Armature::removeSlot(Slot* value)
{
    DRAGONBONES_ASSERT(value != nullptr && value->getArmature() == this, "removeSlot argument error");

    value->_setParent(nullptr);
    value->_setArmature(nullptr);
}

void Armature::setCacheFrameRate(unsigned value)
{
    if (armatureData->cacheFrameRate != value)
    {
        armatureData->cacheFrames(value);

        for (const auto & slot : _slots)
        {
            const auto childArmature = slot->getChildArmature();
            if (childArmature != nullptr && childArmature->getCacheFrameRate() == 0) 
            {
                childArmature->setCacheFrameRate(value);
            }
        }
    }
}

void Armature::setClock(WorldClock* value)
{
    if(_clock == value)
    {
        return;
    }

    if(_clock)
    {
        _clock->remove(this);
    }

    _clock = value;

    if(_clock)
    {
        _clock->add(this);
    }

    // Update childArmature clock.
    for (const auto& slot : _slots)
    {
        const auto childArmature = slot->getChildArmature();
        if (childArmature != nullptr) 
        {
            childArmature->setClock(_clock);
        }
    }
}

void Armature::setReplacedTexture(void* value)
{
    if (_replacedTexture == value) 
    {
        return;
    }

    if (_replaceTextureAtlasData != nullptr) 
    {
        _replaceTextureAtlasData->returnToPool();
        _replaceTextureAtlasData = nullptr;
    }

    _replacedTexture = value;

    for (const auto &slot : _slots) 
    {
        slot->invalidUpdate();
        slot->update(-1);
    }
}

DRAGONBONES_NAMESPACE_END
