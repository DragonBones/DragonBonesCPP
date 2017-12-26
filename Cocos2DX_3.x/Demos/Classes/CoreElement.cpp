#include "CoreElement.h"

USING_NS_CC;
int CoreElementGame::GROUND;
const float CoreElementGame::G = -0.6f;
CoreElementGame* CoreElementGame::instance = nullptr;

void CoreElementGame::_onStart()
{
    CoreElementGame::GROUND = -getStageHeight() * 0.5f + 150.0f;
    CoreElementGame::instance = this;

    _left = false;
    _right = false;

    //
    const auto factory = dragonBones::CCFactory::getFactory();
    factory->loadDragonBonesData("mecha_1502b/mecha_1502b_ske.json");
    factory->loadTextureAtlasData("mecha_1502b/mecha_1502b_tex.json");
    factory->loadDragonBonesData("skin_1502b/skin_1502b_ske.json");
    factory->loadTextureAtlasData("skin_1502b/skin_1502b_tex.json");
    factory->loadDragonBonesData("weapon_1000/weapon_1000_ske.json");
    factory->loadTextureAtlasData("weapon_1000/weapon_1000_tex.json");
    _player = new Mecha();
    //
    const auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(CoreElementGame::_keyBoardPressedHandler, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(CoreElementGame::_keyBoardReleasedHandler, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    const auto touchListener = cocos2d::EventListenerMouse::create();
    touchListener->onMouseDown = CC_CALLBACK_1(CoreElementGame::_mouseDownHandler, this);
    touchListener->onMouseUp = CC_CALLBACK_1(CoreElementGame::_mouseUpHandler, this);
    touchListener->onMouseMove = CC_CALLBACK_1(CoreElementGame::_mouseMovedHandler, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    cocos2d::Director::getInstance()->getScheduler()->schedule(
        schedule_selector(CoreElementGame::_enterFrameHandler),
        this, 0.0f, false
    );
    //
    createText("Press W/A/S/D to move. Press Q/E/SPACE to switch weapons and skin. Touch to aim and fire.");
}

void CoreElementGame::_keyBoardPressedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        _left = true;
        _updateMove(-1);
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_D:
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        _right = true;
        _updateMove(1);
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_W:
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        _player->jump();
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_S:
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        _player->squat(true);
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_Q:
        _player->switchWeaponR();
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_E:
        _player->switchWeaponL();
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
        _player->switchSkin();
        break;
    }
}

void CoreElementGame::_keyBoardReleasedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        _left = false;
        _updateMove(-1);
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_D:
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        _right = false;
        _updateMove(1);
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_W:
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_S:
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        _player->squat(false);
        break;
    }
}

void CoreElementGame::_mouseDownHandler(cocos2d::EventMouse* event)
{
    _player->attack(true);
}
void CoreElementGame::_mouseUpHandler(cocos2d::EventMouse* event)
{
    _player->attack(false);
}

void CoreElementGame::_mouseMovedHandler(cocos2d::EventMouse* event)
{
    cocos2d::Vec2 target;
    target.x = event->getCursorX() - getPositionX();
    target.y = event->getCursorY() - getPositionY();
    _player->aim(target);
}

void CoreElementGame::_enterFrameHandler(float passedTime)
{
    _player->update();

    int i = _bullets.size();
    while (i--)
    {
        const auto bullet = _bullets[i];
        if (bullet->update())
        {
            _bullets.erase(std::find(_bullets.begin(), _bullets.end(), bullet));
            delete bullet;
        }
    }
}

void CoreElementGame::_updateMove(int dir) const
{
    if (_left && _right)
    {
        _player->move(dir);
    }
    else if (_left)
    {
        _player->move(-1);
    }
    else if (_right)
    {
        _player->move(1);
    }
    else
    {
        _player->move(0);
    }
}

void CoreElementGame::addBullet(CoreElementBullet* bullet)
{
    _bullets.push_back(bullet);
}

const std::string Mecha::NORMAL_ANIMATION_GROUP = "normal";
const std::string Mecha::AIM_ANIMATION_GROUP = "aim";
const std::string Mecha::ATTACK_ANIMATION_GROUP = "attack";
const float Mecha::JUMP_SPEED = -20.0f;
const float Mecha::NORMALIZE_MOVE_SPEED = 3.6f;
const float Mecha::MAX_MOVE_SPEED_FRONT = NORMALIZE_MOVE_SPEED * 1.4f;
const float Mecha::MAX_MOVE_SPEED_BACK = NORMALIZE_MOVE_SPEED * 1.0f;
const char* Mecha::WEAPON_R_LIST[6] = { "weapon_1502b_r", "weapon_1005", "weapon_1005b", "weapon_1005c", "weapon_1005d", "weapon_1005e" };
const char* Mecha::WEAPON_L_LIST[5] = { "weapon_1502b_l", "weapon_1005", "weapon_1005b", "weapon_1005c", "weapon_1005d" };
const char* Mecha::SKINS[4] = { "mecha_1502b", "skin_a", "skin_b", "skin_c" };

Mecha::Mecha() :
    _isJumpingA(false),
    _isJumpingB(false),
    _isSquating(false),
    _isAttackingA(false),
    _isAttackingB(false),
    _skinIndex(0),
    _weaponRIndex(0),
    _weaponLIndex(0),
    _faceDir(1),
    _aimDir(0),
    _moveDir(0),
    _aimRadian(0.0f),
    _speedX(0.0f),
    _speedY(0.0f),
    _armatureDisplay(nullptr),
    _weaponR(nullptr),
    _weaponL(nullptr),
    _aimState(nullptr),
    _walkState(nullptr),
    _attackState(nullptr),
    _target()
{
    _armatureDisplay = dragonBones::CCFactory::getFactory()->buildArmatureDisplay("mecha_1502b");
    _armatureDisplay->setPosition(0.0f, CoreElementGame::GROUND);
    _armatureDisplay->getEventDispatcher()->setEnabled(true);
    _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FADE_IN_COMPLETE, std::bind(&Mecha::_animationEventHandler, this, std::placeholders::_1));
    _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FADE_OUT_COMPLETE, std::bind(&Mecha::_animationEventHandler, this, std::placeholders::_1));
    _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::COMPLETE, std::bind(&Mecha::_animationEventHandler, this, std::placeholders::_1));

    _weaponR = _armatureDisplay->getArmature()->getSlot("weapon_r")->getChildArmature();
    _weaponL = _armatureDisplay->getArmature()->getSlot("weapon_l")->getChildArmature();
    const auto weaponRDisplay = static_cast<dragonBones::CCArmatureDisplay*>(_weaponR->getDisplay());
    const auto weaponLDisplay = static_cast<dragonBones::CCArmatureDisplay*>(_weaponL->getDisplay());
    weaponRDisplay->getEventDispatcher()->setEnabled(true);
    weaponLDisplay->getEventDispatcher()->setEnabled(true);
    weaponRDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FRAME_EVENT, std::bind(&Mecha::_frameEventHandler, this, std::placeholders::_1));
    weaponLDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FRAME_EVENT, std::bind(&Mecha::_frameEventHandler, this, std::placeholders::_1));

    CoreElementGame::instance->addChild(_armatureDisplay);
    _updateAnimation();
}

Mecha::~Mecha()
{
}

void Mecha::move(int dir)
{
    if (_moveDir == dir)
    {
        return;
    }

    _moveDir = dir;
    _updateAnimation();
}

void Mecha::jump()
{
    if (_isJumpingA)
    {
        return;
    }

    _isJumpingA = true;
    _armatureDisplay->getAnimation()->fadeIn(
        "jump_1", -1.0f, -1, 
        0, NORMAL_ANIMATION_GROUP
    )->resetToPose = false;

    _walkState = nullptr;
}

void Mecha::squat(bool isSquating)
{
    if (_isSquating == isSquating)
    {
        return;
    }

    _isSquating = isSquating;
    _updateAnimation();
}

void Mecha::attack(bool isAttacking)
{
    if (_isAttackingA == isAttacking)
    {
        return;
    }

    _isAttackingA = isAttacking;
}

void Mecha::switchWeaponR()
{
    auto weaponDisplay = static_cast<dragonBones::CCArmatureDisplay*>(_weaponR->getDisplay());
    weaponDisplay->getEventDispatcher()->removeCustomEventListeners(dragonBones::EventObject::FRAME_EVENT);

    _weaponRIndex++;
    if (_weaponRIndex >= sizeof(WEAPON_R_LIST) / sizeof(WEAPON_R_LIST[0]))
    {
        _weaponRIndex = 0;
    }

    const auto weaponName = WEAPON_R_LIST[_weaponRIndex];
    _weaponR = dragonBones::CCFactory::getFactory()->buildArmature(weaponName);
    _armatureDisplay->getArmature()->getSlot("weapon_r")->setChildArmature(_weaponR);
    weaponDisplay = static_cast<dragonBones::CCArmatureDisplay*>(_weaponR->getDisplay());
    weaponDisplay->getEventDispatcher()->setEnabled(true);
    weaponDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FRAME_EVENT, std::bind(&Mecha::_frameEventHandler, this, std::placeholders::_1));
}

void Mecha::switchWeaponL()
{
    auto weaponDisplay = static_cast<dragonBones::CCArmatureDisplay*>(_weaponL->getDisplay());
    weaponDisplay->getEventDispatcher()->removeCustomEventListeners(dragonBones::EventObject::FRAME_EVENT);

    _weaponLIndex++;
    if (_weaponLIndex >= sizeof(WEAPON_L_LIST) / sizeof(WEAPON_L_LIST[0]))
    {
        _weaponLIndex = 0;
    }

    const auto weaponName = WEAPON_L_LIST[_weaponLIndex];
    _weaponL = dragonBones::CCFactory::getFactory()->buildArmature(weaponName);
    _armatureDisplay->getArmature()->getSlot("weapon_l")->setChildArmature(_weaponL);
    weaponDisplay = static_cast<dragonBones::CCArmatureDisplay*>(_weaponL->getDisplay());
    weaponDisplay->getEventDispatcher()->setEnabled(true);
    weaponDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FRAME_EVENT, std::bind(&Mecha::_frameEventHandler, this, std::placeholders::_1));
}

void Mecha::switchSkin()
{
    _skinIndex++;

    if (_skinIndex >= sizeof(SKINS) / sizeof(SKINS[0]))
    {
        _skinIndex = 0;
    }

    const auto skinName = SKINS[_skinIndex];
    const auto skinData = dragonBones::CCFactory::getFactory()->getArmatureData(skinName)->defaultSkin;
    const std::vector<std::string> exclude = {"weapon_l", "weapon_r" };
    dragonBones::CCFactory::getFactory()->replaceSkin(_armatureDisplay->getArmature(), skinData, false, &exclude);
}

void Mecha::aim(const cocos2d::Vec2& target)
{
    _target = target;
}

void Mecha::update()
{
    _updatePosition();
    _updateAim();
    _updateAttack();
}

void Mecha::_animationEventHandler(cocos2d::EventCustom * event)
{
    const auto eventObject = (dragonBones::EventObject*)event->getUserData();
    if (eventObject->type == dragonBones::EventObject::FADE_IN_COMPLETE)
    {
        if (eventObject->animationState->getName() == "jump_1")
        {
            _isJumpingB = true;
            _speedY = -JUMP_SPEED;

            if (_moveDir != 0)
            {
                if (_moveDir * _faceDir > 0) 
                {
                    _speedX = Mecha::MAX_MOVE_SPEED_FRONT * _faceDir;
                }
                else 
                {
                    _speedX = -Mecha::MAX_MOVE_SPEED_BACK * _faceDir;
                }
            }

            _armatureDisplay->getAnimation()->fadeIn(
                "jump_2", -1.0f, -1, 
                0, NORMAL_ANIMATION_GROUP
            )->resetToPose = false;
        }
        else if (eventObject->animationState->getName() == "jump_4")
        {
            _updateAnimation();
        }
    }
    else if (eventObject->type == dragonBones::EventObject::FADE_OUT_COMPLETE)
    {
        if (eventObject->animationState->getName() == "attack_01")
        {
            _isAttackingB = false;
            _attackState = nullptr;
        }
    }
    else if (eventObject->type == dragonBones::EventObject::COMPLETE)
    {
        if (eventObject->animationState->getName() == "jump_4")
        {
            _isJumpingA = false;
            _isJumpingB = false;
            _updateAnimation();
        }
    }
}

void Mecha::_frameEventHandler(cocos2d::EventCustom* event)
{
    const auto eventObject = static_cast<dragonBones::EventObject*>(event->getUserData());
    if (eventObject->name == "fire")
    {
        const auto display = static_cast<dragonBones::CCArmatureDisplay*>(eventObject->armature->getDisplay());

        const auto transform = display->getNodeToWorldTransform();
        cocos2d::Vec3 localPoint(eventObject->bone->global.x, eventObject->bone->global.y, 0.0f);
        cocos2d::Vec2 globalPoint;
        transform.transformPoint(&localPoint);
        globalPoint.set(localPoint.x - CoreElementGame::instance->getPosition().x, localPoint.y - CoreElementGame::instance->getPosition().y);

        _fire(globalPoint);
    }
}

void Mecha::_fire(const cocos2d::Vec2& firePoint)
{
    const auto radian = _faceDir < 0 ? dragonBones::Transform::PI - _aimRadian : _aimRadian;
    const auto bullet = new CoreElementBullet("bullet_01", "fire_effect_01", radian + cocos2d::random(-0.01f, 0.01f), 40.f, firePoint);
    CoreElementGame::instance->addBullet(bullet);
}

void Mecha::_updateAnimation()
{
    if (_isJumpingA)
    {
        return;
    }

    if (_isSquating)
    {
        _speedX = 0.0f;
        _armatureDisplay->getAnimation()->fadeIn(
            "squat", -1.0f, -1, 
            0, NORMAL_ANIMATION_GROUP
        )->resetToPose = false;
        _walkState = nullptr;
        return;
    }

    if (_moveDir == 0)
    {
        _speedX = 0.0f;
        _armatureDisplay->getAnimation()->fadeIn(
            "idle", -1.0f, -1, 
            0, NORMAL_ANIMATION_GROUP
        )->resetToPose = false;
        _walkState = nullptr;
    }
    else
    {
        if (!_walkState)
        {
            _walkState = _armatureDisplay->getAnimation()->fadeIn(
                "walk", -1.0f, -1,
                0, NORMAL_ANIMATION_GROUP
            );
            _walkState->resetToPose = false;
        }

        if (_moveDir * _faceDir > 0)
        {
            _walkState->timeScale = MAX_MOVE_SPEED_FRONT / NORMALIZE_MOVE_SPEED;
        }
        else
        {
            _walkState->timeScale = -MAX_MOVE_SPEED_BACK / NORMALIZE_MOVE_SPEED;
        }

        if (_moveDir * _faceDir > 0)
        {
            _speedX = MAX_MOVE_SPEED_FRONT * _faceDir;
        }
        else
        {
            _speedX = -MAX_MOVE_SPEED_BACK * _faceDir;
        }
    }
}

void Mecha::_updatePosition()
{
    const auto& position = _armatureDisplay->getPosition();

    if (_speedX != 0.0f)
    {
        _armatureDisplay->setPosition(position.x + _speedX, position.y);
        if (position.x < -CoreElementGame::instance->getStageWidth() * 0.5f - 100.0f)
        {
            _armatureDisplay->setPosition(-CoreElementGame::instance->getStageWidth() * 0.5f - 100.0f, position.y);
        }
        else if (position.x > CoreElementGame::instance->getStageWidth() * 0.5f + 100.0f)
        {
            _armatureDisplay->setPosition(CoreElementGame::instance->getStageWidth() * 0.5f + 100.0f, position.y);
        }
    }

    if (_speedY != 0.0f)
    {
        if (_speedY > 5.0f && _speedY + CoreElementGame::G <= 5.0f)
        {
            _armatureDisplay->getAnimation()->fadeIn(
                "jump_3", -1.0f, 1, 
                0, NORMAL_ANIMATION_GROUP
            )->resetToPose = false;
        }

        _speedY += CoreElementGame::G;
        _armatureDisplay->setPosition(position.x, position.y + _speedY);

        if (position.y < CoreElementGame::GROUND)
        {
            _armatureDisplay->setPosition(position.x, CoreElementGame::GROUND);
            _speedY = 0.0f;
            _armatureDisplay->getAnimation()->fadeIn(
                "jump_4", -1.0f, -1, 
                0, NORMAL_ANIMATION_GROUP
            )->resetToPose = false;
        }
    }
}

void Mecha::_updateAim()
{
    const auto& position = _armatureDisplay->getPosition();

    _faceDir = _target.x > position.x ? 1 : -1;
    if (_armatureDisplay->getArmature()->getFlipX() ? _faceDir > 0 : _faceDir < 0)
    {
        _armatureDisplay->getArmature()->setFlipX(!_armatureDisplay->getArmature()->getFlipX());

        if (_moveDir != 0)
        {
            _updateAnimation();
        }
    }

    const auto aimOffsetY = _armatureDisplay->getArmature()->getBone("chest")->global.y * _armatureDisplay->getScaleY();
    if (_faceDir > 0)
    {
        _aimRadian = std::atan2(_target.y - position.y - aimOffsetY, _target.x - position.x);
    }
    else
    {
        _aimRadian = dragonBones::Transform::PI - std::atan2(_target.y - position.y - aimOffsetY, _target.x - position.x);
        if (_aimRadian > dragonBones::Transform::PI)
        {
            _aimRadian -= dragonBones::Transform::PI * 2.0f;
        }
    }

    int aimDir = 0;
    if (_aimRadian > 0.0f)
    {
        aimDir = 1;
    }
    else
    {
        aimDir = -1;
    }

    if (_aimState == nullptr || _aimDir != aimDir)
    {
        _aimDir = aimDir;

        // Animation Mixing.
        if (_aimDir >= 0)
        {
            _aimState = _armatureDisplay->getAnimation()->fadeIn(
                "aim_up", -1.0f, -1,
                0, AIM_ANIMATION_GROUP
            );
        }
        else
        {
            _aimState = _armatureDisplay->getAnimation()->fadeIn(
                "aim_down", -1.0f, -1,
                0, AIM_ANIMATION_GROUP
            );
        }

        _aimState->resetToPose = false;
    }

    _aimState->weight = std::abs(_aimRadian / dragonBones::Transform::PI * 2.0f);
    _armatureDisplay->getArmature()->invalidUpdate();
}

void Mecha::_updateAttack()
{
    if (!_isAttackingA || _isAttackingB)
    {
        return;
    }

    _isAttackingB = true;
    _attackState = _armatureDisplay->getAnimation()->fadeIn(
        "attack_01", -1.0f, -1,
        0, ATTACK_ANIMATION_GROUP
    );

    _attackState->resetToPose = false;
    _attackState->autoFadeOutTime = _attackState->fadeTotalTime;
}

CoreElementBullet::CoreElementBullet(const std::string& armatureName, const std::string& effectArmatureName, float radian, float speed, const cocos2d::Vec2& position)
{
    _speedX = std::cos(radian) * speed;
    _speedY = std::sin(radian) * speed;

    _armatureDisplay = dragonBones::CCFactory::getFactory()->buildArmatureDisplay(armatureName);
    _armatureDisplay->setPosition(position);
    _armatureDisplay->setRotation(-radian * dragonBones::Transform::RAD_DEG);
    _armatureDisplay->getAnimation()->play("idle");

    if (!effectArmatureName.empty())
    {
        _effectDisplay = dragonBones::CCFactory::getFactory()->buildArmatureDisplay(effectArmatureName);
        _effectDisplay->setPosition(_armatureDisplay->getPosition());
        _effectDisplay->setRotation(-radian * dragonBones::Transform::RAD_DEG);
        _effectDisplay->setScaleX(cocos2d::random(1.0f, 2.0f));
        _effectDisplay->setScaleY(cocos2d::random(1.0f, 1.5f));
        if (cocos2d::random(0.0f, 1.0f) < 0.5f)
        {
            _effectDisplay->setScaleY(_effectDisplay->getScaleY());
        }

        _effectDisplay->getAnimation()->play("idle");

        CoreElementGame::instance->addChild(_effectDisplay);
    }

    CoreElementGame::instance->addChild(_armatureDisplay);
}

CoreElementBullet::~CoreElementBullet()
{
}

bool CoreElementBullet::update()
{
    const auto& position = _armatureDisplay->getPosition();
    _armatureDisplay->setPosition(position.x + _speedX, position.y + _speedY);

    if (
        position.x < -CoreElementGame::instance->getStageWidth() * 0.5f - 100.0f || position.x >= CoreElementGame::instance->getStageWidth() * 0.5f + 100.0f ||
        position.y < -CoreElementGame::instance->getStageHeight() * 0.5f - 100.0f || position.y >= CoreElementGame::instance->getStageHeight() * 0.5f + 100.0f
    )
    {
        CoreElementGame::instance->removeChild(_armatureDisplay);
        _armatureDisplay->dispose();

        if (_effectDisplay)
        {
            CoreElementGame::instance->removeChild(_effectDisplay);
            _effectDisplay->dispose();
        }

        return true;
    }

    return false;
}
