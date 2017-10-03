#include "PerformanceTest.h"

USING_NS_CC;

Scene* PerformanceTest::createScene()
{
    auto scene = Scene::create();
    auto layer = PerformanceTest::create();

    scene->addChild(layer);
    return scene;
}

bool PerformanceTest::init()
{
    if (!LayerColor::initWithColor(cocos2d::Color4B(105, 105, 105, 255)))
    {
        return false;
    }

    _text = cocos2d::Label::create();
    _text->setPosition(cocos2d::Director::getInstance()->getVisibleSize().width * 0.5f, 60.f);
    _text->setAlignment(cocos2d::TextHAlignment::CENTER);
    this->addChild(_text);

    _addingArmature = false;
    _removingArmature = false;

    _dragonBonesData = _factory.loadDragonBonesData("DragonBoy/DragonBoy.json");
    if (_dragonBonesData)
    {
        _factory.loadTextureAtlasData("DragonBoy/DragonBoy_texture_1.json");

        cocos2d::Director::getInstance()->getScheduler()->schedule(
            [&](float passedTime)
            {
                if (_addingArmature)
                {
                    _addArmature();
                    _addArmature();
                    _addArmature();
                    _addArmature();
                    _addArmature();
                    _addArmature();
                    _addArmature();
                    _addArmature();
                    _addArmature();
                    _addArmature();
                    _resetPosition();
                }

                if (_removingArmature)
                {
                    _removeArmature();
                    _removeArmature();
                    _removeArmature();
                    _removeArmature();
                    _removeArmature();
                    _removeArmature();
                    _removeArmature();
                    _removeArmature();
                    _removeArmature();
                    _removeArmature();
                    _resetPosition();
                }

                dragonBones::WorldClock::clock.advanceTime(passedTime);
            },
            this, 0.f, false, "clock"
        );

        const auto listener = cocos2d::EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(PerformanceTest::_touchBeganHandler, this);
        listener->onTouchEnded = CC_CALLBACK_2(PerformanceTest::_touchEndedHandler, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

        //
        for (std::size_t i = 0; i < 100; ++i) 
        {
            _addArmature();
        }

        _resetPosition();
    }
    else
    {
        assert(false);
    }

    return true;
}

void PerformanceTest::_addArmature()
{
    const auto armature = _factory.buildArmature(_dragonBonesData->getArmatureNames()[1]);
    const auto armatureDisplay = dynamic_cast<dragonBones::CCArmatureDisplay*>(armature->getDisplay());

    armatureDisplay->setScale(0.3f);
    this->addChild(armatureDisplay);

    armature->setCacheFrameRate(24);
    armature->getAnimation().play(armature->getAnimation().getAnimationNames()[0], 0);
    dragonBones::WorldClock::clock.add(armature);

    _armatures.push_back(armature);
    _updateText();
}

void PerformanceTest::_removeArmature()
{
    if (_armatures.empty())
    {
        return;
    }

    const auto armature = _armatures.back();
    const auto armatureDisplay = dynamic_cast<dragonBones::CCArmatureDisplay*>(armature->getDisplay());
    this->removeChild(armatureDisplay);
    dragonBones::WorldClock::clock.remove(armature);
    armature->dispose();

    _armatures.pop_back();
    _updateText();
}

void PerformanceTest::_resetPosition()
{
    const auto stageSize = cocos2d::Director::getInstance()->getVisibleSize();

    const auto count = _armatures.size();
    const auto paddingH = 50;
    const auto paddingV = 150;
    const auto columnNum = 10;
    const auto dX = (stageSize.width - paddingH * 2) / columnNum;
    const auto dY = (stageSize.height - paddingV * 2) / std::ceil((float)count / columnNum);

    for (std::size_t i = 0, l = _armatures.size(); i < l; ++i)
    {
        const auto armature = _armatures[i];
        const auto armatureDisplay = dynamic_cast<dragonBones::CCArmatureDisplay*>(armature->getDisplay());
        const auto lineY = (unsigned)std::floor(i / columnNum);

        armatureDisplay->setPosition(
            (i % columnNum) * dX + paddingH,
            stageSize.height - (lineY * dY + paddingV)
        );
    }
}

void PerformanceTest::_updateText()
{
    _text->setString("Count: " + dragonBones::to_string(_armatures.size()) + " \nTouch screen right to increase count / left to decrease count.");
}

bool PerformanceTest::_touchBeganHandler(const cocos2d::Touch* touch, cocos2d::Event* event)
{
    const auto touchRight = touch->getLocation().x > cocos2d::Director::getInstance()->getVisibleSize().width * 0.5f;
    _addingArmature = touchRight;
    _removingArmature = !touchRight;

    return true;
}

void PerformanceTest::_touchEndedHandler(const cocos2d::Touch* touch, cocos2d::Event* event)
{
    _addingArmature = false;
    _removingArmature = false;
}