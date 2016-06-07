#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    addChild(rootNode);

    // DragonBones
    _factory.loadDragonBonesData("Ubbie/Ubbie.json");
    _factory.loadTextureAtlasData("Ubbie/texture.json");

    const auto armature = _factory.buildArmature("ubbie");
    const auto armatureDisplay = static_cast<dragonBones::CCArmatureDisplayContainer*>(armature->getDisplay());

    armature->getAnimation().play("turn face", 0);
    armatureDisplay->setPosition(480.f, 320.f);
    armatureDisplay->setScale(0.3f);

    dragonBones::WorldClock::clock.add(armature);
    this->addChild(armatureDisplay);

    cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(HelloWorld::_updateHandler), this, 0.f, false);

    return true;
}

void HelloWorld::_updateHandler(float passTime)
{
    dragonBones::WorldClock::clock.advanceTime(passTime);
}
