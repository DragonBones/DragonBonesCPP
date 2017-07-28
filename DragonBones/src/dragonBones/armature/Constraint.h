//
// Created by liangshuochen on 12/06/2017.
//

#ifndef DRAGONBONESCPP_CONSTRAINTS_H
#define DRAGONBONESCPP_CONSTRAINTS_H

#include "../core/BaseObject.h"
#include "../geom/Matrix.h"
#include "../geom/Transform.h"
#include "../geom/Point.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* @private
*/
class Constraint : public BaseObject
{
    ABSTRACT_CLASS(Constraint);

protected:
    static Matrix _helpMatrix;
    static Transform _helpTransform;
    static Point _helpPoint;

public:
    Bone* target;
    Bone* bone;
    Bone* root;

    virtual void update() = 0;

	// For WebAssembly.
    Bone* getTarget() const { return  target; }
    Bone* getBone() const { return bone; }
    Bone* getRoot() const { return root; }

    void setTarget(Bone* value ) { target = value; }
    void setBone(Bone* value ) { bone = value; }
    void setRoot(Bone* value ) { root = value; }

protected:
    virtual void _onClear() override;
};
/**
* @private
*/
class IKConstraint : public Constraint
{
    BIND_CLASS_TYPE_A(IKConstraint);

public:
    bool bendPositive;
    bool scaleEnabled; // TODO support
    float weight;

    virtual void update() override;

protected:
    virtual void _onClear() override;

private:
    void _computeA();
    void _computeB();
};

DRAGONBONES_NAMESPACE_END
#endif //DRAGONBONESCPP_CONSTRAINTS_H
