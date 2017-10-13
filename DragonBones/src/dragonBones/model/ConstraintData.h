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
    ABSTRACT_CLASS(ConstraintData)

public:
    int order;
    std::string name;
    const BoneData* target;
    const BoneData* bone;
    const BoneData* root;

protected:
    virtual void _onClear() override;

public: // For WebAssembly.
    const BoneData* getTarget() const { return  target; }
    void setTarget(const BoneData* value) { target = value; }

    const BoneData* getBone() const { return bone; }
    void setBone(const BoneData* value) { bone = value; }

    const BoneData* getRoot() const { return root; }
    void setRoot(const BoneData* value) { root = value; }
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
