#ifndef __ANIMATION_BASE_TEST_H__
#define __ANIMATION_BASE_TEST_H__

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"

class AnimationBaseTest : public cocos2d::LayerColor
{
    CREATE_FUNC(AnimationBaseTest);

public:
    static cocos2d::Scene* createScene()
    {
        auto scene = cocos2d::Scene::create();
        auto layer = AnimationBaseTest::create();

        scene->addChild(layer);
        return scene;
    }

    virtual bool init()
    {
        if (!LayerColor::initWithColor(cocos2d::Color4B(105, 105, 105, 255)))
        {
            return false;
        }

        _isTouched = false;
        const auto stageSize = cocos2d::Director::getInstance()->getVisibleSize();

        const auto factory = dragonBones::CCFactory::getFactory();
        factory->loadDragonBonesData("animation_base_test_ske.json");
        factory->loadTextureAtlasData("animation_base_test_tex.json");

        _armatureDisplay = factory->buildArmatureDisplay("progressBar");
        _armatureDisplay->setPosition(stageSize.width * 0.5f, stageSize.height * 0.5);
        _armatureDisplay->setScale(1.0f);
        addChild(_armatureDisplay);

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

        // Test animation config.
        const auto animaitonConfig = _armatureDisplay->getAnimation()->getAnimationConfig();
        animaitonConfig->name = "test"; // Animation state name.
        animaitonConfig->animation = "idle"; // Animation name.

        // animaitonConfig->playTimes = 1; // Play one time.
        animaitonConfig->playTimes = 3; // Play several times.
        // animaitonConfig->playTimes = 0; // Loop play.

        // animaitonConfig->timeScale = 1.0f; // Play speed.
        // animaitonConfig->timeScale = -1.0f; // Reverse play.

        animaitonConfig->position = 1.0f; // Goto and play.
        // animaitonConfig->duration = 0.0f; // Goto and stop.
        animaitonConfig->duration = 3.0f; // Interval play.
        _armatureDisplay->getAnimation()->playConfig(animaitonConfig);

        // _armatureDisplay->getAnimation()->play("idle", 1);

        const auto listener = cocos2d::EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(AnimationBaseTest::_touchBeganHandler, this);
        listener->onTouchEnded = CC_CALLBACK_2(AnimationBaseTest::_touchEndedHandler, this);
        listener->onTouchMoved = CC_CALLBACK_2(AnimationBaseTest::_touchMovedHandler, this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

        const auto text = cocos2d::Label::create();
        text->setPosition(stageSize.width * 0.5f, 60.f);
        text->setString("Click to control animation play progress.");
        text->setAlignment(cocos2d::TextHAlignment::CENTER);
        this->addChild(text);

        return true;
    }

private:
    bool _isTouched;
    dragonBones::CCArmatureDisplay* _armatureDisplay;

    bool _touchBeganHandler(const cocos2d::Touch* touch, cocos2d::Event* event)
    {
        _isTouched = true;

        const auto progress = std::min(std::max((touch->getLocation().x - _armatureDisplay->getPosition().x + 300.0f) / 600.0f, 0.0f), 1.0f);

        // _armatureDisplay->getAnimation()->gotoAndPlayByTime("idle", 0.5f, 1);
        // _armatureDisplay->getAnimation()->gotoAndStopByTime("idle", 1);

        // _armatureDisplay->getAnimation().gotoAndPlayByFrame("idle", 25, 2);
        // _armatureDisplay->getAnimation().gotoAndStopByFrame("idle", 50);

        // _armatureDisplay->getAnimation()->gotoAndPlayByProgress("idle", progress, 3);
        _armatureDisplay->getAnimation()->gotoAndStopByProgress("idle", progress);

        return true;
    }

    void _touchEndedHandler(const cocos2d::Touch* touch, cocos2d::Event* event)
    {
        _isTouched = false;
        _armatureDisplay->getAnimation()->play();
    }

    void _touchMovedHandler(const cocos2d::Touch* touch, cocos2d::Event* event)
    {
        if (_isTouched)
        {
            const auto progress = std::min(std::max((touch->getLocation().x - _armatureDisplay->getPosition().x + 300.0f) / 600.0f, 0.0f), 1.0f);
            const auto animationState = _armatureDisplay->getAnimation()->getState("idle");
            animationState->setCurrentTime(animationState->getTotalTime() * progress);
        }
    }

    void _animationEventHandler(cocos2d::EventCustom* event) const
    {
        const auto eventObject = (dragonBones::EventObject*)event->getUserData();

        cocos2d::log("%s %s %s", eventObject->animationState->name.c_str(), eventObject->type.c_str(), eventObject->name.c_str());
    }
};

#endif // __ANIMATION_BASE_TEST_H__
