#include "HelloDragonBones.h"

USING_NS_CC;

Scene* HelloDragonBones::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloDragonBones::create();

    scene->addChild(layer);
    return scene;
}

bool HelloDragonBones::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    _armatureIndex = 0;
    _animationIndex = 0;
    _armature = nullptr;
    _armatureDisplay = nullptr;

    // Load DragonBones Data.
    _dragonBonesData = _factory.loadDragonBonesData("AnimationBaseTest/AnimationBaseTest.json");
    _factory.loadTextureAtlasData("AnimationBaseTest/texture.json");

    if (_dragonBonesData)
    {
        _changeArmature();
        _changeAnimation();

        const auto listener = cocos2d::EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(HelloDragonBones::_touchHandler, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }
    else
    {
        assert(false);
    }

    return true;
}

void HelloDragonBones::_changeArmature()
{
    // Remove prev Armature.
    if (_armature)
    {
        _armature->dispose();
        this->removeChild(_armatureDisplay);

        // b.
        // dragonBones::WorldClock::clock.remove(_armature);
    }

    // Get Next Armature name.
    const auto& armatureNames = _dragonBonesData->getArmatureNames();
    _armatureIndex++;
    if (_armatureIndex >= armatureNames.size())
    {
        _armatureIndex = 0;
    }

    const auto& armatureName = armatureNames[_armatureIndex];

    // a. Build Armature Display. (buildArmatureDisplay will advanceTime animation by Armature Display)
    _armatureDisplay = _factory.buildArmatureDisplay(armatureName);
    _armature = _armatureDisplay->getArmature();

    // b. Build Armature. (buildArmature will advanceTime animation by WorldClock)
    /*_armature = _factory.buildArmature(armatureName);
    _armatureDisplay = dynamic_cast<dragonBones::CCArmatureDisplayContainer*>(_armature->getDisplay());
    dragonBones::WorldClock::clock.add(_armature);
    cocos2d::Director::getInstance()->getScheduler()->schedule(
        [&](float passedTime)
        {
            dragonBones::WorldClock::clock.advanceTime(passedTime);
        },
        this, 0.f, false, "clock"
    );*/

    // Add Armature Display.
    _armatureDisplay->setPosition(480.f, 200.f);
    _armatureDisplay->setScale(1.0f);
    this->addChild(_armatureDisplay);
}

void HelloDragonBones::_changeAnimation()
{
    // Get next Animation name.
    const auto& animationNames = _armatureDisplay->getAnimation().getAnimationNames();
    _animationIndex++;
    if (_animationIndex >= animationNames.size())
    {
        _animationIndex = 0;
    }

    const auto& animationName = animationNames[_animationIndex];

    // Play Animation.
    _armatureDisplay->getAnimation().play(animationName);
    //_armature->getAnimation().play(animationName);
}

bool HelloDragonBones::_touchHandler(const cocos2d::Touch* touch, cocos2d::Event* event)
{
    const auto touchRight = touch->getLocation().x > 960.f * 0.5f;

    if (_dragonBonesData->getArmatureNames().size() > 1 && !touchRight)
    {
        _changeArmature();
    }

    _changeAnimation();
    
    return true;
}