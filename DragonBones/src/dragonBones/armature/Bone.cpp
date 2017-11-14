#include "Bone.h"
#include "../geom/Matrix.h"
#include "../geom/Transform.h"
#include "Armature.h"
#include "Slot.h"
#include "Constraint.h"
#include "cocos2d.h"

DRAGONBONES_NAMESPACE_BEGIN

void Bone::_onClear()
{
    TransformObject::_onClear();

    offsetMode = OffsetMode::Additive;
    animationPose.identity();

    _transformDirty = false;
    _childrenTransformDirty = false;
    _blendDirty = false;
    _localDirty = true;
    _hasConstraint = false;
    _visible = true;
    _cachedFrameIndex = -1;
    _blendLayer = 0;
    _blendLeftWeight = 1.0f;
    _blendLayerWeight = 0.0f;
    _boneData = nullptr;
    _cachedFrameIndices = nullptr;
}

void Bone::_updateGlobalTransformMatrix(bool isCache)
{
    const auto flipX = _armature->getFlipX();
    const auto flipY = _armature->getFlipY() == DragonBones::yDown;
    auto inherit = _parent != nullptr;
    auto rotation = 0.0f;

    if (offsetMode == OffsetMode::Additive) 
    {
        // global.copyFrom(origin).add(offset).add(animationPose);
        global.x = origin->x + offset.x + animationPose.x;
        global.y = origin->y + offset.y + animationPose.y;
        global.skew = origin->skew + offset.skew + animationPose.skew;
        global.rotation = origin->rotation + offset.rotation + animationPose.rotation;
        global.scaleX = origin->scaleX * offset.scaleX * animationPose.scaleX;
        global.scaleY = origin->scaleY * offset.scaleY * animationPose.scaleY;
    }
    else if (offsetMode == OffsetMode::None) 
    {
        global = *origin;
        global.add(animationPose);
    }
    else 
    {
        inherit = false;
        global = offset;
    }

    if (inherit) 
    {
        const auto& parentMatrix = _parent->globalTransformMatrix;
        if (_boneData->inheritScale) 
        {
            if (!_boneData->inheritRotation) 
            {
                _parent->updateGlobalTransform();

                if (flipX && flipY) 
                {
                    rotation = global.rotation - (_parent->global.rotation + Transform::PI);
                }
                else if (flipX) 
                {
                    rotation = global.rotation + _parent->global.rotation + Transform::PI;
                }
                else if (flipY) 
                {
                    rotation = global.rotation + _parent->global.rotation;
                }
                else 
                {
                    rotation = global.rotation - _parent->global.rotation;
                }

                global.rotation = rotation;
            }

            global.toMatrix(globalTransformMatrix);
            globalTransformMatrix.concat(parentMatrix);

            if (_boneData->inheritTranslation) 
            {
                global.x = globalTransformMatrix.tx;
                global.y = globalTransformMatrix.ty;
            }
            else 
            {
                globalTransformMatrix.tx = global.x;
                globalTransformMatrix.ty = global.y;
            }

            if (isCache) 
            {
                global.fromMatrix(globalTransformMatrix);
            }
            else 
            {
                _globalDirty = true;
            }
        }
        else 
        {
            if (_boneData->inheritTranslation) 
            {
                const auto x = global.x;
                const auto y = global.y;
                global.x = parentMatrix.a * x + parentMatrix.c * y + parentMatrix.tx;
                global.y = parentMatrix.d * y + parentMatrix.b * x + parentMatrix.ty;
            }
            else 
            {
                if (flipX)
                {
                    global.x = -global.x;
                }

                if (flipY)
                {
                    global.y = -global.y;
                }
            }

            if (_boneData->inheritRotation) 
            {
                _parent->updateGlobalTransform();


                if (_parent->global.scaleX < 0.0f)
                {
                    rotation = global.rotation + _parent->global.rotation + Transform::PI;
                }
                else 
                {
                    rotation = global.rotation + _parent->global.rotation;
                }

                if (parentMatrix.a * parentMatrix.d - parentMatrix.b * parentMatrix.c < 0.0f)
                {
                    rotation -= global.rotation * 2.0f;

                    if (flipX != flipY || _boneData->inheritReflection) 
                    {
                        global.skew += Transform::PI;
                    }
                }

                global.rotation = rotation;
            }
            else if (flipX || flipY) 
            {
                if (flipX && flipY) 
                {
                    rotation = global.rotation + Transform::PI;
                }
                else 
                {
                    if (flipX) 
                    {
                        rotation = Transform::PI - global.rotation;
                    }
                    else
                    {
                        rotation = -global.rotation;
                    }

                    global.skew += Transform::PI;
                }

                global.rotation = rotation;
            }

            global.toMatrix(globalTransformMatrix);
        }
    }
    else 
    {
        if (flipX || flipY) 
        {
            if (flipX) 
            {
                global.x = -global.x;
            }

            if (flipY) 
            {
                global.y = -global.y;
            }

            if (flipX && flipY) 
            {
                rotation = global.rotation + Transform::PI;
            }
            else 
            {
                if (flipX) 
                {
                    rotation = Transform::PI - global.rotation;
                }
                else 
                {
                    rotation = -global.rotation;
                }

                global.skew += Transform::PI;
            }

            global.rotation = rotation;
        }

        global.toMatrix(globalTransformMatrix);
    }
}

void Bone::_setArmature(Armature* value)
{
    if (_armature == value) 
    {
        return;
    }

    std::vector<Slot*> oldSlots;
    std::vector<Bone*> oldBones;

    if (_armature != nullptr)
    {
        oldSlots = getSlots();
        oldBones = getBones();
        _armature->_removeBoneFromBoneList(this);
    }

    _armature = value; //

    if (_armature != nullptr)
    {
        _armature->_addBoneToBoneList(this);
    }

    if (!oldSlots.empty()) 
    {
        for (const auto &slot : oldSlots) 
        {
            if (slot->getParent()== this) 
            {
                slot->_setArmature(_armature);
            }
        }
    }

    if (!oldBones.empty()) 
    {
        for (const auto &bone : oldBones) 
        {
            if (bone->getParent() == this)  
            {
                bone->_setArmature(_armature);
            }
        }
    }
}

void Bone::init(BoneData* boneData)
{
    if (_boneData != nullptr) 
    {
        return;
    }

    _boneData = boneData;
    //
    origin = &_boneData->transform;
}

void Bone::update(int cacheFrameIndex)
{
    _blendDirty = false;

    if (cacheFrameIndex >= 0 && _cachedFrameIndices != nullptr) 
    {
        const auto cachedFrameIndex = (*_cachedFrameIndices)[cacheFrameIndex];
        if (cachedFrameIndex >= 0 && _cachedFrameIndex == cachedFrameIndex) // Same cache.
        {
            _transformDirty = false;
        }
        else if (cachedFrameIndex >= 0) // Has been Cached.
        {
            _transformDirty = true;
            _cachedFrameIndex = cachedFrameIndex;
        }
        else 
        {
            if (_hasConstraint) // Update constraints.
            {
                for (const auto constraint : _armature->_constraints) 
                {
                    if (constraint->_bone == this)
                    {
                        constraint->update();
                    }
                }
            }

            if ( _transformDirty || (_parent != nullptr && _parent->_childrenTransformDirty)) // Dirty.
            {
                _transformDirty = true;
                _cachedFrameIndex = -1;
            }
            else if (_cachedFrameIndex >= 0) // Same cache, but not set index yet.
            {
                _transformDirty = false;
                (*_cachedFrameIndices)[cacheFrameIndex] = _cachedFrameIndex;
            }
            else // Dirty.
            {
                _transformDirty = true;
                _cachedFrameIndex = -1;
            }
        }
    }
    else 
    {
        if (_hasConstraint) // Update constraints.
        {
            for (const auto constraint : _armature->_constraints)
            {
                if (constraint->_bone == this)
                {
                    constraint->update();
                }
            }
        }

        if ( _transformDirty || (_parent != nullptr && _parent->_childrenTransformDirty)) // Dirty.
        {
            cacheFrameIndex = -1;
            _transformDirty = true;
            _cachedFrameIndex = -1;
        }
    }

    if (_transformDirty) 
    {
        _transformDirty = false;
        _childrenTransformDirty = true;

        if (_cachedFrameIndex < 0) 
        {
            const auto isCache = cacheFrameIndex >= 0;
            if (_localDirty) 
            {
                _updateGlobalTransformMatrix(isCache);
            }

            if (isCache && _cachedFrameIndices != nullptr) 
            {
                _cachedFrameIndex = (*_cachedFrameIndices)[cacheFrameIndex] = _armature->_armatureData->setCacheFrame(globalTransformMatrix, global);
            }
        }
        else 
        {
            _armature->_armatureData->getCacheFrame(globalTransformMatrix, global, _cachedFrameIndex);
        }
    }
    else if (_childrenTransformDirty) 
    {
        _childrenTransformDirty = false;
    }

    _localDirty = true;
}

void Bone::updateByConstraint()
{
    if (_localDirty && (_transformDirty || (_parent != nullptr && _parent->_childrenTransformDirty)))
    {
        _localDirty = false;
        _transformDirty = true;
        _updateGlobalTransformMatrix(true);
    }
}

bool Bone::contains(const TransformObject* value) const
{
    if (value == this)
    {
        return false;
    }

    auto ancestor = value;
    while (ancestor != this && ancestor != nullptr)
    {
        ancestor = ancestor->getParent();
    }

    return ancestor == this;
}

const std::vector<Bone*> Bone::getBones() const
{
    std::vector<Bone*> bones;

    for (const auto &bone : _armature->getBones())
    {
        if (bone->getParent() == this)
        {
            bones.push_back(bone);
        }
    }

    return bones;
}

const std::vector<Slot*> Bone::getSlots() const
{
    std::vector<Slot*> slots;

    for (const auto &slot : _armature->getSlots())
    {
        if (slot->getParent() == this)
        {
            slots.push_back(slot);
        }
    }

    return slots;
}

void Bone::setVisible(bool value)
{
    if (_visible == value)
    {
        return;
    }

    _visible = value;

    for (const auto & slot : _armature->getSlots())
    {
        if (slot->_parent == this)
        {
            slot->_updateVisible();
        }
    }
}

DRAGONBONES_NAMESPACE_END
