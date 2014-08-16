#include "Object.h"
#include "Armature.h"

NAME_SPACE_DRAGON_BONES_BEGIN
bool Object::getVisible() const
{
    return _visible;
}
void Object::setVisible(bool visible)
{
    _visible = visible;
}

Armature* Object::getArmature() const
{
    return _armature;
}
Bone* Object::getParent() const
{
    return _parent;
}

void Object::setArmature(Armature *armature)
{
    if (_armature)
    {
        _armature->removeObject(this);
    }
    
    _armature = armature;
    
    if (_armature)
    {
        _armature->addObject(this);
    }
}
void Object::setParent(Bone *bone)
{
    _parent = bone;
}

Object::Object()
{
    _visible = true;
    _armature = nullptr;
    _parent = nullptr;
    offset.scaleX = offset.scaleY = 1.f;
    userData = nullptr;
}
Object::~Object()
{
    dispose();
}
void Object::dispose()
{
    _armature = nullptr;
    _parent = nullptr;
    
    if (userData)
    {
        delete userData;
        userData = nullptr;
    }
}
NAME_SPACE_DRAGON_BONES_END