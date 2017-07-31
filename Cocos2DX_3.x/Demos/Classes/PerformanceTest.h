#ifndef __PERFORMANCE_TEST_H__
#define __PERFORMANCE_TEST_H__

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"

class PerformanceTest : public cocos2d::LayerColor
{
    CREATE_FUNC(PerformanceTest);

public:
    static cocos2d::Scene* createScene()
    {
        auto scene = cocos2d::Scene::create();
        auto layer = PerformanceTest::create();

        scene->addChild(layer);
        return scene;
    }

    virtual bool init()
    {
        if (!LayerColor::initWithColor(cocos2d::Color4B(105, 105, 105, 255)))
        {
            return false;
        }

        _addingArmature = false;
        _removingArmature = false;

        _text = cocos2d::Label::create();
        _text->setPosition(cocos2d::Director::getInstance()->getVisibleSize().width * 0.5f, 60.f);
        _text->setAlignment(cocos2d::TextHAlignment::CENTER);
        addChild(_text);

        const auto listener = cocos2d::EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(PerformanceTest::_touchBeganHandler, this);
        listener->onTouchEnded = CC_CALLBACK_2(PerformanceTest::_touchEndedHandler, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

        cocos2d::Director::getInstance()->getScheduler()->schedule(
            [&](float passedTime)
            {
                if (_addingArmature)
                {
                    for (std::size_t i = 0; i < 10; ++i) 
                    {
                        _addArmature();
                    }

                    _resetPosition();
                    _updateText();
                }

                if (_removingArmature)
                {
                    for (std::size_t i = 0; i < 10; ++i)
                    {
                        _removeArmature();
                    }

                    _resetPosition();
                    _updateText();
                }
            },
            this, 0.f, false, "enterFrameHandler"
        );

        for (std::size_t i = 0; i < 100; ++i)
        {
            _addArmature();
        }

        _resetPosition();
        _updateText();

        return true;
    }

private:
    bool _addingArmature;
    bool _removingArmature;
    std::vector<dragonBones::CCArmatureDisplay*> _armatures;
    cocos2d::Label* _text;

    bool _touchBeganHandler(const cocos2d::Touch* touch, cocos2d::Event* event)
    {
        const auto touchRight = touch->getLocation().x > cocos2d::Director::getInstance()->getVisibleSize().width * 0.5f;
        _addingArmature = touchRight;
        _removingArmature = !touchRight;

        return true;
    }

    void _touchEndedHandler(const cocos2d::Touch* touch, cocos2d::Event* event)
    {
        _addingArmature = false;
        _removingArmature = false;
    }

    void _addArmature()
    {
        if (_armatures.empty())
        {
            dragonBones::CCFactory::getFactory()->loadDragonBonesData("dragon_boy_ske.json");
            dragonBones::CCFactory::getFactory()->loadTextureAtlasData("dragon_boy_tex.json");
        }

        const auto armatureDisplay = dragonBones::CCFactory::getFactory()->buildArmatureDisplay("DragonBoy");
        armatureDisplay->setScale(0.7f);
        armatureDisplay->getArmature()->setCacheFrameRate(24);
        armatureDisplay->getAnimation()->play("walk", 0);
        addChild(armatureDisplay);

        _armatures.push_back(armatureDisplay);
    }

    void _removeArmature()
    {
        if (_armatures.empty())
        {
            return;
        }

        const auto armatureDisplay = _armatures.back();
        _armatures.pop_back();

        this->removeChild(armatureDisplay);
        armatureDisplay->dispose();

        if (_armatures.empty())
        {
            dragonBones::CCFactory::getFactory()->clear(true);
            dragonBones::BaseObject::clearPool();
        }
    }

    void _resetPosition()
    {
        const auto armatureCount = _armatures.size();
        if (armatureCount == 0) 
        {
            return;
        }

        const auto& stageSize = cocos2d::Director::getInstance()->getVisibleSize();

        const auto paddingH = 50;
        const auto paddingV = 150;
        const auto gapping = 100;

        const auto stageWidth = stageSize.width - paddingH * 2;
        const auto columnCount = (unsigned)(stageWidth / gapping);
        const auto paddingHModify = (stageSize.width - columnCount * gapping) * 0.5f;
        const auto dX = stageWidth / columnCount;
        const auto dY = (stageSize.height - paddingV * 2) / std::ceil(armatureCount / columnCount);

        for (std::size_t i = 0, l = _armatures.size(); i < l; ++i)
        {
            const auto armatureDisplay = _armatures[i];
            const auto lineY = (unsigned)std::floor(i / columnCount);

            armatureDisplay->setPosition(
                (i % columnCount) * dX + paddingHModify,
                stageSize.height - (lineY * dY + paddingV)
            );
        }
    }

    void _updateText()
    {
        _text->setString("Count: " + dragonBones::to_string(_armatures.size()) + " \nTouch screen left to decrease count / right to increase count.");
        addChild(_text);
    }
};

#endif // __PERFORMANCE_TEST_H__
