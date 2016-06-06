#include "Armature.h"
#include "Bone.h"
#include "Slot.h"
#include "../model/TimelineData.h"

NAMESPACE_DRAGONBONES_BEGIN

Bone::Bone()
{
    _onClear();
}
Bone::~Bone()
{
    _onClear();
}

void Bone::_onClear()
{
    TransformObject::_onClear();

    inheritTranslation = true;
    inheritRotation = true;
    inheritScale = true;
    ikBendPositive = true;
    ikWeight = 1.f;
    length = 0.f;

    _transformDirty = BoneTransformDirty::All;
    _cacheFrames = nullptr;
    _animationPose.identity();

    _visible = true;
    _ikChain = 0;
    _ikChainIndex = 0;
    _ik = nullptr;

    _bones.clear();
    _slots.clear();
}

void Bone::_updateGlobalTransformMatrix()
{
    if (this->_parent)
    {
        const auto parentRotation = this->_parent->global.skewY;
        const auto& parentMatrix = *this->_parent->globalTransformMatrix;

        if (inheritScale)
        {
            if (!inheritRotation)
            {
                this->global.skewX -= parentRotation;
                this->global.skewY -= parentRotation;
            }

            this->global.toMatrix(*this->globalTransformMatrix);
            this->globalTransformMatrix->concat(parentMatrix);

            if (!inheritTranslation)
            {
                this->globalTransformMatrix->tx = this->global.x;
                this->globalTransformMatrix->ty = this->global.y;
            }

            this->global.fromMatrix(*this->globalTransformMatrix);
        }
        else
        {
            if (inheritTranslation)
            {
                const auto x = this->global.x;
                const auto y = this->global.y;
                this->global.x = parentMatrix.a * x + parentMatrix.c * y + parentMatrix.tx;
                this->global.y = parentMatrix.d * y + parentMatrix.b * x + parentMatrix.ty;
            }

            if (inheritRotation)
            {
                this->global.skewX += parentRotation;
                this->global.skewY += parentRotation;
            }

            this->global.toMatrix(*this->globalTransformMatrix);
        }
    }
    else
    {
        this->global.toMatrix(*this->globalTransformMatrix);
    }
}

void Bone::_computeIK()
{
}

void Bone::_setArmature(Armature* value)
{
    if (this->_armature == value)
    {
        return;
    }

    _ik = nullptr;

    if (this->_armature)
    {
        this->_armature->_removeBoneFromBoneList(this);
    }

    this->_armature = value;

    if (this->_armature)
    {
        this->_armature->_addBoneToBoneList(this);
    }
}

void Bone::_setIK(Bone* value, unsigned chain, unsigned chainIndex)
{
    if (value)
    {
        if (chain == chainIndex)
        {
            auto chainEnd = this->_parent;
            if (chain && chainEnd)
            {
                chain = 1;
            }
            else
            {
                chain = 0;
                chainIndex = 0;
                chainEnd = this;
            }

            if (chainEnd == value || chainEnd->contains(value))
            {
                value = nullptr;
                chain = 0;
                chainIndex = 0;
            }
            else
            {
                auto ancestor = value;
                while (ancestor->getIK() && ancestor->getIKChain())
                {
                    if (chainEnd->contains(ancestor->getIK()))
                    {
                        value = nullptr;
                        chain = 0;
                        chainIndex = 0;
                        break;
                    }

                    ancestor = ancestor->getParent();
                }
            }
        }
    }
    else
    {
        chain = 0;
        chainIndex = 0;
    }

    _ik = value;
    _ikChain = chain;
    _ikChainIndex = chainIndex;

    if (this->_armature)
    {
        this->_armature->_bonesDirty = true;
    }
}

void Bone::_update(int cacheFrameIndex)
{
    if (cacheFrameIndex >= 0)
    {
        auto& cacheFrames = *_cacheFrames;
        const auto cacheFrame = cacheFrames[cacheFrameIndex];

        if (this->globalTransformMatrix == cacheFrame)
        {
            _transformDirty = BoneTransformDirty::None;
        }
        else if (cacheFrame)
        {
            _transformDirty = BoneTransformDirty::All;
            this->globalTransformMatrix = cacheFrame;
        }
        else if (
            _transformDirty == BoneTransformDirty::All ||
            (this->_parent && this->_parent->_transformDirty != BoneTransformDirty::None) ||
            (_ik && _ik->_transformDirty != BoneTransformDirty::None)
            )
        {
            _transformDirty = BoneTransformDirty::All;
            this->globalTransformMatrix = &this->_globalTransformMatrix;
        }
        else if (this->globalTransformMatrix != &this->_globalTransformMatrix)
        {
            _transformDirty = BoneTransformDirty::None;
            cacheFrames[cacheFrameIndex] = this->globalTransformMatrix;
        }
        else
        {
            _transformDirty = BoneTransformDirty::Self;
            this->globalTransformMatrix = &this->_globalTransformMatrix;
        }
    }
    else if (
        _transformDirty == BoneTransformDirty::All ||
        (this->_parent && this->_parent->_transformDirty != BoneTransformDirty::None) ||
        (_ik && _ik->_transformDirty != BoneTransformDirty::None)
        )
    {
        _transformDirty = BoneTransformDirty::All;
        this->globalTransformMatrix = &this->_globalTransformMatrix;
    }

    if (_transformDirty != BoneTransformDirty::None)
    {
        if (_transformDirty == BoneTransformDirty::All)
        {
            _transformDirty = BoneTransformDirty::Self;
        }
        else
        {
            _transformDirty = BoneTransformDirty::None;
        }

        if (this->globalTransformMatrix == &this->_globalTransformMatrix)
        {
            this->global = this->origin; // copy
            this->global.add(this->offset).add(_animationPose);

            _updateGlobalTransformMatrix();

            if (_ik && ikWeight)
            {
                _computeIK();
            }

            if (cacheFrameIndex >= 0)
            {
                this->globalTransformMatrix = BoneTimelineData::cacheFrame(*_cacheFrames, cacheFrameIndex, *this->globalTransformMatrix);
            }
        }
    }
}

bool Bone::contains(const TransformObject* child) const
{
    if (child)
    {
        if (child == this)
        {
            return false;
        }

        auto ancestor = child;
        while (ancestor != this && ancestor)
        {
            ancestor = ancestor->getParent();
        }

        return ancestor == this;
    }
    
    // throw new Error();
    return false;
}

void Bone::setVisible(bool value)
{
    if (_visible == value)
    {
        return;
    }

    _visible = value;

    for (const auto slot : this->_armature->getSlots())
    {
        if (slot->_parent == this)
        {
            slot->_updateVisible();
        }
    }
}

NAMESPACE_DRAGONBONES_END