#ifndef __PERFORMANCE_TEST_H__
#define __PERFORMANCE_TEST_H__

#include "cocos2d.h"
#include "cocos2dx/CCDragonBonesHeaders.h"

class PerformanceTest : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(PerformanceTest);

private:
    bool _addingArmature;
    bool _removingArmature;
    dragonBones::DragonBonesData* _dragonBonesData;
    dragonBones::Armature* _armature;
    dragonBones::CCArmatureDisplayContainer* _armatureDisplay;
    dragonBones::CCFactory _factory;
    std::vector<dragonBones::Armature*> _armatures;

    cocos2d::Label* _text;

    bool _touchBeganHandler(const cocos2d::Touch* touch, cocos2d::Event* event);
    void _touchEndedHandler(const cocos2d::Touch* touch, cocos2d::Event* event);
    void _addArmature();
    void _removeArmature();
    void _resetPosition();
    void _updateText();
};

#endif // __PERFORMANCE_TEST_H__
