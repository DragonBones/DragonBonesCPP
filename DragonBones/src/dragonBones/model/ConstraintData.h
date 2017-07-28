//
// Created by liangshuochen on 09/06/2017.
//

#ifndef DRAGONBONESCPP_CONSTRAINTDATA_H
#define DRAGONBONESCPP_CONSTRAINTDATA_H

#include "../core/BaseObject.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* @private
*/
class ConstraintData : public BaseObject 
{
    ABSTRACT_CLASS(ConstraintData);

public:
    int order;
    BoneData* target;
    BoneData* bone;
    BoneData* root;

protected:
    virtual void _onClear() override;

public: // For WebAssembly.
    BoneData* getTarget() const { return  target; }
    BoneData* getBone() const { return bone; }
    BoneData* getRoot() const { return root; }

    void setTarget(BoneData* value) { target = value; }
    void setBone(BoneData* value) { bone = value; }
    void setRoot(BoneData* value) { root = value; }
};
/**
* @private
*/
class IKConstraintData : public ConstraintData 
{
    BIND_CLASS_TYPE_A(IKConstraintData);

public:
    bool bendPositive;
    bool scaleEnabled;
    float weight;

protected:
    virtual void _onClear() override;
};

DRAGONBONES_NAMESPACE_END
#endif //DRAGONBONESCPP_CONSTRAINTDATA_H
