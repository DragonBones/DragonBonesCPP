//
// Created by liangshuochen on 12/06/2017.
//

#include "Constraint.h"
#include "Armature.h"
#include "Bone.h"

DRAGONBONES_NAMESPACE_BEGIN

Matrix Constraint::_helpMatrix;
Transform Constraint::_helpTransform;
Point Constraint::_helpPoint;

void Constraint::_onClear()
{
    _constraintData = nullptr;
    _armature = nullptr;
    _target = nullptr;
    _bone = nullptr;
    _root = nullptr;
}

void IKConstraint::_onClear()
{
    Constraint::_onClear();

    _scaleEnabled = false;
    _bendPositive = false;
    _weight = 1.0f;
}

void IKConstraint::_computeA()
{
    const auto& ikGlobal = _target->global;
    auto& global = _bone->global;
    auto& globalTransformMatrix = _bone->globalTransformMatrix;

    auto radian = std::atan2(ikGlobal.y - global.y, ikGlobal.x - global.x);
    if (global.scaleX < 0.0f)
    {
        radian += Transform::PI;
    }

    global.rotation += (radian - global.rotation) * _weight;
    global.toMatrix(globalTransformMatrix);
}

void IKConstraint::_computeB()
{
    const auto boneLength = _bone->_boneData->length;
    const auto parent = _root;
    const auto& ikGlobal = _target->global;
    auto& parentGlobal = parent->global;
    auto& global = _bone->global;
    auto& globalTransformMatrix = _bone->globalTransformMatrix;

    const auto x = globalTransformMatrix.a * boneLength;
    const auto y = globalTransformMatrix.b * boneLength;
    const auto lLL = x * x + y * y;
    const auto lL = sqrt(lLL);
    auto dX = global.x - parentGlobal.x;
    auto dY = global.y - parentGlobal.y;
    const auto lPP = dX * dX + dY * dY;
    const auto lP = sqrt(lPP);
    const auto rawRadian = global.rotation;
    const auto rawParentRadian = parentGlobal.rotation;
    const auto rawRadianA = std::atan2(dY, dX);

    dX = ikGlobal.x - parentGlobal.x;
    dY = ikGlobal.y - parentGlobal.y;
    const auto lTT = dX * dX + dY * dY;
    const auto lT = sqrt(lTT);

    auto radianA = 0.0f;
    if (lL + lP <= lT || lT + lL <= lP || lT + lP <= lL) 
    {
        radianA = std::atan2(ikGlobal.y - parentGlobal.y, ikGlobal.x - parentGlobal.x);
        if (lL + lP <= lT) 
        {
        }
        else if (lP < lL) 
        {
            radianA += Transform::PI;
        }
    }
    else 
    {
        const auto h = (lPP - lLL + lTT) / (2.0f * lTT);
        const auto r = sqrt(lPP - h * h * lTT) / lT;
        const auto hX = parentGlobal.x + (dX * h);
        const auto hY = parentGlobal.y + (dY * h);
        const auto rX = -dY * r;
        const auto rY = dX * r;

        bool isPPR = false;
        if (parent->getParent() != nullptr) 
        {
            auto parentParentMatrix = parent->getParent()->globalTransformMatrix;
            isPPR = parentParentMatrix.a * parentParentMatrix.d - parentParentMatrix.b * parentParentMatrix.c < 0.0f;
        }

        if (isPPR != _bendPositive) 
        {
            global.x = hX - rX;
            global.y = hY - rY;
        }
        else 
        {
            global.x = hX + rX;
            global.y = hY + rY;
        }

        radianA = std::atan2(global.y - parentGlobal.y, global.x - parentGlobal.x);
    }

    const auto dR = Transform::normalizeRadian(radianA - rawRadianA);
    parentGlobal.rotation = rawParentRadian + dR * _weight;
    parentGlobal.toMatrix(parent->globalTransformMatrix);
    //
    const auto currentRadianA = rawRadianA + dR * _weight;
    global.x = parentGlobal.x + cos(currentRadianA) * lP;
    global.y = parentGlobal.y + sin(currentRadianA) * lP;
    //
    auto radianB = std::atan2(ikGlobal.y - global.y, ikGlobal.x - global.x);
    if (global.scaleX < 0.0f) {
        radianB += Transform::PI;
    }

    global.rotation = parentGlobal.rotation + rawRadian - rawParentRadian + Transform::normalizeRadian(radianB - dR - rawRadian) * _weight;
    global.toMatrix(globalTransformMatrix);
}

void IKConstraint::init(ConstraintData* constraintData, Armature* armature)
{
    if (_constraintData != nullptr)
    {
        return;
    }

    _constraintData = constraintData;
    _armature = armature;
    _target = _armature->getBone(_constraintData->target->name);
    _bone = _armature->getBone(_constraintData->bone->name);
    _root = _constraintData->root != nullptr ? _armature->getBone(_constraintData->root->name) : nullptr;

    {
        const auto ikConstraintData = static_cast<IKConstraintData*>(_constraintData);
        _scaleEnabled = ikConstraintData->scaleEnabled;
        _bendPositive = ikConstraintData->bendPositive;
        _weight = ikConstraintData->weight;
    }

    _bone->_hasConstraint = true;
}

void IKConstraint::update()
{
    if (_root == nullptr)
    {
        _bone->updateByConstraint();
        _computeA();
    }
    else
    {
        _root->updateByConstraint();
        _bone->updateByConstraint();
        _computeB();
    }
}

void IKConstraint::invalidUpdate()
{
    if (_root == nullptr)
    {
        _bone->invalidUpdate();
    }
    else
    {
        _root->invalidUpdate();
        _bone->invalidUpdate();
    }
}

DRAGONBONES_NAMESPACE_END
