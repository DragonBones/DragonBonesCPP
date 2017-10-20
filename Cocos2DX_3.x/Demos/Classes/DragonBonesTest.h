#ifndef DRAGONBONES_TEST_H
#define DRAGONBONES_TEST_H

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
#include "rapidjson/document.h"

class DragonBonesTest : public cocos2d::LayerColor
{
public:
    CREATE_FUNC(DragonBonesTest);
    static cocos2d::Scene* createScene()
    {
        auto scene = cocos2d::Scene::create();
        auto layer = DragonBonesTest::create();

        scene->addChild(layer);
        return scene;
    }

    static std::vector<std::string> splitString(std::string string, std::string delim)
    {
        std::vector<std::string> result;
        std::replace_if(
            string.begin(),
            string.end(),
            [&](const char& c)
        {
            return delim.find(c) != std::string::npos;
        },
            delim.at(0)
            );
        std::size_t pos = string.find(delim.at(0));
        std::string addedString = "";

        while (pos != std::string::npos)
        {
            addedString = string.substr(0, pos);
            result.push_back(addedString);
            string.erase(string.begin(), string.begin() + pos + 1);
            pos = string.find(delim.at(0));
        }

        addedString = string;
        result.push_back(addedString);

        return result;
    }

    virtual bool init()
    {
        if (!LayerColor::initWithColor(cocos2d::Color4B(105, 105, 105, 255)))
        {
            return false;
        }

        _isTouched = false;
        _isTouchMoved = false;
        _isHorizontalMoved = false;
        _prevArmatureScale = 1.0f;
        _prevAnimationScale = 1.0f;

        _dragonBonesIndex = 0;
        _armatureIndex = -1;
        _animationIndex = 0;

        const auto& fullpath = cocos2d::FileUtils::getInstance()->fullPathForFilename("base_test.res.json");
        auto jsonString = cocos2d::FileUtils::getInstance()->getStringFromFile(fullpath);

        rapidjson::Document document;
        document.Parse(jsonString.c_str());
        const auto& resources = document["resources"];
        const auto& groups = document["groups"];
        const auto factory = dragonBones::CCFactory::getFactory();

        std::map<std::string, std::size_t>resourceMap;
        for (std::size_t i = 0, l = resources.Size(); i < l; ++i)
        {
            const auto& resource = resources[i];
            resourceMap[resource["name"].GetString()] = i;
        }

        for (std::size_t i = 0, l = groups.Size(); i < l; ++i) 
        {
            const auto& group = groups[i];
            std::string groupName = group["name"].GetString();
            if (groupName == "search")
            {
                std::string groupKeysString = group["keys"].GetString();
                const auto& result = splitString(groupKeysString, ",");
                for (const auto& key : result)
                {
                    const auto& dragonBonesResource = resources[resourceMap[key]];
                    // const auto& dragonBonesResource = resources[resourceMap[key + "_binary"]];
                    factory->loadDragonBonesData(dragonBonesResource["url"].GetString());

                    unsigned index = 0;
                    while (true) 
                    {
                        std::string textureAtlasDataName = key + "_texture_config_" + dragonBones::to_string(index);
                        if (resourceMap.find(textureAtlasDataName) != resourceMap.cend())
                        {
                            const auto& textureAtlasResource = resources[resourceMap[textureAtlasDataName]];
                            factory->loadTextureAtlasData(textureAtlasResource["url"].GetString());
                        }
                        else 
                        {
                            break;
                        }

                        index++;
                    }

                    if (index == 0)
                    {
                        // TODO
                    }
                }
            }
        }

        factory->autoSearch = true;
        const auto& allDragonBonesData = factory->getAllDragonBonesData();
        for (const auto& pair : allDragonBonesData)
        {
            _allDragonBonesData.push_back(pair.second);
        }

        if (!_allDragonBonesData.empty())
        {
            // Add event listeners.
            const auto listener = cocos2d::EventListenerMouse::create();
            listener->onMouseDown = CC_CALLBACK_1(DragonBonesTest::_mouseDownHandler, this);
            listener->onMouseUp = CC_CALLBACK_1(DragonBonesTest::_mouseUpHandler, this);
            listener->onMouseMove = CC_CALLBACK_1(DragonBonesTest::_mouseMovedHandler, this);
            getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

            // Add infomation.
            _text = cocos2d::Label::create();
            _text->setPosition(cocos2d::Director::getInstance()->getVisibleSize().width * 0.5f, 60.f);
            _text->setAlignment(cocos2d::TextHAlignment::CENTER);
            addChild(_text);

            // Add sound event listener.
            factory->getSoundEventManager()->getEventDispatcher()->setEnabled(true);
            factory->getSoundEventManager()->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::SOUND_EVENT, std::bind(&DragonBonesTest::_animationEventHandler, this, std::placeholders::_1));

            // Add armature.
            _changeArmature(1);
            _changeAnimation();
        }

        return true;
    }

private:
    bool _isTouched;
    bool _isTouchMoved;
    bool _isHorizontalMoved;
    float _prevArmatureScale;
    float _prevAnimationScale;
    cocos2d::Vec2 _startPoint;
    cocos2d::Label* _text;

    int _dragonBonesIndex;
    int _armatureIndex;
    int _animationIndex;
    dragonBones::CCArmatureDisplay* _armatureDisplay;
    std::vector<dragonBones::DragonBonesData*> _allDragonBonesData;

    void _mouseDownHandler(cocos2d::EventMouse* event)
    {
        _isTouched = true;

        _prevArmatureScale = _armatureDisplay->getScale();
        _prevAnimationScale = _armatureDisplay->getAnimation()->timeScale;
        _startPoint.setPoint(event->getCursorX(), event->getCursorY());
    }

    void _mouseUpHandler(cocos2d::EventMouse* event)
    {
        if (_isTouched) 
        {
            _isTouched = false;

            if (_isTouchMoved) 
            {
                _isTouchMoved = false;
            }
            else 
            {
                if (_allDragonBonesData.size() > 1 || _allDragonBonesData[0]->armatureNames.size() > 1) 
                {
                    const auto& stageSize = cocos2d::Director::getInstance()->getVisibleSize();
                    const auto isSide = std::abs(stageSize.width / 2.0f - event->getCursorX()) > stageSize.width / 6.0f;
                    const auto touchRight = event->getCursorX() > stageSize.width / 2.0f;
                    if (isSide) 
                    {
                        if (touchRight) 
                        {
                            _changeArmature(1);
                        }
                        else 
                        {
                            _changeArmature(-1);
                        }
                    }
                }

                _changeAnimation();
            }
        }
    }

    void _mouseMovedHandler(cocos2d::EventMouse* event)
    {
        if (_isTouched)
        {
            const auto dX = _startPoint.x - event->getCursorX();
            const auto dY = _startPoint.y - event->getCursorY();

            if (!_isTouchMoved) 
            {
                const auto dAX = std::abs(dX);
                const auto dAY = std::abs(dY);

                if (dAX > 5.0f || dAY > 5.0f)
                {
                    _isTouchMoved = true;
                    _isHorizontalMoved = dAX > dAY;
                }
            }

            if (_isTouchMoved) 
            {
                if (_isHorizontalMoved) 
                {
                    const auto currentAnimationScale = std::max(-dX / 200.0f + _prevAnimationScale, 0.01f);
                    _armatureDisplay->getAnimation()->timeScale = currentAnimationScale;
                }
                else 
                {
                    const auto currentArmatureScale = std::max(dY / 200.0f + _prevArmatureScale, 0.01f);
                    _armatureDisplay->setScale(currentArmatureScale);
                }
            }
        }
    }
    /**
    * Animation listener.
    */
    void _animationEventHandler(cocos2d::EventCustom* event) const
    {
        const auto eventObject = (dragonBones::EventObject*)event->getUserData();
        cocos2d::log("%s %s %s", eventObject->animationState->name.c_str(), eventObject->type.c_str(), eventObject->name.c_str());
    }
    /**
    * Change armature.
    */
    void _changeArmature(int dir)
    {
        auto dragonBonesChange = false;
        const auto dragonBonesData = _allDragonBonesData[_dragonBonesIndex];
        const auto& armatureNames = dragonBonesData->armatureNames;

        if (armatureNames.empty()) 
        {
            CCLOG("No armature in data. %s", dragonBonesData->name);
        }

        if (_armatureIndex + dir < 0)
        {
            dragonBonesChange = true;
            _dragonBonesIndex--;
        }
        else if ((std::size_t)(_armatureIndex + dir) >= armatureNames.size()) 
        {
            dragonBonesChange = true;
            _dragonBonesIndex++;
        }

        if (dragonBonesChange) 
        {
            if (_dragonBonesIndex < 0) 
            {
                _dragonBonesIndex = _allDragonBonesData.size() - 1;
            }
            else if ((std::size_t)_dragonBonesIndex >= _allDragonBonesData.size())
            {
                _dragonBonesIndex = 0;
            }

            _armatureIndex = dir > 0 ? -1 : _allDragonBonesData[_dragonBonesIndex]->armatureNames.size();
            _changeArmature(dir);

            return;
        }

        _armatureIndex += dir > 0 ? 1 : -1;
        const auto& armatureName = armatureNames[_armatureIndex];

        // Remove prev armature.
        if (_armatureDisplay) 
        {
            //disableDrag(_armatureDisplay);
            removeChild(_armatureDisplay);

            // Remove listeners.
            _armatureDisplay->getEventDispatcher()->removeCustomEventListeners(dragonBones::EventObject::START);
            _armatureDisplay->getEventDispatcher()->removeCustomEventListeners(dragonBones::EventObject::LOOP_COMPLETE);
            _armatureDisplay->getEventDispatcher()->removeCustomEventListeners(dragonBones::EventObject::COMPLETE);
            _armatureDisplay->getEventDispatcher()->removeCustomEventListeners(dragonBones::EventObject::FRAME_EVENT);
            _armatureDisplay->dispose();
        }

        // Build armature display.
        _armatureDisplay = dragonBones::CCFactory::getFactory()->buildArmatureDisplay(armatureName, dragonBonesData->name);
        _armatureDisplay->getArmature()->setCacheFrameRate(24); // Cache animation.

        // Add listener.
        _armatureDisplay->getEventDispatcher()->setEnabled(true);
        _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::START, std::bind(&DragonBonesTest::_animationEventHandler, this, std::placeholders::_1));
        _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::LOOP_COMPLETE, std::bind(&DragonBonesTest::_animationEventHandler, this, std::placeholders::_1));
        _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::COMPLETE, std::bind(&DragonBonesTest::_animationEventHandler, this, std::placeholders::_1));
        _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FRAME_EVENT, std::bind(&DragonBonesTest::_animationEventHandler, this, std::placeholders::_1));

        // Add armature display.
        const auto& stageSize = cocos2d::Director::getInstance()->getVisibleSize();

        _armatureDisplay->setPosition(stageSize.width * 0.5f, stageSize.height * 0.5f - 100.0f);
        addChild(_armatureDisplay);
        //enableDrag(this._armatureDisplay);

        // Update boundingBox tester. TODO
        // this._boundingBoxTester.armatureDisplay = this._armatureDisplay;
        // this.addChild(this._boundingBoxTester);
    }
    /**
    * Change armature animation.
    */
    void _changeAnimation()
    {
        const auto& animationNames = _armatureDisplay->getAnimation()->getAnimationNames();
        if (animationNames.empty())
        {
            _text->setString(
                "DragonBones: " + _armatureDisplay->getArmature()->armatureData->parent->name +
                "    Armature: " + _armatureDisplay->getArmature()->getName() +
                "\nTouch screen left / right to change prev / next armature.\nTouch center to play next animation."
            );
            return;
        }

        // Get next animation name.
        _animationIndex++;
        if ((std::size_t)_animationIndex >= animationNames.size())
        {
            _animationIndex = 0;
        }

        const auto& animationName = animationNames[_animationIndex];

        // Play animation.
        _armatureDisplay->getAnimation()->play(animationName);

        // Infomation.
        _text->setString(
            "DragonBones: " + _armatureDisplay->getArmature()->armatureData->parent->name +
            "    Armature: " + _armatureDisplay->getArmature()->getName() +
            "    Animation: " + _armatureDisplay->getArmature()->getAnimation()->getLastAnimationName() +
            "\nTouch screen left/right to change prev/next armature.\nTouch center to play next animation."
        );
    }
};

#endif // DRAGONBONES_TEST_H
