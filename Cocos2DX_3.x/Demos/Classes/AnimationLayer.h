#include "BaseDemo.h"

class AnimationLayer : BaseDemo
{
public:
    CREATE_FUNC(AnimationLayer);

    static cocos2d::Scene* createScene()
    {
        auto scene = cocos2d::Scene::create();
        auto layer = AnimationLayer::create();

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
        _armatureDisplay = factory->buildArmatureDisplay("mecha_1004d");
        _armatureDisplay->getEventDispatcher()->setEnabled(true);
        _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::LOOP_COMPLETE, std::bind(&AnimationLayer::_animationEventHandler, this, std::placeholders::_1));
        _armatureDisplay->getAnimation()->play("walk");
        _armatureDisplay->setPosition(0.0f, -100.0f);
        addChild(_armatureDisplay);
    }

private:
    void _animationEventHandler(cocos2d::EventCustom* event) const
    {
        auto attackState = _armatureDisplay->getAnimation()->getState("attack_01");
        if (!attackState) 
        {
            attackState = _armatureDisplay->getAnimation()->fadeIn("attack_01", 0.1f, 1, 1);
            attackState->resetToPose = false;
            attackState->autoFadeOutTime = 0.1f;
            attackState->addBoneMask("chest");
            attackState->addBoneMask("effect_l");
            attackState->addBoneMask("effect_r");
        }
    }
};