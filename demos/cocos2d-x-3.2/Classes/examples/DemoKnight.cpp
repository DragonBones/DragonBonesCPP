#include "DemoKnight.h"

USING_NS_DB;

DemoKnight::DemoKnight()
: _armatureNode(nullptr)
, _armArmature(nullptr)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
, _keyboardListener(nullptr)
#endif
{

}

DemoKnight::~DemoKnight()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    this->getEventDispatcher()->removeEventListener(_keyboardListener);
    _keyboardListener = nullptr;
#endif
    _armatureNode = nullptr;
}

std::string DemoKnight::title()
{
    return "Knight Demo";
}

std::string DemoKnight::subtitle()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    return "Press W / A / D to move.Press S to upgrade weapon.\nPress SPACE to switch weapons.Press K to attack.";
#else
    return "";
#endif
}

void DemoKnight::update(float dt)
{
    updateSpeed();
    updateArrows();
}

void DemoKnight::demoInit()
{
    // factory
    DBCCFactory::getInstance()->loadDragonBonesData("armatures/Knight/skeleton.xml", "Knight");
    DBCCFactory::getInstance()->loadTextureAtlas("armatures/Knight/texture.xml", "Knight");

    // armature
    _armatureNode = DBCCFactory::getInstance()->buildArmatureNode("knight");
    _armArmature = _armatureNode->getCCSlot("armOutside")->getCCChildArmature();
    _armatureNode->setPosition(480.f, 200.f);
    this->addChild(_armatureNode);
    // armature event
    _armArmature->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::FADE_IN, std::bind(&DemoKnight::armAnimationHandler, this, std::placeholders::_1));
    _armArmature->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::COMPLETE, std::bind(&DemoKnight::armAnimationHandler, this, std::placeholders::_1));
    _armArmature->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::ANIMATION_FRAME_EVENT, std::bind(&DemoKnight::armAnimationHandler, this, std::placeholders::_1));
    // key
    addInteraction();
    //
    _isLeft = false;
    _isRight = false;
    _isJump = false;
    _moveDir = 0;
    _weaponIndex = 0;
    _speedX = 0.f;
    _speedY = 0.f;
    _weaponList.push_back("sword");
    _weaponList.push_back("pike");
    _weaponList.push_back("axe");
    _weaponList.push_back("bow");
    _weaponLevelList.push_back(0);
    _weaponLevelList.push_back(0);
    _weaponLevelList.push_back(0);
    _weaponLevelList.push_back(0);
    _isAttacking = false;
    _isComboAttack = false;
    _hitCount = 1;
    //
    updateAnimation();
    scheduleUpdate();
}

void DemoKnight::addInteraction()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    _keyboardListener = cocos2d::EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = CC_CALLBACK_2(DemoKnight::keyPressHandler, this);
    _keyboardListener->onKeyReleased = CC_CALLBACK_2(DemoKnight::keyReleaseHandler, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);
#else
    std::vector<MenuItemFont*> items({
        MenuItemFont::create("Move Left", std::bind([](DemoKnight* ref){ref->move(ref->_moveDir, false); ref->move(-1, true); }, this)),
        MenuItemFont::create("Move Right", std::bind([](DemoKnight* ref){ref->move(ref->_moveDir, false); ref->move(1, true); }, this)),
        MenuItemFont::create("Stop", std::bind([](DemoKnight* ref){ ref->move(ref->_moveDir, false); }, this)),
        MenuItemFont::create("Jump", std::bind([](DemoKnight* ref){ref->jump(); }, this)),
        MenuItemFont::create("Change Weapon", std::bind([](DemoKnight* ref){ref->changeWeapon(); }, this)),
        MenuItemFont::create("Update Weapon Level", std::bind([](DemoKnight* ref){ref->upgradeWeaponLevel(); }, this)),
        MenuItemFont::create("Attack", std::bind([](DemoKnight* ref){ref->attack(); }, this))
    });

    Menu *menu = Menu::create();
    for (auto i = 0; i < items.size(); ++i)
    {
        items[i]->setAnchorPoint(Point(0, 0));
        menu->addChild(items[i], 0, i);
    }

    menu->setPosition(VisibleRect::left(10, 0));
    menu->alignItemsVertically();

    this->addChild(menu);
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
void DemoKnight::keyReleaseHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    switch (keyCode)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_S:
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

void DemoKnight::keyPressHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    switch (keyCode)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        jump();
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_S:
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        upgradeWeaponLevel();
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
        changeWeapon();
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_K:
        attack();
        break;
    }
}
#endif

void DemoKnight::jump()
{
    if (_isJump)
    {
        return;
    }

    _isJump = true;
    _speedY = 24.f;
    _armatureNode->getAnimation()->gotoAndPlay("jump");
}

void DemoKnight::move(int dir, bool isDown)
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

void DemoKnight::armReady()
{
    const std::string &weaponName = _weaponList[_weaponIndex];
    const std::string &animationName = "ready_" + weaponName;
    _armArmature->getAnimation()->gotoAndPlay(animationName);
}

void DemoKnight::updateAnimation()
{
    if (_isJump)
    {
    }
    else if (_moveDir != 0)
    {
        _speedX = _moveDir * 6.f;
        _armatureNode->getAnimation()->gotoAndPlay("run");
        _armatureNode->setScaleX(_moveDir);
    }
    else
    {
        _speedX = 0.f;
        _armatureNode->getAnimation()->gotoAndPlay("stand");
    }
}

void DemoKnight::changeWeapon()
{
    ++_weaponIndex;

    if (_weaponIndex >= (int)(_weaponList.size()))
    {
        _weaponIndex = 0;
    }

    armReady();
}

void DemoKnight::upgradeWeaponLevel()
{
    int weaponLevel = _weaponLevelList[_weaponIndex];
    ++weaponLevel;

    if (weaponLevel >= 3)
    {
        weaponLevel = 0;
    }

    _weaponLevelList[_weaponIndex] = weaponLevel;
    const std::string &weaponName = _weaponList[_weaponIndex];

    int newWeaponLevel = weaponLevel + 1;
    switch (_weaponIndex)
    {
    case 0:
    case 1:
    case 2:
    {
        dragonBones::DBCCSlot *weaponSlot = _armArmature->getCCSlot("weapon");
        weaponSlot->setDisplay(DBCCFactory::getInstance()->getTextureDisplay(getWeaponName(weaponName, newWeaponLevel)));
        break;
    }

    case 3:
    {
        dragonBones::DBCCSlot *bowSlot = _armArmature->getCCSlot("bow");
        dragonBones::DBCCSlot *bowBA = bowSlot->getCCChildArmature()->getCCSlot("ba");
        dragonBones::DBCCSlot *bowBB = bowSlot->getCCChildArmature()->getCCSlot("bb");
        dragonBones::DBCCSlot *bowArrow = bowSlot->getCCChildArmature()->getCCSlot("arrow");
        dragonBones::DBCCSlot *bowArrowB = bowSlot->getCCChildArmature()->getCCSlot("arrowBackup");
        bowBA->setDisplay(DBCCFactory::getInstance()->getTextureDisplay(getWeaponName(weaponName, newWeaponLevel)));
        bowBB->setDisplay(DBCCFactory::getInstance()->getTextureDisplay(getWeaponName(weaponName, newWeaponLevel)));
        bowArrow->setDisplay(DBCCFactory::getInstance()->getTextureDisplay(getWeaponName("arrow", newWeaponLevel)));
        bowArrowB->setDisplay(DBCCFactory::getInstance()->getTextureDisplay(getWeaponName("arrow", newWeaponLevel)));
        break;
    }
    }
}

void DemoKnight::attack()
{
    if (_isAttacking)
    {
        return;
    }

    _isAttacking = true;
    const std::string &weaponName = _weaponList[_weaponIndex];
    char animationName[512];
    sprintf(animationName, "attack_%s_%d", weaponName.c_str(), _hitCount);
    _armArmature->getAnimation()->gotoAndPlay(animationName);
}

void DemoKnight::armAnimationHandler(cocos2d::EventCustom *event)
{
    dragonBones::EventData *eventData = (dragonBones::EventData*)(event->getUserData());

    switch (eventData->getType())
    {
    case dragonBones::EventData::EventType::FADE_IN:
        _isComboAttack = false;
        cocos2d::log("animation fade in: %s", eventData->animationState->name.c_str());
        break;

    case dragonBones::EventData::EventType::COMPLETE:
        cocos2d::log("animation complete: %s _isComboAttack: %d", eventData->animationState->name.c_str(), _isComboAttack);

        if (_isComboAttack)
        {
            armReady();
        }
        else
        {
            _isAttacking = false;
            _hitCount = 1;
            _isComboAttack = false;
        }

        break;

    case dragonBones::EventData::EventType::ANIMATION_FRAME_EVENT:
    {
        if (eventData->frameLabel == "fire")
        {
            dragonBones::Bone *bowBone = _armArmature->getBone("bow");
            cocos2d::Point resultPoint = _armArmature->getCCDisplay()->convertToWorldSpace(cocos2d::Point(bowBone->global.x, -bowBone->global.y));
            float r = 0.f;

            if (_armatureNode->getScaleX() > 0)
            {
                r = CC_DEGREES_TO_RADIANS(-_armatureNode->getRotation()) + bowBone->global.getRotation();
            }
            else
            {
                r = CC_DEGREES_TO_RADIANS(-_armatureNode->getRotation()) - bowBone->global.getRotation() + dragonBones::PI;
            }

            switch (_weaponLevelList[_weaponIndex])
            {
            case 0:
                createArrow(r, resultPoint);
                break;

            case 1:
                createArrow(3.f / 180.f * dragonBones::PI + r, resultPoint);
                createArrow(-3.f / 180.f * dragonBones::PI + r, resultPoint);
                break;

            case 2:
                createArrow(6.f / 180.f * dragonBones::PI + r, resultPoint);
                createArrow(r, resultPoint);
                createArrow(-6.f / 180.f * dragonBones::PI + r, resultPoint);
                break;
            }

            cocos2d::log("frameEvent: %s", eventData->frameLabel.c_str());
        }
        else if (eventData->frameLabel == "ready")
        {
            _isAttacking = false;
            _isComboAttack = true;
            ++_hitCount;
            cocos2d::log("attack ready: %s", eventData->animationState->name.c_str());
        }

        break;
    }
    }
}

void DemoKnight::createArrow(float r, const cocos2d::Point &point)
{
    cocos2d::Node *arrowNode = static_cast<cocos2d::Node*>(DBCCFactory::getInstance()->getTextureDisplay(getWeaponName("arrow", 1)));
    arrowNode->setPosition(point);
    arrowNode->setRotation(CC_RADIANS_TO_DEGREES(r));
    cocos2d::Point *speedPoint = new cocos2d::Point();
    speedPoint->x = cos(r) * 36;
    speedPoint->y = -sin(r) * 36;
    arrowNode->setUserData(speedPoint);
    _arrowList.push_back(arrowNode);
    this->addChild(arrowNode);
}

void DemoKnight::updateArrows()
{
    for (size_t i = 0, l = _arrowList.size(); i < l; ++i)
    {
        cocos2d::Node *arrowNode = _arrowList[i];
        cocos2d::Point *speedPoint = static_cast<cocos2d::Point*>(arrowNode->getUserData());

        if (arrowNode->getPositionY() < -400)
        {
            if (i == l - 1)
            {
                delete speedPoint;
                this->removeChild(arrowNode);
                _arrowList.pop_back();
            }
        }
        else
        {
            speedPoint->y += -1.f;
            arrowNode->setPosition(arrowNode->getPosition() + *speedPoint);
            arrowNode->setRotation(-CC_RADIANS_TO_DEGREES(atan2(speedPoint->y, speedPoint->x)));
        }
    }
}

void DemoKnight::updateSpeed()
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
            if (_armatureNode->getAnimation()->hasAnimation("fall"))
            {
                _armatureNode->getAnimation()->gotoAndPlay("fall", 0.2f);
            }
        }

        _speedY += speedG;
        y += _speedY * timeScale;

        if (y < 200)
        {
            y = 200.f;
            _isJump = false;
            _speedY = 0.f;
            _speedX = 0.f;
            _armatureNode->setRotation(0.f);
            updateAnimation();
        }
        else
        {
            _armatureNode->setRotation(-_speedY * _armatureNode->getScaleX());
        }
    }

    _armatureNode->setPosition(x, y);
}