#ifndef HELLO_DRAGONBONES_H
#define HELLO_DRAGONBONES_H

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
/**
 * How to use
 * 1. Load data.
 *    factory.loadDragonBonesData();
 *    factory.loadTextureAtlasData();
 *
 * 2. Build armature.
 *    armatureDisplay = factory.buildArmatureDisplay("armatureName");
 *
 * 3. Play animation.
 *    armatureDisplay->getAnimation()->play("animationName");
 *
 * 4. Add armature to stage.
 *    addChild(armatureDisplay);
 */
class HelloDragonBones : public cocos2d::LayerColor
{
public:
    CREATE_FUNC(HelloDragonBones);
    static cocos2d::Scene* createScene()
    {
        auto scene = cocos2d::Scene::create();
        auto layer = HelloDragonBones::create();

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
        // factory->loadDragonBonesData("home_ske.json");
        factory->loadDragonBonesData("home_ske.dbbin");
        factory->loadTextureAtlasData("home_tex.json");
        const auto armatureDisplay = factory->buildArmatureDisplay("home");
        armatureDisplay->getAnimation()->play("0-1");

        addChild(armatureDisplay);
        armatureDisplay->setPosition(stageSize.width * 0.5f, stageSize.height * 0.5f);
        armatureDisplay->setScale(0.3f);

        return true;
    }
};

#endif // HELLO_DRAGONBONES_H
