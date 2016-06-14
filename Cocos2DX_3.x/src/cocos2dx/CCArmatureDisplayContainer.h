#ifndef DRAGONBONES_CC_ARMATURE_DISPLAY_CONTAINER_H
#define DRAGONBONES_CC_ARMATURE_DISPLAY_CONTAINER_H

#include "DragonBonesHeaders.h"
#include "cocos2d.h"

DRAGONBONES_NAMESPACE_BEGIN

class CCArmatureDisplayContainer : public cocos2d::Node, public IArmatureDisplayContainer
{
public:
    /** @private */
    static CCArmatureDisplayContainer* create();

public:
    /** @private */
    Armature* _armature;

protected:
    cocos2d::EventDispatcher _dispatcher;

protected:
    CCArmatureDisplayContainer();
    virtual ~CCArmatureDisplayContainer();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(CCArmatureDisplayContainer);

public:
    /** @private */
    virtual void _onClear() override;
    /** @private */
    virtual void _dispatchEvent(EventObject* value) override;
    /** @private */
    virtual void update(float passedTime) override;

public:
    virtual void advanceTimeBySelf(bool on) override;

    inline virtual bool hasEvent(const std::string& type) const override
    {
        return _dispatcher.isEnabled();
    }

    inline virtual Armature* getArmature() const override 
    {
        return _armature;
    }

    virtual Animation& getAnimation() const override 
    {
        return _armature->getAnimation();
    }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_CC_ARMATURE_DISPLAY_CONTAINER_H
