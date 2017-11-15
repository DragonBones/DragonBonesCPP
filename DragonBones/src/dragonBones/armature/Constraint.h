//
// Created by liangshuochen on 12/06/2017.
//

#ifndef DRAGONBONESCPP_CONSTRAINTS_H
#define DRAGONBONESCPP_CONSTRAINTS_H

#include "../core/BaseObject.h"
#include "../geom/Matrix.h"
#include "../geom/Transform.h"
#include "../geom/Point.h"
#include "../model/ArmatureData.h"
#include "../model/ConstraintData.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* @private
*/
class Constraint : public BaseObject
{
    ABSTRACT_CLASS(Constraint)

protected:
    static Matrix _helpMatrix;
    static Transform _helpTransform;
    static Point _helpPoint;

public:
    ConstraintData* _constraintData;
    Bone* _target;
    Bone* _bone;

protected:
    Armature* _armature;
    Bone* _root;

    virtual void _onClear() override;

public:
    virtual void init(ConstraintData* constraintData, Armature* armature) = 0;
    virtual void update() = 0;
    virtual void invalidUpdate() = 0;

    inline const std::string& getName() 
    {
        return _constraintData->name;
    }

};
/**
* @private
*/
class IKConstraint : public Constraint
{
    BIND_CLASS_TYPE_A(IKConstraint);

public:
    bool _bendPositive;
    float _weight;
    
private:
    bool _scaleEnabled;

protected:
    virtual void _onClear() override;

private:
    void _computeA();
    void _computeB();

public:
    virtual void init(ConstraintData* constraintData, Armature* armature) override;
    virtual void update() override;
    virtual void invalidUpdate() override;
};

DRAGONBONES_NAMESPACE_END
#endif //DRAGONBONESCPP_CONSTRAINTS_H
