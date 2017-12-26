#ifndef BASE_DEMO_H
#define BASE_DEMO_H

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
class BaseDemo : public cocos2d::LayerColor
{
protected:
    cocos2d::Sprite* _background;
    virtual void _onStart() = 0;

public:
    BaseDemo():
        _background(nullptr)
    {
    }

    virtual bool init()
    {
        if (!LayerColor::initWithColor(cocos2d::Color4B(105, 105, 105, 255)))
        {
            return false;
        }

        const auto& stageSize = cocos2d::Director::getInstance()->getVisibleSize();
        setPosition(stageSize.width * 0.5f, stageSize.height * 0.5f);

        _background = cocos2d::Sprite::create("background.png");
        const auto& textureSize = _background->getContentSize();
        addChild(_background);

        _onStart();

        return true;
    }

    cocos2d::Label* createText(const std::string& string)
    {
        const auto text = cocos2d::Label::create();
        text->setPosition(0.0f, -(getStageHeight() * 0.5f - 100.f));
        text->setString(string);
        text->setAlignment(cocos2d::TextHAlignment::CENTER);
        addChild(text);

        return text;
    }

    float getStageWidth() const
    {
        const auto& stageSize = cocos2d::Director::getInstance()->getVisibleSize();

        return stageSize.width;
    }

    float getStageHeight() const
    {
        const auto& stageSize = cocos2d::Director::getInstance()->getVisibleSize();

        return stageSize.height;
    }
};

#endif // BASE_DEMO_H
