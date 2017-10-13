//
// Created by liangshuochen on 12/06/2017.
//

#include "Constraint.h"
#include "../model/ArmatureData.h"
#include "Bone.h"

DRAGONBONES_NAMESPACE_BEGIN

Matrix Constraint::_helpMatrix;
Transform Constraint::_helpTransform;
Point Constraint::_helpPoint;

void Constraint::_onClear()
{
    target = nullptr;
    bone = nullptr;
    root = nullptr;
}

void IKConstraint::update()
{
    if (root == nullptr) 
    {
        bone->updateByConstraint();
        _computeA();
    }
    else 
    {
        root->updateByConstraint();
        bone->updateByConstraint();
        _computeB();
    }
}

void IKConstraint::_onClear()
{
    Constraint::_onClear();

    bendPositive = false;
    scaleEnabled = false;
    weight = 1.0f;
}

void IKConstraint::_computeA()
{
    const auto& ikGlobal = target->global;
    auto& global = bone->global;
    auto& globalTransformMatrix = bone->globalTransformMatrix;
    // const boneLength = bone.boneData.length; TODO
    // const x = globalTransformMatrix.a * boneLength;

    float ikRadian = atan2(ikGlobal.y - global.y, ikGlobal.x - global.x);
    if (global.scaleX < 0.0f)
    {
        ikRadian += Transform::PI;
    }

    global.rotation += (ikRadian - global.rotation) * weight;
    global.toMatrix(globalTransformMatrix);
}

void IKConstraint::_computeB()
{
    const auto boneLength = bone->boneData->length;
    const auto parent = root;
    const auto& ikGlobal = target->global;
    auto& parentGlobal = parent->global;
    auto& global = bone->global;
    auto& globalTransformMatrix = bone->globalTransformMatrix;

    const auto x = globalTransformMatrix.a * boneLength;
    const auto y = globalTransformMatrix.b * boneLength;

    const auto lLL = x * x + y * y;
    const auto lL = sqrt(lLL);

    float dX = global.x - parentGlobal.x;
    float dY = global.y - parentGlobal.y;
    const auto lPP = dX * dX + dY * dY;
    const auto lP = sqrt(lPP);
    const auto rawRadianA = atan2(dY, dX);

    dX = ikGlobal.x - parentGlobal.x;
    dY = ikGlobal.y - parentGlobal.y;
    const auto lTT = dX * dX + dY * dY;
    const auto lT = sqrt(lTT);

    auto ikRadianA = 0.0f;
    if (lL + lP <= lT || lT + lL <= lP || lT + lP <= lL) 
    {
        ikRadianA = atan2(ikGlobal.y - parentGlobal.y, ikGlobal.x - parentGlobal.x);
        if (lL + lP <= lT) 
        {
        }
        else if (lP < lL) 
        {
            ikRadianA += Transform::PI;
        }
    }
    else {
        const auto h = (lPP - lLL + lTT) / (2.0 * lTT);
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

        if (isPPR != bendPositive) 
        {
            global.x = hX - rX;
            global.y = hY - rY;
        }
        else 
        {
            global.x = hX + rX;
            global.y = hY + rY;
        }

        ikRadianA = atan2(global.y - parentGlobal.y, global.x - parentGlobal.x);
    }

    float dR = (ikRadianA - rawRadianA) * weight;
    parentGlobal.rotation += dR;
    parentGlobal.toMatrix(parent->globalTransformMatrix);

    const auto parentRadian = rawRadianA + dR;
    global.x = parentGlobal.x + cos(parentRadian) * lP;
    global.y = parentGlobal.y + sin(parentRadian) * lP;

    float ikRadianB = atan2(ikGlobal.y - global.y, ikGlobal.x - global.x);
    if (global.scaleX < 0.0f) {
        ikRadianB += Transform::PI;
    }

    dR = (ikRadianB - global.rotation) * weight;
    global.rotation += dR;
    global.toMatrix(globalTransformMatrix);
}

DRAGONBONES_NAMESPACE_END
