#ifndef REPLACE_SLOT_DISPLAY_H
#define REPLACE_SLOT_DISPLAY_H

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"

class ReplaceSlotDisplay : public cocos2d::LayerColor
{
public:
    CREATE_FUNC(ReplaceSlotDisplay);
    static cocos2d::Scene* createScene()
    {
        auto scene = cocos2d::Scene::create();
        auto layer = ReplaceSlotDisplay::create();

        scene->addChild(layer);
        return scene;
    }

    virtual bool init()
    {
        if (!LayerColor::initWithColor(cocos2d::Color4B(105, 105, 105, 255)))
        {
            return false;
        }

        _replaceDisplays = {
            // Replace normal display.
            "display0002", "display0003", "display0004", "display0005", "display0006", "display0007", "display0008", "display0009", "display0010",
            // Replace mesh display.
            "meshA", "meshB", "meshC"
        };

        _displayIndex = 0;
        const auto& stageSize = cocos2d::Director::getInstance()->getVisibleSize();

        _factory = dragonBones::CCFactory::getFactory();
        _factory->loadDragonBonesData("replace_slot_display/main_ske.json");
        _factory->loadTextureAtlasData("replace_slot_display/main_tex.json");
        _factory->loadDragonBonesData("replace_slot_display/replace_ske.json");
        _factory->loadTextureAtlasData("replace_slot_display/replace_tex.json");

        _armatureDisplay = _factory->buildArmatureDisplay("MyArmature");
        _armatureDisplay->getAnimation()->timeScale = 0.1f;
        _armatureDisplay->getAnimation()->play();
        _armatureDisplay->setPosition(
            stageSize.width * 0.5,
            stageSize.height * 0.5
        );
        addChild(_armatureDisplay);

        const auto listener = cocos2d::EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(ReplaceSlotDisplay::_touchBeganHandler, this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

        return true;
    }

private:
    unsigned _displayIndex;
    std::vector<std::string> _replaceDisplays;

    dragonBones::CCFactory* _factory;
    dragonBones::CCArmatureDisplay* _armatureDisplay;

    bool _touchBeganHandler(const cocos2d::Touch* touch, cocos2d::Event* event)
    {
        _replaceDisplay();

        return true;
    }

    void _replaceDisplay()
    {
        _displayIndex = fmod(_displayIndex + 1, _replaceDisplays.size());

        const auto replaceDisplayName = _replaceDisplays[_displayIndex];

        if (replaceDisplayName.find("mesh") != std::string::npos) // Replace mesh display.
        {
            if (replaceDisplayName == "meshA")
            {
                // Normal to mesh.
                _factory->replaceSlotDisplay(
                    "replace",
                    "MyMesh",
                    "meshA",
                    "weapon_1004_1",
                    _armatureDisplay->getArmature()->getSlot("weapon")
                );
            }
            else if (replaceDisplayName == "meshB")
            {
                // Normal to mesh.
                _factory->replaceSlotDisplay(
                    "replace",
                    "MyMesh",
                    "meshB",
                    "weapon_1004_1",
                    _armatureDisplay->getArmature()->getSlot("weapon")
                );

                // Replace mesh texture. 
                _factory->replaceSlotDisplay(
                    "replace",
                    "MyDisplay",
                    "ball",
                    "display0003",
                    _armatureDisplay->getArmature()->getSlot("mesh")
                );
            }
            else if (replaceDisplayName == "meshC")
            {
                // Back to normal.
                _factory->replaceSlotDisplay(
                    "replace",
                    "MyMesh",
                    "mesh",
                    "weapon_1004_1",
                    _armatureDisplay->getArmature()->getSlot("weapon")
                );

                // Replace mesh texture. 
                _factory->replaceSlotDisplay(
                    "replace",
                    "MyDisplay",
                    "ball",
                    "display0005",
                    _armatureDisplay->getArmature()->getSlot("mesh")
                );
            }
        }
        else 
        {
            // Replace normal display.
            _factory->replaceSlotDisplay(
                "replace",
                "MyDisplay",
                "ball",
                replaceDisplayName,
                _armatureDisplay->getArmature()->getSlot("ball")
            );
        }
    }
};

#endif // REPLACE_SLOT_DISPLAY_H
