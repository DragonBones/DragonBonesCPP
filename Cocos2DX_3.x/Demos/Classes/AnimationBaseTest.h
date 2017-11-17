#ifndef ANIMATION_BASE_TEST_H
#define ANIMATION_BASE_TEST_H

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"

class AnimationBaseTest : public cocos2d::LayerColor
{
public:
    CREATE_FUNC(AnimationBaseTest);
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

        const auto& stageSize = cocos2d::Director::getInstance()->getVisibleSize();

        const auto factory = dragonBones::CCFactory::getFactory();
        factory->loadDragonBonesData("animation_base_test_ske.json");
        factory->loadTextureAtlasData("animation_base_test_tex.json");

        _armatureDisplay = factory->buildArmatureDisplay("progressBar");
        _armatureDisplay->setPosition(stageSize.width * 0.5f, stageSize.height * 0.5f);
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
        _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FRAME_EVENT, std::bind(&AnimationBaseTest::_animationEventHandler, this, std::placeholders::_1));
        _armatureDisplay->getAnimation()->play("idle", 1);

        // Test animation config.
        // const auto animaitonConfig = _armatureDisplay->getAnimation()->getAnimationConfig();
        // animaitonConfig->name = "test"; // Animation state name.
        // animaitonConfig->animation = "idle"; // Animation name.

        // animaitonConfig->playTimes = 1; // Play one time.
        // animaitonConfig->playTimes = 3; // Play several times.
        // animaitonConfig->playTimes = 0; // Loop play.

        // animaitonConfig->timeScale = 1.0f; // Play speed.
        // animaitonConfig->timeScale = -1.0f; // Reverse play.

        // animaitonConfig->position = 1.0f; // Goto and play.
        // animaitonConfig->duration = 3.0f; // Interval play.
        // _armatureDisplay->getAnimation()->playConfig(animaitonConfig);

        const auto listener = cocos2d::EventListenerMouse::create();
        listener->onMouseDown = CC_CALLBACK_1(AnimationBaseTest::_mouseDownHandler, this);
        listener->onMouseUp = CC_CALLBACK_1(AnimationBaseTest::_mouseUpHandler, this);
        listener->onMouseMove = CC_CALLBACK_1(AnimationBaseTest::_mouseMovedHandler, this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

        const auto text = cocos2d::Label::create();
        text->setPosition(stageSize.width * 0.5f, 60.f);
        text->setString("Click to control animation play progress.");
        text->setAlignment(cocos2d::TextHAlignment::CENTER);
        this->addChild(text);

        return true;
    }

private:
    dragonBones::CCArmatureDisplay* _armatureDisplay;

    void _mouseDownHandler(cocos2d::EventMouse* event)
    {
        const auto progress = std::min(std::max((event->getCursorX() - _armatureDisplay->getPosition().x + 300.0f) / 600.0f, 0.0f), 1.0f);
        _armatureDisplay->getAnimation()->gotoAndStopByProgress("idle", progress);
    }

    void _mouseUpHandler(cocos2d::EventMouse* event)
    {
        _armatureDisplay->getAnimation()->play();
    }

    void _mouseMovedHandler(cocos2d::EventMouse* event)
    {
        if (event->getMouseButton() == 0)
        {
            const auto progress = std::min(std::max((event->getCursorX() - _armatureDisplay->getPosition().x + 300.0f) / 600.0f, 0.0f), 1.0f);
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

#endif // ANIMATION_BASE_TEST_H
