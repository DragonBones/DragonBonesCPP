#include "BaseDemo.h"

class ReplaceSlotDisplay : BaseDemo
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

protected:
    unsigned _leftWeaponIndex;
    unsigned _rightWeaponIndex;
    std::vector<std::string> _weaponDisplayNames;
    dragonBones::CCFactory* _factory;
    dragonBones::CCArmatureDisplay* _armatureDisplay;
    cocos2d::Label* _logoText;

    virtual void _onStart()
    {
        _leftWeaponIndex = 0;
        _rightWeaponIndex = 0;
        _weaponDisplayNames = { "weapon_1004_r", "weapon_1004b_r", "weapon_1004c_r", "weapon_1004d_r", "weapon_1004e_r"};
        _logoText = nullptr;
        //
        _factory = dragonBones::CCFactory::getFactory();
        _factory->loadDragonBonesData("mecha_1004d_show/mecha_1004d_show_ske.json");
        _factory->loadTextureAtlasData("mecha_1004d_show/mecha_1004d_show_tex.json");
        _factory->loadDragonBonesData("weapon_1004_show/weapon_1004_show_ske.json");
        _factory->loadTextureAtlasData("weapon_1004_show/weapon_1004_show_tex.json");
        //
        _armatureDisplay = _factory->buildArmatureDisplay("mecha_1004d");
        _armatureDisplay->getAnimation()->play();
        //
        _armatureDisplay->setPosition(100.0f, -200.0f);
        addChild(_armatureDisplay);
        //
        const auto listener = cocos2d::EventListenerMouse::create();
        listener->onMouseDown = CC_CALLBACK_1(ReplaceSlotDisplay::_mouseDownHandler, this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        //
        createText("Touch screen left / center / right to relace slot display.");
    }

private:
    void _mouseDownHandler(cocos2d::EventMouse* event)
    {
        const auto localX = event->getLocation().x - getPosition().x;
        if (localX < -150.0f) 
        {
            _replaceDisplay(-1);
        }
        else if (localX > 150.0f) 
        {
            _replaceDisplay(1);
        }
        else 
        {
            _replaceDisplay(0);
        }
    }

    void _replaceDisplay(int type)
    {

        if (type == -1)
        {
            _rightWeaponIndex++;
            _rightWeaponIndex %= _weaponDisplayNames.size();
            const auto displayName = _weaponDisplayNames[_rightWeaponIndex];
            _factory->replaceSlotDisplay("weapon_1004_show", "weapon", "weapon_r", displayName, _armatureDisplay->getArmature()->getSlot("weapon_hand_r"));
        }
        else if (type == 1)
        {
            _leftWeaponIndex++;
            _leftWeaponIndex %= 5;
            _armatureDisplay->getArmature()->getSlot("weapon_hand_l")->setDisplayIndex(_leftWeaponIndex);
        }
        else
        {
            const auto logoSlot = _armatureDisplay->getArmature()->getSlot("logo");
            if (logoSlot->getDisplay() == _logoText) 
            {
                logoSlot->setDisplay(logoSlot->getRawDisplay(), dragonBones::DisplayType::Image);
            }
            else 
            {
                if (!_logoText) 
                {
                    _logoText = cocos2d::Label::create();
                    _logoText->retain();
                    _logoText->setString("Core Element");
                    _logoText->setAlignment(cocos2d::TextHAlignment::CENTER);
                }

                logoSlot->setDisplay(_logoText, dragonBones::DisplayType::Image);
            }
        }
    }
};