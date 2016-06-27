#include "AnimationBaseTest.h"

USING_NS_CC;

Scene* AnimationBaseTest::createScene()
{
    auto scene = Scene::create();
    auto layer = AnimationBaseTest::create();

    scene->addChild(layer);
    return scene;
}

bool AnimationBaseTest::init()
{
    if (!LayerColor::initWithColor(cocos2d::Color4B(105, 105, 105, 255)))
    {
        return false;
    }

    _isTouched = false;
    _armatureDisplay = nullptr;

    // Load DragonBones Data.
    const auto dragonBonesData = _factory.loadDragonBonesData("AnimationBaseTest/AnimationBaseTest.json");
    _factory.loadTextureAtlasData("AnimationBaseTest/texture.json");

    if (dragonBonesData)
    {
        const auto& armatureNames = dragonBonesData->getArmatureNames();
        const auto& armatureName = armatureNames[0];

        _armatureDisplay = _factory.buildArmatureDisplay(armatureName);

        _armatureDisplay->setPosition(480.f, 320.f);
        _armatureDisplay->setScale(1.0f);
        this->addChild(_armatureDisplay);

        const auto listener = cocos2d::EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(AnimationBaseTest::_touchBeganHandler, this);
        listener->onTouchEnded = CC_CALLBACK_2(AnimationBaseTest::_touchEndedHandler, this);
        listener->onTouchMoved = CC_CALLBACK_2(AnimationBaseTest::_touchMovedHandler, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

        // Enabled cocos2d Event
        _armatureDisplay->getEventDispatcher()->setEnabled(true);

        // Test animation event
        _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::START, std::bind(&AnimationBaseTest::_animationEventHandler, this, std::placeholders::_1));
        _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::LOOP_COMPLETE, std::bind(&AnimationBaseTest::_animationEventHandler, this, std::placeholders::_1));
        _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::COMPLETE, std::bind(&AnimationBaseTest::_animationEventHandler, this, std::placeholders::_1));
        _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FADE_IN, std::bind(&AnimationBaseTest::_animationEventHandler, this, std::placeholders::_1));
        _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FADE_IN_COMPLETE, std::bind(&AnimationBaseTest::_animationEventHandler, this, std::placeholders::_1));
        _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FADE_OUT, std::bind(&AnimationBaseTest::_animationEventHandler, this, std::placeholders::_1));
        _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FADE_OUT_COMPLETE, std::bind(&AnimationBaseTest::_animationEventHandler, this, std::placeholders::_1));

        // Test frame event
        _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FRAME_EVENT, std::bind(&AnimationBaseTest::_animationEventHandler, this, std::placeholders::_1));
    }
    else
    {
        assert(false);
    }

    return true;
}

bool AnimationBaseTest::_touchBeganHandler(const cocos2d::Touch* touch, cocos2d::Event* event)
{
    _isTouched = true;

    const auto progress = std::min(std::max((touch->getLocation().x - _armatureDisplay->getPosition().x + 300.f) / 600.f, 0.f), 1.f);

    _armatureDisplay->getAnimation().gotoAndPlayByTime("idle", 0.5, 1);
    _armatureDisplay->getAnimation().gotoAndStopByTime("idle", 1);

    //_armatureDisplay->getAnimation().gotoAndPlayByFrame("idle", 25, 2);
    //_armatureDisplay->getAnimation().gotoAndStopByFrame("idle", 50);

    _armatureDisplay->getAnimation().gotoAndPlayByProgress("idle", progress, 3);
    //_armatureDisplay->getAnimation().gotoAndStopByProgress("idle", progress);
    
    return true;
}

void AnimationBaseTest::_touchEndedHandler(const cocos2d::Touch* touch, cocos2d::Event* event)
{
    _isTouched = false;
}

void AnimationBaseTest::_touchMovedHandler(const cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (_isTouched && _armatureDisplay->getAnimation().getState("idle") && !_armatureDisplay->getAnimation().getState("idle")->getIsPlaying())
    {
        const auto progress = std::min(std::max((touch->getLocation().x - _armatureDisplay->getPosition().x + 300.f) / 600.f, 0.f), 1.f);

        _armatureDisplay->getAnimation().gotoAndStopByProgress("idle", progress);
    }
}

void AnimationBaseTest::_animationEventHandler(cocos2d::EventCustom* event) const
{
    const auto eventObject = (dragonBones::EventObject*)event->getUserData();

    cocos2d::log("%s %s %s", eventObject->animationState->getName().c_str(), eventObject->type.c_str(), eventObject->name.c_str());
}