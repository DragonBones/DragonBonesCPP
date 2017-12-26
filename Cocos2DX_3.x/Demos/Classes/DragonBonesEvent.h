#include "BaseDemo.h"

class DragonBonesEvent : BaseDemo
{
public:
    CREATE_FUNC(DragonBonesEvent);

    static cocos2d::Scene* createScene()
    {
        auto scene = cocos2d::Scene::create();
        auto layer = DragonBonesEvent::create();

        scene->addChild(layer);
        return scene;
    }

private:
    dragonBones::CCArmatureDisplay* _armatureDisplay;

protected:
    virtual void _onStart()
    {
        const auto factory = dragonBones::CCFactory::getFactory();
        factory->loadDragonBonesData("mecha_1004d/mecha_1004d_ske.json");
        factory->loadTextureAtlasData("mecha_1004d/mecha_1004d_tex.json");
        //
        factory->getSoundEventManager()->getEventDispatcher()->setEnabled(true);
        factory->getSoundEventManager()->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::SOUND_EVENT, std::bind(&DragonBonesEvent::_soundEventHandler, this, std::placeholders::_1));
        //
        _armatureDisplay = factory->buildArmatureDisplay("mecha_1004d");
        _armatureDisplay->getEventDispatcher()->setEnabled(true);
        _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::COMPLETE, std::bind(&DragonBonesEvent::_animationEventHandler, this, std::placeholders::_1));
        _armatureDisplay->getAnimation()->play("walk");
        _armatureDisplay->setPosition(0.0f, -100.0f);
        addChild(_armatureDisplay);
        //
        const auto listener = cocos2d::EventListenerMouse::create();
        listener->onMouseDown = CC_CALLBACK_1(DragonBonesEvent::_mouseDownHandler, this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        //
        createText("Touch to play animation.");
    }

private:
    void _mouseDownHandler(cocos2d::EventMouse* event)
    {
        _armatureDisplay->getAnimation()->fadeIn("skill_03", 0.2f);
    }

    void _soundEventHandler(cocos2d::EventCustom* event) const
    {
        const auto eventObject = (dragonBones::EventObject*)event->getUserData();
        cocos2d::log("%s", eventObject->name.c_str());
    }

    void _animationEventHandler(cocos2d::EventCustom* event) const
    {
        const auto eventObject = (dragonBones::EventObject*)event->getUserData();
        if (eventObject->animationState->name == "skill_03")
        {
            _armatureDisplay->getAnimation()->fadeIn("walk", 0.2f);
        }
    }
};