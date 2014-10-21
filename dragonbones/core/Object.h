#ifndef CORE_OBJECT_H
#define CORE_OBJECT_H

#include "../DragonBones.h"
#include "../geoms/Matrix.h"
#include "../geoms/Transform.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class Armature;
class Bone;

class Object
{
public:
    bool inheritRotation;
    bool inheritScale;
    
    std::string name;
    Transform global;
    Transform origin;
    Transform offset;
    Matrix globalTransformMatrix;
    
    void *userData;
    
protected:
    bool _visible;
    
    Armature *_armature;
    Bone *_parent;
    
public:
    virtual bool getVisible() const;
    virtual void setVisible(bool vislble);
    
    virtual Armature* getArmature() const;
    virtual Bone* getParent() const;
    
protected:
    virtual void setArmature(Armature *armature);
    virtual void setParent(Bone *bone);
    
public:
    Object();
    virtual ~Object();
    virtual void dispose();
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(Object);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // CORE_OBJECT_H
