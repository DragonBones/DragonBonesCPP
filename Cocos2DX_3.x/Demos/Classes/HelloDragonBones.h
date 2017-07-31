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

        const auto factory = dragonBones::CCFactory::getFactory();
        // factory->loadDragonBonesData("dragon_boy_ske.json");
        factory->loadDragonBonesData("dragon_boy_ske.dbbin");
        factory->loadTextureAtlasData("dragon_boy_tex.json");
        const auto armatureDisplay = factory->buildArmatureDisplay("DragonBoy");
        armatureDisplay->getArmature()->setCacheFrameRate(24);
        armatureDisplay->getAnimation()->play("walk");

        addChild(armatureDisplay);

        const auto stageSize = cocos2d::Director::getInstance()->getVisibleSize();
        armatureDisplay->setPosition(stageSize.width * 0.5f, stageSize.height * 0.5f);

        return true;
    }

    CREATE_FUNC(HelloDragonBones);
};

#endif // HELLO_DRAGONBONES_H
