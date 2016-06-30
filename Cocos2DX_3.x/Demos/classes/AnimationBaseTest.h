#ifndef __ANIMATION_BASE_TEST_H__
#define __ANIMATION_BASE_TEST_H__

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"

class AnimationBaseTest : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(AnimationBaseTest);

private:
    bool _isTouched;
    dragonBones::CCArmatureDisplayContainer* _armatureDisplay;
    dragonBones::CCFactory _factory;

    bool _touchBeganHandler(const cocos2d::Touch* touch, cocos2d::Event* event);
    void _touchEndedHandler(const cocos2d::Touch* touch, cocos2d::Event* event);
    void _touchMovedHandler(const cocos2d::Touch* touch, cocos2d::Event* event);
    void _animationEventHandler(cocos2d::EventCustom* event) const;
};

#endif // __ANIMATION_BASE_TEST_H__
