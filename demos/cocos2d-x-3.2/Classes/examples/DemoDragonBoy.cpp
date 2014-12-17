#include "DemoDragonBoy.h"

USING_NS_DB;

DemoDragonBoy::DemoDragonBoy()
: _armatureNode(nullptr)
, _fallEndFadeOutListener(nullptr)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
, _keyboardListener(nullptr)
#endif
{

}

DemoDragonBoy::~DemoDragonBoy()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    this->getEventDispatcher()->removeEventListener(_keyboardListener);
    _keyboardListener = nullptr;
#endif
    if (_fallEndFadeOutListener)
    {
        _armatureNode->getCCEventDispatcher()->removeEventListener(_fallEndFadeOutListener);
        _fallEndFadeOutListener = nullptr;
    }
    _armatureNode = nullptr;
}

std::string DemoDragonBoy::title()
{
    return "DragonBoy Demo";
}

std::string DemoDragonBoy::subtitle()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    return "Press W/S/A/D to move.\nPress SPACE to switch clothes.";
#else
    return "";
#endif
}

void DemoDragonBoy::demoInit()
{
    // factory
    DBCCFactory::getInstance()->loadDragonBonesData("armatures/DragonBoy/skeleton.xml", "DragonBoy");
    DBCCFactory::getInstance()->loadTextureAtlas("armatures/DragonBoy/texture.xml", "DragonBoy");
    // armature
    _armatureNode = DBCCFactory::getInstance()->buildArmatureNode("dragonBoy");
    _armatureNode->setPosition(480.f, 200.f);
    _armatureNode->setScale(0.5f);
    addChild(_armatureNode);
    // event
    _armatureNode->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::BONE_FRAME_EVENT, CC_CALLBACK_1(DemoDragonBoy::eventHandler, this));
    _armatureNode->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::ANIMATION_FRAME_EVENT, CC_CALLBACK_1(DemoDragonBoy::eventHandler, this));
    _armatureNode->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::FADE_IN, CC_CALLBACK_1(DemoDragonBoy::eventHandler, this));
    _armatureNode->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::FADE_OUT, CC_CALLBACK_1(DemoDragonBoy::eventHandler, this));
    _armatureNode->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::FADE_IN_COMPLETE, CC_CALLBACK_1(DemoDragonBoy::eventHandler, this));
    _armatureNode->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::FADE_OUT_COMPLETE, CC_CALLBACK_1(DemoDragonBoy::eventHandler, this));
    _armatureNode->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::START, CC_CALLBACK_1(DemoDragonBoy::eventHandler, this));
    _armatureNode->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::COMPLETE, CC_CALLBACK_1(DemoDragonBoy::eventHandler, this));
    _armatureNode->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::LOOP_COMPLETE, CC_CALLBACK_1(DemoDragonBoy::eventHandler, this));
    _armatureNode->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::SOUND, CC_CALLBACK_1(DemoDragonBoy::eventHandler, this));
    _armatureNode->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::Z_ORDER_UPDATED, CC_CALLBACK_1(DemoDragonBoy::eventHandler, this));
    // interaction
    addInteraction();
    //
    _isLeft = false;
    _isRight = false;
    _isJump = false;
    _isSquat = false;
    _moveDir = 0;
    _currentClotheIndex = 0;
    _speedX = 0.f;
    _speedY = 0.f;
    _clothesList.push_back("parts/clothes1");
    _clothesList.push_back("parts/clothes2");
    _clothesList.push_back("parts/clothes3");
    _clothesList.push_back("parts/clothes4");
    //
    updateAnimation();
    scheduleUpdate();
}

void DemoDragonBoy::addInteraction()
{
    // key
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    _keyboardListener = cocos2d::EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = CC_CALLBACK_2(DemoDragonBoy::keyPressHandler, this);
    _keyboardListener->onKeyReleased = CC_CALLBACK_2(DemoDragonBoy::keyReleaseHandler, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);
#else
    std::vector<MenuItemFont*> items({
        MenuItemFont::create("Move Left", std::bind([](DemoDragonBoy* ref){ref->squat(false); ref->move(ref->_moveDir, false); ref->move(-1, true); }, this)),
        MenuItemFont::create("Move Right", std::bind([](DemoDragonBoy* ref){ref->squat(false); ref->move(ref->_moveDir, false); ref->move(1, true); }, this)),
        MenuItemFont::create("Stop", std::bind([](DemoDragonBoy* ref){ref->squat(false); ref->move(ref->_moveDir, false); }, this)),
        MenuItemFont::create("Jump", std::bind([](DemoDragonBoy* ref){ref->squat(false); ref->jump(); }, this)),
        MenuItemFont::create("Squat", std::bind([](DemoDragonBoy* ref){ref->squat(true); }, this)),
        MenuItemFont::create("Change Clothes", std::bind([](DemoDragonBoy* ref){ref->changeClothe(); }, this))
    });

    Menu *menu = Menu::create();
    for (auto i = 0; i < items.size(); ++i)
    {
        items[i]->setAnchorPoint(Vec2(0,0));
        menu->addChild(items[i], 0, i);
    }

    menu->setPosition(VisibleRect::left(10, 0));
    menu->alignItemsVertically();

    this->addChild(menu);
#endif
}

void DemoDragonBoy::update(float passTime)
{
    updateSpeed();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
void DemoDragonBoy::keyPressHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    switch (keyCode)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        jump();
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_S:
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        squat(true);
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_A:
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        move(-1, true);
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_D:
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        move(1, true);
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
        changeClothe();
        break;
    }
}

void DemoDragonBoy::keyReleaseHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    switch (keyCode)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_S:
        squat(false);
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_A:
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        move(-1, false);
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_D:
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        move(1, false);
        break;
    }
}
#endif

void DemoDragonBoy::eventHandler(cocos2d::EventCustom *event)
{
    dragonBones::EventData *eventData = static_cast<dragonBones::EventData*>(event->getUserData());

    if (eventData->animationState)
    {
        CCLOG("Animation name: %s, Event type: %s",
            eventData->animationState->name.c_str(),
            eventData->getStringType().c_str()
            );
    }
}

void DemoDragonBoy::jump()
{
    if (_isJump)
    {
        return;
    }

    _isJump = true;
    _speedY = 24.f;
    _armatureNode->getAnimation()->gotoAndPlay("jump");
}

void DemoDragonBoy::squat(bool isSquat)
{
    if (_isSquat == isSquat)
    {
        return;
    }

    _isSquat = isSquat;
    updateAnimation();
}

void DemoDragonBoy::move(int dir, bool isDown)
{
    if (dir < 0)
    {
        _isLeft = isDown;
    }
    else if (dir > 0)
    {
        _isRight = isDown;
    }

    int moveDir = 0;

    if (_isLeft && _isRight)
    {
        moveDir = _moveDir;
    }
    else if (_isLeft)
    {
        moveDir = -1;
    }
    else if (_isRight)
    {
        moveDir = 1;
    }
    else
    {
        moveDir = 0;
    }

    if (_moveDir == moveDir)
    {
        return;
    }

    _moveDir = moveDir;
    updateAnimation();
}

void DemoDragonBoy::updateAnimation()
{
    if (_isJump)
    {
    }
    else if (_isSquat)
    {
        _speedX = 0.f;
        _armatureNode->getAnimation()->gotoAndPlay("squat");
    }
    else if (_moveDir != 0)
    {
        _speedX = _moveDir * 6.f;
        _armatureNode->getAnimation()->gotoAndPlay("walk");
        _armatureNode->setScaleX(-_moveDir * 0.5f);
    }
    else
    {
        _speedX = 0.f;
        _armatureNode->getAnimation()->gotoAndPlay("stand");
    }
}

void DemoDragonBoy::changeClothe()
{
    ++_currentClotheIndex;

    if (_currentClotheIndex > (int)(_clothesList.size()))
    {
        _currentClotheIndex = 0;
    }

    if (_currentClotheIndex < (int)(_clothesList.size()))
    {
        _armatureNode->getCCSlot("clothes")->setDisplay(DBCCFactory::getInstance()->getTextureDisplay(_clothesList[_currentClotheIndex]));
    }
    else
    {
        _armatureNode->getCCSlot("clothes")->setDisplay(nullptr);
    }
}

void DemoDragonBoy::updateSpeed()
{
    float timeScale = _armatureNode->getAnimation()->getTimeScale();
    float x = _armatureNode->getPositionX();
    float y = _armatureNode->getPositionY();

    if (_speedX != 0)
    {
        x += _speedX * timeScale;

        if (x < 0)
        {
            x = 0.f;
        }
        else if (x > 960)
        {
            x = 960.f;
        }
    }

    if (_isJump)
    {
        float speedG = -1.f * timeScale;

        if (_speedY >= 0 && _speedY + speedG < 0)
        {
            _armatureNode->getAnimation()->gotoAndPlay("fall", 0.2f);
        }

        _speedY += speedG;
        y += _speedY * timeScale;

        if (y < 200)
        {
            y = 200.f;
            _isJump = false;
            _speedY = 0.f;
            _speedX = 0.f;
            _fallEndFadeOutListener = _armatureNode->getCCEventDispatcher()->addCustomEventListener(
                dragonBones::EventData::FADE_OUT_COMPLETE,
                CC_CALLBACK_1(DemoDragonBoy::fallEndFadeOutCompleteHandler, this)
                );
            _armatureNode->getAnimation()->gotoAndPlay("fallEnd");
        }
    }

    _armatureNode->setPosition(x, y);
}

void DemoDragonBoy::fallEndFadeOutCompleteHandler(cocos2d::EventCustom *event)
{
    _armatureNode->getCCEventDispatcher()->removeEventListener(_fallEndFadeOutListener);
    _fallEndFadeOutListener = nullptr;
    updateAnimation();
}
