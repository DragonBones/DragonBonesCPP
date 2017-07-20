#include "Robo.h"

USING_NS_CC;

Scene* Robo::createScene()
{
    auto scene = Scene::create();
    auto layer = Robo::create();
    scene->addChild(layer);
    return scene;
}

bool Robo::init()
{
    if ( !LayerColor::initWithColor(cocos2d::Color4B(105, 105, 105, 255))) return false;
    _armature = nullptr;
    _armatureDisplay = nullptr;
    _dragonBonesData = _factory.loadDragonBonesData("Robo/robot_ske.json");
    _factory.loadTextureAtlasData("Robo/robot_tex.json");
    if (_dragonBonesData)
    {
        _changeArmature();
        _changeAnimation(0);
    }
    else
    {
        assert(false);
    }
    return true;
}

void Robo::_changeArmature()
{
    const auto& armatureNames = _dragonBonesData->getArmatureNames();
    if (armatureNames.empty()) return;
    if (_armature)
    {
        _armature->dispose();
        this->removeChild(_armatureDisplay);
    }
    const auto& armatureName = armatureNames[0];
    _armatureDisplay = _factory.buildArmatureDisplay(armatureName);
    _armature = _armatureDisplay->getArmature();
    const auto center = cocos2d::Director::getInstance()->getVisibleSize();
    _armatureDisplay->setPosition(center.width * 0.5f, center.height * 0.5f);
    this->addChild(_armatureDisplay);

}

void Robo::_changeAnimation(int i)
{
    const auto& animationNames = _armatureDisplay->getAnimation().getAnimationNames();
    if (animationNames.empty()) return;
    const auto& animationName = animationNames[i];
    _armatureDisplay->getAnimation().play(animationName);
}
