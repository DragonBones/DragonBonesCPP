#include "BaseDemo.h"

class PerformanceTest : public BaseDemo
{
public:
    CREATE_FUNC(PerformanceTest);
    static cocos2d::Scene* createScene()
    {
        auto scene = cocos2d::Scene::create();
        auto layer = PerformanceTest::create();

        scene->addChild(layer);
        return scene;
    }

protected:
    bool _addingArmature;
    bool _removingArmature;
    std::vector<dragonBones::CCArmatureDisplay*> _armatures;
    cocos2d::Label* _text;

    virtual void _onStart()
    {
        _addingArmature = false;
        _removingArmature = false;
        _text = createText("");
        //
        const auto listener = cocos2d::EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(PerformanceTest::_touchBeganHandler, this);
        listener->onTouchEnded = CC_CALLBACK_2(PerformanceTest::_touchEndedHandler, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        //
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
            this, 0.0f, false, "enterFrameHandler"
        );

        for (std::size_t i = 0; i < 300; ++i)
        {
            _addArmature();
        }

        _resetPosition();
        _updateText();
    }

private:
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
            dragonBones::CCFactory::getFactory()->loadDragonBonesData("mecha_1406/mecha_1406_ske.dbbin");
            dragonBones::CCFactory::getFactory()->loadTextureAtlasData("mecha_1406/mecha_1406_tex.json");
        }

        const auto armatureDisplay = dragonBones::CCFactory::getFactory()->buildArmatureDisplay("mecha_1406");
        armatureDisplay->setScale(0.5f);
        armatureDisplay->getArmature()->setCacheFrameRate(24);
        armatureDisplay->getAnimation()->play("walk");
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
            dragonBones::CCFactory::getFactory()->clear();
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

        const auto paddingH = 100;
        const auto paddingT = 200;
        const auto paddingB = 100;
        const auto gapping = 90;

        const auto stageWidth = stageSize.width - paddingH * 2;
        const auto columnCount = (unsigned)(stageWidth / gapping);
        const auto paddingHModify = (stageSize.width - columnCount * gapping) * 0.5f;
        const auto dX = stageWidth / columnCount;
        const auto dY = (stageSize.height - paddingT - paddingB) / std::ceil(armatureCount / columnCount);

        for (std::size_t i = 0, l = _armatures.size(); i < l; ++i)
        {
            const auto armatureDisplay = _armatures[i];
            const auto lineY = (unsigned)std::floor(i / columnCount);

            armatureDisplay->setPosition(
                (i % columnCount) * dX + paddingHModify - stageSize.width * 0.5f,
                stageSize.height * 0.5f - (lineY * dY + paddingT)
            );
        }
    }

    void _updateText()
    {
        _text->setString("Count: " + dragonBones::to_string(_armatures.size()) + " \nTouch screen left to decrease count / right to increase count.");
    }
};
