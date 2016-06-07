#ifndef DRAGONBONES_CC_ARMATURE_DISPLAY_CONTAINER_H
#define DRAGONBONES_CC_ARMATURE_DISPLAY_CONTAINER_H

#include "DragonBonesHeaders.h"
#include "cocos2d.h"

DRAGONBONES_NAMESPACE_BEGIN

class CCArmatureDisplayContainer : public cocos2d::Node, public IArmatureDisplayContainer
{
public:
    static CCArmatureDisplayContainer* create();

public:
    Armature* _armature;

public:
    CCArmatureDisplayContainer();
    virtual ~CCArmatureDisplayContainer();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(CCArmatureDisplayContainer);

public:
    virtual void _dispatchEvent(EventObject* value) {} // TODO

public:
    virtual bool hasEvent(const std::string& type) const override 
    {
        return false;  // TODO
    }

    virtual void addEvent(const std::string& type) override {};  // TODO
    virtual void removeEvent(const std::string& type) override {};  // TODO

    virtual void dispose() override {};  // TODO
    virtual void advanceTimeSelf(bool on) override {};  // TODO

    inline virtual Armature& getArmature() const override 
    {
        return *_armature;
    }

    virtual Animation& getAnimation() const override 
    {
        return _armature->getAnimation();
    }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_CC_ARMATURE_DISPLAY_CONTAINER_H
