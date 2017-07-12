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
    if ( !LayerColor::initWithColor(cocos2d::Color4B(105, 105, 105, 255)))
    {
        return false;
    }

    _isMoved = false;
    _prevArmatureScale = 1.f;
    _armatureScale = 1.f;

    _armatureIndex = 0;
    _animationIndex = 0;
    _armature = nullptr;
    _armatureDisplay = nullptr;

    // Load DragonBones Data.
    _dragonBonesData = _factory.loadDragonBonesData("FadeInCrash/16_ske.json");
    _factory.loadTextureAtlasData("FadeInCrash/16_tex.json");

    if (_dragonBonesData)
    {
        _armatureDisplay = _factory.buildArmatureDisplay("armatureName");
        _armature = _armatureDisplay->getArmature();
        
        const auto center = cocos2d::Director::getInstance()->getVisibleSize();
        _armatureDisplay->setPosition(center.width * 0.5f, center.height * 0.5f);
        _armatureDisplay->setScale(_armatureScale);
        this->addChild(_armatureDisplay);
        
        _armature->getAnimation().fadeIn("Z公主-定帧", 0, 1, 0, "gongzhu", dragonBones::AnimationFadeOutMode::SameGroup);
        _armature->getAnimation().fadeIn("Z男孩-定帧", 0, 1, 0, "nanhai", dragonBones::AnimationFadeOutMode::SameGroup);
        _armatureDisplay->addEvent(dragonBones::EventObject::COMPLETE, std::bind(&HelloDragonBones::_frameEventHandler, this, std::placeholders::_1));
//        _changeArmature();
//        _changeAnimation();

//        const auto listener = cocos2d::EventListenerTouchOneByOne::create();
//        listener->onTouchBegan = CC_CALLBACK_2(HelloDragonBones::_touchBeganHandler, this);
//        listener->onTouchEnded = CC_CALLBACK_2(HelloDragonBones::_touchEndedHandler, this);
//        listener->onTouchMoved = CC_CALLBACK_2(HelloDragonBones::_touchMovedHandler, this);
//        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }
    else
    {
        assert(false);
    }

    const auto text = cocos2d::Label::create();
    text->setPosition(cocos2d::Director::getInstance()->getVisibleSize().width * 0.5f, 60.f);
    text->setString("Touch screen left to change Armature / right to change Animation.");
    text->setAlignment(cocos2d::TextHAlignment::CENTER);
    this->addChild(text);

    return true;
}

void HelloDragonBones::_frameEventHandler(dragonBones::EventObject* event)
{
    auto eventType = event->type;
    auto animName = event->animationState->getName();
    
    if (strcmp(eventType.c_str(), dragonBones::EventObject::COMPLETE) == 0) {
        
        if (strcmp("Z公主-定帧", animName.c_str()) == 0) {
            _armature->getAnimation().fadeIn("动画-公主-定帧", 0, 1, 0, "gongzhu", dragonBones::AnimationFadeOutMode::SameGroup);
        } else if (strcmp("Z男孩-定帧", animName.c_str()) == 0) {
            
            _armature->getAnimation().fadeIn("动画-男孩-定帧", 0, 1, 0, "nanhai", dragonBones::AnimationFadeOutMode::SameGroup);
        }
    }
}

void HelloDragonBones::_changeArmature()
{
    const auto& armatureNames = _dragonBonesData->getArmatureNames();
    if (armatureNames.empty())
    {
        return;
    }

    // Remove prev Armature.
    if (_armature)
    {
        _armature->dispose();
        this->removeChild(_armatureDisplay);

        // b.
        // dragonBones::WorldClock::clock.remove(_armature);
    }

    // Get Next Armature name.
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
    _armatureDisplay = dynamic_cast<dragonBones::CCArmatureDisplay*>(_armature->getDisplay());
    dragonBones::WorldClock::clock.add(_armature);
    cocos2d::Director::getInstance()->getScheduler()->schedule(
        [&](float passedTime)
        {
            dragonBones::WorldClock::clock.advanceTime(passedTime);
        },
        this, 0.f, false, "clock"
    );*/

    // Add Armature Display.
    const auto center = cocos2d::Director::getInstance()->getVisibleSize();
    _armatureDisplay->setPosition(center.width * 0.5f, center.height * 0.5f);
    _armatureDisplay->setScale(_armatureScale);
    this->addChild(_armatureDisplay);

    _animationIndex = 0;
}

void HelloDragonBones::_changeAnimation()
{
    const auto& animationNames = _armatureDisplay->getAnimation().getAnimationNames();
    if (animationNames.empty())
    {
        return;
    }

    // Get next Animation name.
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

bool HelloDragonBones::_touchBeganHandler(const cocos2d::Touch* touch, cocos2d::Event* event)
{
    _prevArmatureScale = _armatureScale;
    _startPoint = touch->getLocation();
    
    return true;
}

void HelloDragonBones::_touchEndedHandler(const cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (_isMoved)
    {
        _isMoved = false;
    }
    else 
    {
        const auto touchRight = touch->getLocation().x > cocos2d::Director::getInstance()->getVisibleSize().width * 0.5f;

        if (_dragonBonesData->getArmatureNames().size() > 1 && !touchRight)
        {
            _changeArmature();
        }

        _changeAnimation();
    }
}

void HelloDragonBones::_touchMovedHandler(const cocos2d::Touch* touch, cocos2d::Event* event)
{
    _isMoved = true;
    _armatureScale = std::max((touch->getLocation().y - _startPoint.y) / 200.f + _prevArmatureScale, 0.1f);
    _armatureDisplay->setScale(_armatureScale);
}
