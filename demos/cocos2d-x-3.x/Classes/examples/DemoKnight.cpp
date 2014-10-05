#include "DemoKnight.h"

//DemoKnight::DemoKnight()
//: _armature(nullptr)
//, _armArmature(nullptr)
//{
//
//}
//
//DemoKnight::~DemoKnight()
//{
//   //_armature->dispose();
//   //_armarmature->dispose();
//   //cc_safe_delete(_armature);
//   //cc_safe_delete(_armarmature);
//}

std::string DemoKnight::title()
{
    return "Knight Demo";
}

std::string DemoKnight::subtitle()
{
    return "Press W / A / D to move.Press S to upgrade weapon.\nPress SPACE to switch weapons.Press K to attack.";
}

void DemoKnight::demoInit()
{
    // factory
    dragonBones::DBCCFactory::factory.loadDragonBonesData("armatures/Knight/skeleton.xml", "Knight");
    dragonBones::DBCCFactory::factory.loadTextureAtlas("armatures/Knight/texture.xml", "Knight");

    // armature
    _armature = (dragonBones::DBCCArmature*)(dragonBones::DBCCFactory::factory.buildArmature("knight"));
    _armArmature = _armature->getCCSlot("armOutside")->getCCChildArmature();
    _armature->getCCDisplay()->setPosition(480.f, 200.f);
    this->addChild(_armature->getCCDisplay());
    // armature event
    _armArmature->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::FADE_IN, std::bind(&DemoKnight::armAnimationHandler, this, std::placeholders::_1));
    _armArmature->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::COMPLETE, std::bind(&DemoKnight::armAnimationHandler, this, std::placeholders::_1));
    _armArmature->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::ANIMATION_FRAME_EVENT, std::bind(&DemoKnight::armAnimationHandler, this, std::placeholders::_1));
    // update
    dragonBones::WorldClock::clock.add(_armature);
    // key
    cocos2d::EventListenerKeyboard *listener = cocos2d::EventListenerKeyboard::create();
    listener->onKeyPressed = std::bind(&DemoKnight::keyPressHandler, this, std::placeholders::_1, std::placeholders::_2);
    listener->onKeyReleased = std::bind(&DemoKnight::keyReleaseHandler, this, std::placeholders::_1, std::placeholders::_2);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
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
}

void DemoKnight::updateHandler(float passTime)
{
    updateSpeed();
    dragonBones::WorldClock::clock.advanceTime(passTime);
    updateArrows();
}

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

void DemoKnight::jump()
{
    if (_isJump)
    {
        return;
    }

    _isJump = true;
    _speedY = 24.f;
    _armature->getAnimation()->gotoAndPlay("jump");
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
        _armature->getAnimation()->gotoAndPlay("run");
        _armature->getCCDisplay()->setScaleX(_moveDir);
    }
    else
    {
        _speedX = 0.f;
        _armature->getAnimation()->gotoAndPlay("stand");
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
        weaponSlot->setDisplay(dragonBones::DBCCFactory::factory.getTextureDisplay(getWeaponName(weaponName, newWeaponLevel)));
        break;
    }

    case 3:
    {
        dragonBones::DBCCSlot *bowSlot = _armArmature->getCCSlot("bow");
        dragonBones::DBCCSlot *bowBA = bowSlot->getCCChildArmature()->getCCSlot("ba");
        dragonBones::DBCCSlot *bowBB = bowSlot->getCCChildArmature()->getCCSlot("bb");
        dragonBones::DBCCSlot *bowArrow = bowSlot->getCCChildArmature()->getCCSlot("arrow");
        dragonBones::DBCCSlot *bowArrowB = bowSlot->getCCChildArmature()->getCCSlot("arrowBackup");
        bowBA->setDisplay(dragonBones::DBCCFactory::factory.getTextureDisplay(getWeaponName(weaponName, newWeaponLevel)));
        bowBB->setDisplay(dragonBones::DBCCFactory::factory.getTextureDisplay(getWeaponName(weaponName, newWeaponLevel)));
        bowArrow->setDisplay(dragonBones::DBCCFactory::factory.getTextureDisplay(getWeaponName("arrow", newWeaponLevel)));
        bowArrowB->setDisplay(dragonBones::DBCCFactory::factory.getTextureDisplay(getWeaponName("arrow", newWeaponLevel)));
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

            if (_armature->getCCDisplay()->getScaleX() > 0)
            {
                r = CC_DEGREES_TO_RADIANS(-_armature->getCCDisplay()->getRotation()) + bowBone->global.getRotation();
            }
            else
            {
                r = CC_DEGREES_TO_RADIANS(-_armature->getCCDisplay()->getRotation()) - bowBone->global.getRotation() + dragonBones::PI;
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
    cocos2d::Node *arrowNode = static_cast<cocos2d::Node*>(dragonBones::DBCCFactory::factory.getTextureDisplay(getWeaponName("arrow", 1)));
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
    float timeScale = _armature->getAnimation()->getTimeScale();
    float x = _armature->getCCDisplay()->getPositionX();
    float y = _armature->getCCDisplay()->getPositionY();

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
            if (_armature->getAnimation()->hasAnimation("fall"))
            {
                _armature->getAnimation()->gotoAndPlay("fall", 0.2f);
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
            _armature->getCCDisplay()->setRotation(0.f);
            updateAnimation();
        }
        else
        {
            _armature->getCCDisplay()->setRotation(-_speedY * _armature->getCCDisplay()->getScaleX());
        }
    }

    _armature->getCCDisplay()->setPosition(x, y);
}