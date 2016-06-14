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

    // Load DragonBones Data
    const auto dragonBonesData = _factory.loadDragonBonesData("Ubbie/Ubbie.json");
    _factory.loadTextureAtlasData("Ubbie/texture.json");

    // a. Build Armature Display. (buildArmatureDisplay will advanceTime animation by Armature Display)
    _armatureDisplay = _factory.buildArmatureDisplay(dragonBonesData->getArmatureNames()[0]);
    _armature = _armatureDisplay->getArmature();

    // b. Build Armature. (buildArmature will advanceTime animation by WorldClock)
    /*_armature = _factory.buildArmature(dragonBonesData->getArmatureNames()[0]);
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
    _armatureDisplay->setScale(0.5f);
    this->addChild(_armatureDisplay);

    // Play animation.
    _armatureDisplay->getAnimation().play(_armatureDisplay->getAnimation().getAnimationNames()[0], 0);
    //_armature->getAnimation().play(_armature->getAnimation().getAnimationNames()[0], 0);

    const auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloDragonBones::_touchHandler, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

// Touch to change Armature animation.
bool HelloDragonBones::_touchHandler(const cocos2d::Touch* touch, cocos2d::Event* event) const
{
    const auto& animationNames = _armature->getAnimation().getAnimationNames();
    const auto iterator = std::find(animationNames.cbegin(), animationNames.cend(), _armature->getAnimation().getLastAnimationName());
    auto animationIndex = std::distance(animationNames.cbegin(), iterator);
    animationIndex++;
    if (animationIndex >= animationNames.size())
    {
        animationIndex = 0;
    }

    const auto& animationName = animationNames[animationIndex];
    _armature->getAnimation().play(animationName, 0);
    
    return true;
}