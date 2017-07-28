#ifndef HELLO_DRAGONBONES_H
#define HELLO_DRAGONBONES_H

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"

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

        dragonBones::CCFactory::factory.loadDragonBonesData("dragon_boy_ske.json");
        dragonBones::CCFactory::factory.loadTextureAtlasData("dragon_boy_tex.json");
        const auto armatureDisplay = dragonBones::CCFactory::factory.buildArmatureDisplay("DragonBoy");
        armatureDisplay->getArmature()->setCacheFrameRate(24);
        armatureDisplay->getAnimation()->play("walk");

        addChild(armatureDisplay);


        const auto center = cocos2d::Director::getInstance()->getVisibleSize();
        armatureDisplay->setPosition(center.width * 0.5f, center.height * 0.5f);

        return true;
    }

    CREATE_FUNC(HelloDragonBones);
};

#endif // HELLO_DRAGONBONES_H
