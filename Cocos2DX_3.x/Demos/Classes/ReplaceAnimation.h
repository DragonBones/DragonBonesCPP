#include "BaseDemo.h"

class ReplaceAnimation : BaseDemo
{
public:
    CREATE_FUNC(ReplaceAnimation);

    static cocos2d::Scene* createScene()
    {
        auto scene = cocos2d::Scene::create();
        auto layer = ReplaceAnimation::create();

        scene->addChild(layer);
        return scene;
    }

private:
    dragonBones::CCArmatureDisplay* _armatureDisplayA;
    dragonBones::CCArmatureDisplay* _armatureDisplayB;
    dragonBones::CCArmatureDisplay* _armatureDisplayC;
    dragonBones::CCArmatureDisplay* _armatureDisplayD;

protected:
    virtual void _onStart()
    {
        const auto factory = dragonBones::CCFactory::getFactory();
        factory->loadDragonBonesData("mecha_2903/mecha_2903_ske.json");
        factory->loadTextureAtlasData("mecha_2903/mecha_2903_tex.json");

        _armatureDisplayA = factory->buildArmatureDisplay("mecha_2903");
        _armatureDisplayB = factory->buildArmatureDisplay("mecha_2903b");
        _armatureDisplayC = factory->buildArmatureDisplay("mecha_2903c");
        _armatureDisplayD = factory->buildArmatureDisplay("mecha_2903d");

        const auto sourceArmatureData = factory->getArmatureData("mecha_2903d");
        factory->replaceAnimation(_armatureDisplayA->getArmature(), sourceArmatureData);
        factory->replaceAnimation(_armatureDisplayB->getArmature(), sourceArmatureData);
        factory->replaceAnimation(_armatureDisplayC->getArmature(), sourceArmatureData);

        addChild(_armatureDisplayD);
        addChild(_armatureDisplayA);
        addChild(_armatureDisplayB);
        addChild(_armatureDisplayC);

        _armatureDisplayA->setPosition(0.0f - 350.0f, 0.0f - 150.0f);
        _armatureDisplayB->setPosition(0.0, 0.0f - 150.0f);
        _armatureDisplayC->setPosition(0.0f + 350.0f, 0.0f - 150.0f);
        _armatureDisplayD->setPosition(0.0f, 0.0f + 50.0f);
        //
        const auto listener = cocos2d::EventListenerMouse::create();
        listener->onMouseDown = CC_CALLBACK_1(ReplaceAnimation::_mouseDownHandler, this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        //
        createText("Touch to change animation.");
    }

private:
    void _mouseDownHandler(cocos2d::EventMouse* event)
    {
        _changeAnimation();
    }

    void _changeAnimation() 
    {
        auto animationName = _armatureDisplayD->getAnimation()->getLastAnimationName();
        if (!animationName.empty()) 
        {
            const auto& animationNames = _armatureDisplayD->getAnimation()->getAnimationNames();
            const auto animationIndex = (dragonBones::indexOf(animationNames, animationName) + 1) % animationNames.size();
            _armatureDisplayD->getAnimation()->play(animationNames[animationIndex]);
        }
        else 
        {
            _armatureDisplayD->getAnimation()->play();
        }

        animationName = _armatureDisplayD->getAnimation()->getLastAnimationName();

        _armatureDisplayA->getAnimation()->play(animationName);
        _armatureDisplayB->getAnimation()->play(animationName);
        _armatureDisplayC->getAnimation()->play(animationName);
    }
};