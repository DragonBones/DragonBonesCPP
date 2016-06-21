#include "CoreElement.h"

USING_NS_CC;

Scene* CoreElementGame::createScene()
{
    auto scene = Scene::create();
    auto layer = CoreElementGame::create();

    scene->addChild(layer);
    return scene;
}

const int CoreElementGame::GROUND = 120.f;
const float CoreElementGame::G = -0.6f;
CoreElementGame* CoreElementGame::instance = nullptr;

bool CoreElementGame::init()
{
    if (!LayerColor::initWithColor(cocos2d::Color4B(105, 105, 105, 255)))
    {
        return false;
    }

    CoreElementGame::instance = this;

    _left = false;
    _right = false;
    _player = nullptr;

    // Load DragonBones Data.
    const auto dragonBonesData = factory.loadDragonBonesData("CoreElement/CoreElement.json");
    factory.loadTextureAtlasData("CoreElement/CoreElement_texture_1.json");

    if (dragonBonesData)
    {
        cocos2d::Director::getInstance()->getScheduler()->schedule(
            schedule_selector(CoreElementGame::_enterFrameHandler),
            this, 0.f, false
        );

        const auto keyboardListener = cocos2d::EventListenerKeyboard::create();
        keyboardListener->onKeyPressed = CC_CALLBACK_2(CoreElementGame::_keyBoardPressedHandler, this);
        keyboardListener->onKeyReleased = CC_CALLBACK_2(CoreElementGame::_keyBoardReleasedHandler, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

        const auto touchListener = cocos2d::EventListenerMouse::create();
        touchListener->onMouseDown = CC_CALLBACK_1(CoreElementGame::_mouseDownHandler, this);
        touchListener->onMouseUp = CC_CALLBACK_1(CoreElementGame::_mouseUpHandler, this);
        touchListener->onMouseMove = CC_CALLBACK_1(CoreElementGame::_mouseMovedHandler, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

        _player = new Mecha();

        const auto text = cocos2d::Label::create();
        text->setPosition(480.f, 60.f);
        text->setString("Press W/A/S/D to move. Press Q/E/SPACE to switch weapens.\nMove mouse to aim. Click to fire.");
        text->setAlignment(cocos2d::TextHAlignment::CENTER);

        this->addChild(text);
    }
    else
    {
        assert(false);
    }

    return true;
}

void CoreElementGame::addBullet(CoreElementBullet* bullet)
{
    _bullets.push_back(bullet);
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

    dragonBones::WorldClock::clock.advanceTime(passedTime);
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
        _player->switchWeaponR();
        _player->switchWeaponL();
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
    // event->getLocation(); // WTF
    // event->getLocationInView();

    cocos2d::Vec2 target;
    target.x = event->getCursorX();
    target.y = event->getCursorY();
    _player->aim(target);
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

const std::string Mecha::NORMAL_ANIMATION_GROUP = "normal";
const std::string Mecha::AIM_ANIMATION_GROUP = "aim";
const std::string Mecha::ATTACK_ANIMATION_GROUP = "attack";
const float Mecha::JUMP_SPEED = -20.f;
const float Mecha::NORMALIZE_MOVE_SPEED = 3.6f;
const float Mecha::MAX_MOVE_SPEED_FRONT = NORMALIZE_MOVE_SPEED * 1.4f;
const float Mecha::MAX_MOVE_SPEED_BACK = NORMALIZE_MOVE_SPEED * 1.0f;
const char* Mecha::WEAPON_R_LIST[6] = { "weapon_1502b_r", "weapon_1005", "weapon_1005b", "weapon_1005c", "weapon_1005d", "weapon_1005e" };
const char* Mecha::WEAPON_L_LIST[5] = { "weapon_1502b_l", "weapon_1005", "weapon_1005b", "weapon_1005c", "weapon_1005d" };

Mecha::Mecha() :
    _isJumpingA(false),
    _isJumpingB(false),
    _isSquating(false),
    _isAttackingA(false),
    _isAttackingB(false),
    _weaponRIndex(0),
    _weaponLIndex(0),
    _faceDir(1),
    _aimDir(0),
    _moveDir(0),
    _aimRadian(0.f),
    _speedX(0.f),
    _speedY(0.f),
    _armature(nullptr),
    _armatureDisplay(nullptr),
    _weaponR(nullptr),
    _weaponL(nullptr),
    _aimState(nullptr),
    _walkState(nullptr),
    _attackState(nullptr),
    _target()
{
    _armature = CoreElementGame::instance->factory.buildArmature("mecha_1502b");
    _armatureDisplay = (dragonBones::CCArmatureDisplayContainer*)_armature->getDisplay();
    _armatureDisplay->setPosition(480.f, CoreElementGame::GROUND);
    _armatureDisplay->setScale(1.f);
    _armatureDisplay->getEventDispatcher()->setEnabled(true);
    _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FADE_IN_COMPLETE, std::bind(&Mecha::_animationEventHandler, this, std::placeholders::_1));
    _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FADE_OUT_COMPLETE, std::bind(&Mecha::_animationEventHandler, this, std::placeholders::_1));

    _armature->getSlot("effects_1")->displayController = NORMAL_ANIMATION_GROUP;
    _armature->getSlot("effects_2")->displayController = NORMAL_ANIMATION_GROUP;

    _weaponR = _armature->getSlot("weapon_r")->getChildArmature();
    _weaponL = _armature->getSlot("weapon_l")->getChildArmature();
    const auto weaponRDisplay = (dragonBones::CCArmatureDisplayContainer*)_weaponR->getDisplay();
    const auto weaponLDisplay = (dragonBones::CCArmatureDisplayContainer*)_weaponL->getDisplay();
    weaponRDisplay->getEventDispatcher()->setEnabled(true);
    weaponLDisplay->getEventDispatcher()->setEnabled(true);
    weaponRDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FRAME_EVENT, std::bind(&Mecha::_frameEventHandler, this, std::placeholders::_1));
    weaponLDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FRAME_EVENT, std::bind(&Mecha::_frameEventHandler, this, std::placeholders::_1));

    _updateAnimation();

    dragonBones::WorldClock::clock.add(_armature);
    CoreElementGame::instance->addChild(_armatureDisplay);
}

Mecha::~Mecha()
{
}

void Mecha::update()
{
    _updatePosition();
    _updateAim();
    _updateAttack();
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
    _armature->getAnimation().fadeIn("jump_1", -1, -1, 0, NORMAL_ANIMATION_GROUP);
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
    _weaponRIndex++;
    
    if (_weaponRIndex >= sizeof(WEAPON_R_LIST)/ sizeof(WEAPON_R_LIST[0]))
    {
        _weaponRIndex = 0;
    }

    auto weaponDisplay = (dragonBones::CCArmatureDisplayContainer*)_weaponR->getDisplay();
    weaponDisplay->getEventDispatcher()->removeCustomEventListeners(dragonBones::EventObject::FRAME_EVENT);

    const auto weaponName = WEAPON_R_LIST[_weaponRIndex];
    _weaponR = CoreElementGame::instance->factory.buildArmature(weaponName);
    _armature->getSlot("weapon_r")->setChildArmature(_weaponR);
    weaponDisplay = (dragonBones::CCArmatureDisplayContainer*)_weaponR->getDisplay();
    weaponDisplay->getEventDispatcher()->setEnabled(true);
    weaponDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FRAME_EVENT, std::bind(&Mecha::_frameEventHandler, this, std::placeholders::_1));
}

void Mecha::switchWeaponL()
{
    _weaponLIndex++;

    if (_weaponLIndex >= sizeof(WEAPON_L_LIST) / sizeof(WEAPON_L_LIST[0]))
    {
        _weaponLIndex = 0;
    }

    auto weaponDisplay = (dragonBones::CCArmatureDisplayContainer*)_weaponL->getDisplay();
    weaponDisplay->getEventDispatcher()->removeCustomEventListeners(dragonBones::EventObject::FRAME_EVENT);

    const auto weaponName = WEAPON_L_LIST[_weaponLIndex];
    _weaponL = CoreElementGame::instance->factory.buildArmature(weaponName);
    _armature->getSlot("weapon_l")->setChildArmature(_weaponL);
    weaponDisplay = (dragonBones::CCArmatureDisplayContainer*)_weaponL->getDisplay();
    weaponDisplay->getEventDispatcher()->setEnabled(true);
    weaponDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FRAME_EVENT, std::bind(&Mecha::_frameEventHandler, this, std::placeholders::_1));
}

void Mecha::aim(const cocos2d::Vec2& target)
{
    if (_aimDir == 0)
    {
        _aimDir = 10;
    }

    _target = target;
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
            _armature->getAnimation().fadeIn("jump_2", -1.f, -1, 0, NORMAL_ANIMATION_GROUP);
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
}

void Mecha::_frameEventHandler(cocos2d::EventCustom* event)
{
    const auto eventObject = (dragonBones::EventObject*)event->getUserData();
    if (eventObject->name == "onFire")
    {
        const auto display = (dragonBones::CCArmatureDisplayContainer*)eventObject->armature->getDisplay();
        const auto firePointBone = eventObject->armature->getBone("firePoint");
        const auto transform = display->getNodeToWorldTransform();
        cocos2d::Vec3 localPoint(firePointBone->global.x, -firePointBone->global.y, 0.f);
        cocos2d::Vec2 globalPoint;
        transform.transformPoint(&localPoint);
        globalPoint.set(localPoint.x, localPoint.y);

        _fire(globalPoint);
    }
}

void Mecha::_fire(const cocos2d::Vec2& firePoint)
{
    const auto radian = _faceDir < 0 ? dragonBones::PI - _aimRadian : _aimRadian;
    const auto bullet = new CoreElementBullet("bullet_01", "fireEffect_01", radian + cocos2d::random(-0.01f, 0.01f), 40.f, firePoint);
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
        _speedX = 0.f;
        _armature->getAnimation().fadeIn("squat", -1.f, -1, 0, NORMAL_ANIMATION_GROUP);
        _walkState = nullptr;
        return;
    }

    if (_moveDir == 0)
    {
        _speedX = 0.f;
        _armature->getAnimation().fadeIn("idle", -1.f, -1, 0, NORMAL_ANIMATION_GROUP);
        _walkState = nullptr;
    }
    else
    {
        if (!_walkState)
        {
            _walkState = _armature->getAnimation().fadeIn("walk", -1.f, -1, 0, NORMAL_ANIMATION_GROUP);
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
    if (_speedX != 0.f)
    {
        _armatureDisplay->setPosition(position.x + _speedX, position.y);
        if (position.x < 0.f)
        {
            _armatureDisplay->setPosition(0.f, position.y);
        }
        else if (position.x > 960.f)
        {
            _armatureDisplay->setPosition(960.f, position.y);
        }
    }

    if (_speedY != 0.f)
    {
        if (_speedY > 5.f && _speedY + CoreElementGame::G <= 5.f)
        {
            _armature->getAnimation().fadeIn("jump_3", -1, -1, 0, NORMAL_ANIMATION_GROUP);
        }

        _speedY += CoreElementGame::G;

        _armatureDisplay->setPosition(position.x, position.y + _speedY);
        if (position.y < CoreElementGame::GROUND)
        {
            _armatureDisplay->setPosition(position.x, CoreElementGame::GROUND);
            _isJumpingA = false;
            _isJumpingB = false;
            _speedY = 0.f;
            _speedX = 0.f;
            _armature->getAnimation().fadeIn("jump_4", -1, -1, 0, NORMAL_ANIMATION_GROUP);
            if (_isSquating || _moveDir)
            {
                _updateAnimation();
            }
        }
    }
}

void Mecha::_updateAim()
{
    if (_aimDir == 0)
    {
        return;
    }

    const auto& position = _armatureDisplay->getPosition();

    _faceDir = _target.x > position.x ? 1 : -1;
    if (_armatureDisplay->getScaleX() * _faceDir < 0)
    {
        _armatureDisplay->setScaleX(-_armatureDisplay->getScaleX());

        if (_moveDir)
        {
            _updateAnimation();
        }
    }

    const auto aimOffsetY = _armature->getBone("chest")->global.y;

    if (_faceDir > 0)
    {
        _aimRadian = std::atan2(-(_target.y - position.y + aimOffsetY), _target.x - position.x);
    }
    else
    {
        _aimRadian = dragonBones::PI - std::atan2(-(_target.y - position.y + aimOffsetY), _target.x - position.x);
        if (_aimRadian > dragonBones::PI)
        {
            _aimRadian -= dragonBones::PI * 2.f;
        }
    }

    int aimDir = 0;
    if (_aimRadian > 0.f)
    {
        aimDir = -1;
    }
    else
    {
        aimDir = 1;
    }

    if (_aimDir != aimDir)
    {
        _aimDir = aimDir;

        // Animation Mixing.
        if (_aimDir >= 0)
        {
            _aimState = _armature->getAnimation().fadeIn(
                "aimUp", 0.f, 1,
                0, AIM_ANIMATION_GROUP, dragonBones::AnimationFadeOutMode::SameGroup
            );
        }
        else
        {
            _aimState = _armature->getAnimation().fadeIn(
                "aimDown", 0.f, 1,
                0, AIM_ANIMATION_GROUP, dragonBones::AnimationFadeOutMode::SameGroup
            );
        }

        // Add bone Mask.
        //_aimState->addBoneMask("pelvis");
    }

    _aimState->weight = std::abs(_aimRadian / dragonBones::PI * 2.f);

    //_armature->invalidUpdate("pelvis"); // Only Update bone Mask.
    _armature->invalidUpdate();
}

void Mecha::_updateAttack()
{
    if (!_isAttackingA || _isAttackingB)
    {
        return;
    }

    _isAttackingB = true;

    //Animation Mixing.
    _attackState = _armature->getAnimation().fadeIn(
        "attack_01", -1.f, -1,
        0, ATTACK_ANIMATION_GROUP, dragonBones::AnimationFadeOutMode::SameGroup
    );

    _attackState->autoFadeOutTime = _attackState->fadeTotalTime;
    _attackState->addBoneMask("pelvis");
}

CoreElementBullet::CoreElementBullet(const std::string& armatureName, const std::string& effectArmatureName, float radian, float speed, const cocos2d::Vec2& position)
{
    _speedX = std::cos(radian) * speed;
    _speedY = -std::sin(radian) * speed;

    _armature = CoreElementGame::instance->factory.buildArmature(armatureName);
    _armatureDisplay = (dragonBones::CCArmatureDisplayContainer*)_armature->getDisplay();
    _armatureDisplay->setPosition(position);
    _armatureDisplay->setRotation(radian * dragonBones::RADIAN_TO_ANGLE);
    _armature->getAnimation().play("idle");

    if (!effectArmatureName.empty())
    {
        _effect = CoreElementGame::instance->factory.buildArmature(effectArmatureName);
        const auto effectDisplay = (dragonBones::CCArmatureDisplayContainer*)_effect->getDisplay();
        effectDisplay->setPosition(position);
        effectDisplay->setRotation(radian * dragonBones::RADIAN_TO_ANGLE);
        effectDisplay->setScaleX(cocos2d::random(1.f, 2.f));
        effectDisplay->setScaleY(cocos2d::random(1.f, 1.5f));
        if (cocos2d::random(0.f, 1.f) < 0.5f)
        {
            effectDisplay->setScaleY(effectDisplay->getScaleY());
        }

        _effect->getAnimation().play("idle");

        dragonBones::WorldClock::clock.add(_effect);
        CoreElementGame::instance->addChild(effectDisplay);
    }

    dragonBones::WorldClock::clock.add(_armature);
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
        position.x < -100.f || position.x >= 960.f + 100.f ||
        position.y < -100.f || position.y >= 640.f + 100.f
        )
    {
        dragonBones::WorldClock::clock.remove(_armature);
        CoreElementGame::instance->removeChild(_armatureDisplay);
        _armature->dispose();

        if (_effect)
        {
            dragonBones::WorldClock::clock.remove(_effect);
            CoreElementGame::instance->removeChild((dragonBones::CCArmatureDisplayContainer*)_effect->getDisplay());
            _effect->dispose();
        }

        return true;
    }

    return false;
}