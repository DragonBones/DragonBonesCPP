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

    dragonBones::CCFactory::factory.loadDragonBonesData("assets/dragon_boy_ske.json");
    dragonBones::CCFactory::factory.loadTextureAtlasData("assets/dragon_boy_tex.json");
    const auto armatureDisplay = dragonBones::CCFactory::factory.buildArmatureDisplay("DragonBoy");
    armatureDisplay->getAnimation()->play("walk");

    addChild(armatureDisplay);

    const auto center = cocos2d::Director::getInstance()->getVisibleSize();
    armatureDisplay->setPosition(center.width * 0.5f, center.height * 0.5f);

    return true;
}
