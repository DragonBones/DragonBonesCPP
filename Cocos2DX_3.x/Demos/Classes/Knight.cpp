#include "Knight.h"

USING_NS_CC;

Scene* KnightGame::createScene()
{
    auto scene = Scene::create();
    auto layer = KnightGame::create();

    scene->addChild(layer);
    return scene;
}

const int KnightGame::GROUND = 120.f;
const float KnightGame::G = -0.6f;
KnightGame* KnightGame::instance = nullptr;

bool KnightGame::init()
{
    if (!LayerColor::initWithColor(cocos2d::Color4B(105, 105, 105, 255)))
    {
        return false;
    }

    KnightGame::instance = this;

    _left = false;
    _right = false;
    _player = nullptr;

    // Load DragonBones Data.
    const auto dragonBonesData = factory.loadDragonBonesData("Knight/Knight.json");
    factory.loadTextureAtlasData("Knight/Knight_texture_1.json");

    if (dragonBonesData)
    {
        cocos2d::Director::getInstance()->getScheduler()->schedule(
            schedule_selector(KnightGame::_enterFrameHandler),
            this, 0.f, false
        );

        const auto keyboardListener = cocos2d::EventListenerKeyboard::create();
        keyboardListener->onKeyPressed = CC_CALLBACK_2(KnightGame::_keyBoardPressedHandler, this);
        keyboardListener->onKeyReleased = CC_CALLBACK_2(KnightGame::_keyBoardReleasedHandler, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

        const auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = CC_CALLBACK_2(KnightGame::_touchHandler, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

        _player = new Hero();
    }
    else
    {
        assert(false);
    }

    const auto text = cocos2d::Label::create();
    text->setPosition(480.f, 60.f);
    text->setString("Press W/A/S/D to move. Press SPACE to switch weapen. Press Q/E to upgrade weapen.\nClick to attack.");
    text->setAlignment(cocos2d::TextHAlignment::CENTER);
    this->addChild(text);

    return true;
}

void KnightGame::addBullet(KnightBullet* bullet)
{
    _bullets.push_back(bullet);
}

void KnightGame::_enterFrameHandler(float passedTime)
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

void KnightGame::_keyBoardPressedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
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
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_Q:
        _player->upgradeWeapon(-1);
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_E:
        _player->upgradeWeapon(1);
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
        _player->switchWeapon();
        break;
    }
}

void KnightGame::_keyBoardReleasedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
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
        break;
    }
}

bool KnightGame::_touchHandler(const cocos2d::Touch* touch, cocos2d::Event* event)
{
    _player->attack();
    return true;
}

void KnightGame::_updateMove(int dir) const
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

const int Hero::MAX_WEAPON_LEVEL = 3;
const float Hero::JUMP_SPEED = 20.f;
const float Hero::MOVE_SPEED = 4.f;
std::vector<std::string> Hero::WEAPON_LIST;

Hero::Hero() :
    _isJumping(false),
    _isAttacking(false),
    _hitCount(0),
    _weaponIndex(0),
    _weaponName(""),
    _weaponsLevel(),
    _faceDir(1),
    _moveDir(0),
    _speedX(0.f),
    _speedY(0.f),
    _armature(nullptr),
    _armatureDisplay(nullptr),
    _armArmature(nullptr)
{
    WEAPON_LIST.push_back("sword");
    WEAPON_LIST.push_back("pike");
    WEAPON_LIST.push_back("axe");
    WEAPON_LIST.push_back("bow");

    _weaponName = WEAPON_LIST[_weaponIndex];
    _weaponsLevel.resize(4, 0);

    _armature = KnightGame::instance->factory.buildArmature("knight");
    _armatureDisplay = (dragonBones::CCArmatureDisplay*)_armature->getDisplay();
    _armatureDisplay->setPosition(480.f, KnightGame::GROUND);
    _armatureDisplay->setScale(1.f);

    _armArmature = _armature->getSlot("armOutside")->getChildArmature();
    const auto armArmatureDisplay = (dragonBones::CCArmatureDisplay*)_armArmature->getDisplay();
    armArmatureDisplay->getEventDispatcher()->setEnabled(true);
    armArmatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::COMPLETE, std::bind(&Hero::_armEventHandler, this, std::placeholders::_1));
    armArmatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FRAME_EVENT, std::bind(&Hero::_armEventHandler, this, std::placeholders::_1));

    _updateAnimation();

    dragonBones::WorldClock::clock.add(_armature);
    KnightGame::instance->addChild(_armatureDisplay);
}

Hero::~Hero()
{
}

void Hero::update()
{
    _updatePosition();
}

void Hero::move(int dir)
{
    if (_moveDir == dir)
    {
        return;
    }

    _moveDir = dir;
    if (_moveDir)
    {
        if (_faceDir != _moveDir)
        {
            _faceDir = _moveDir;
            _armatureDisplay->setScaleX(-_armatureDisplay->getScaleX());
        }
    }

    _updateAnimation();
}

void Hero::jump()
{
    if (_isJumping)
    {
        return;
    }

    _isJumping = true;
    _speedY = JUMP_SPEED;
    _armature->getAnimation().fadeIn("jump");
}

void Hero::attack()
{
    if (_isAttacking)
    {
        return;
    }

    _isAttacking = true;
    const auto animationName = "attack_" + _weaponName + "_" + dragonBones::to_string(_hitCount + 1);
    _armArmature->getAnimation().fadeIn(animationName);
}

void Hero::switchWeapon()
{
    _isAttacking = false;
    _hitCount = 0;

    _weaponIndex++;
    if (_weaponIndex >= WEAPON_LIST.size())
    {
        _weaponIndex = 0;
    }

    _weaponName = WEAPON_LIST[_weaponIndex];

    _armArmature->getAnimation().fadeIn("ready_" + _weaponName);
}

void Hero::upgradeWeapon(int dir)
{
    auto weaponLevel = _weaponsLevel[_weaponIndex] + dir;
    weaponLevel %= MAX_WEAPON_LEVEL;
    if (weaponLevel < 0)
    {
        weaponLevel = MAX_WEAPON_LEVEL + weaponLevel;
    }

    _weaponsLevel[_weaponIndex] = weaponLevel;

    // Replace display.
    if (_weaponName == "bow")
    {
        _armArmature->getSlot("bow")->setChildArmature(KnightGame::instance->factory.buildArmature("knightFolder/" + _weaponName + "_" + dragonBones::to_string(weaponLevel + 1)));
    }
    else
    {
        KnightGame::instance->factory.replaceSlotDisplay(
            "", "weapons", "weapon",
            "knightFolder/" + _weaponName + "_" + dragonBones::to_string(weaponLevel + 1),
            *_armArmature->getSlot("weapon")
        );
    }
}

void Hero::_armEventHandler(cocos2d::EventCustom* event)
{
    const auto eventObject = (dragonBones::EventObject*)event->getUserData();

    if (eventObject->type == dragonBones::EventObject::COMPLETE)
    {
        _isAttacking = false;
        _hitCount = 0;
        const auto animationName = "ready_" + _weaponName;
        _armArmature->getAnimation().fadeIn(animationName);
    }
    else if (eventObject->type == dragonBones::EventObject::FRAME_EVENT)
    {
        if (eventObject->name == "ready")
        {
            _isAttacking = false;
            _hitCount++;
        }
        else if (eventObject->name == "fire")
        {
            const auto display = (dragonBones::CCArmatureDisplay*)(eventObject->armature->getDisplay());
            const auto firePointBone = eventObject->armature->getBone("bow");
            const auto transform = display->getNodeToWorldTransform();
            cocos2d::Vec3 localPoint(firePointBone->global.x, -firePointBone->global.y, 0.f);
            cocos2d::Vec2 globalPoint;
            transform.transformPoint(&localPoint);
            globalPoint.set(localPoint.x, localPoint.y);

            auto radian = 0.f;
            if (_faceDir > 0)
            {
                radian = firePointBone->global.getRotation() + display->getRotation() * dragonBones::ANGLE_TO_RADIAN;
            }
            else
            {
                radian = dragonBones::PI - (firePointBone->global.getRotation() + display->getRotation() * dragonBones::ANGLE_TO_RADIAN);
            }

            switch (_weaponsLevel[_weaponIndex])
            {
            case 0:
                _fire(globalPoint, radian);
                break;

            case 1:
                _fire(globalPoint, radian + 3.f * dragonBones::ANGLE_TO_RADIAN);
                _fire(globalPoint, radian - 3.f * dragonBones::ANGLE_TO_RADIAN);
                break;

            case 2:
                _fire(globalPoint, radian + 6.f * dragonBones::ANGLE_TO_RADIAN);
                _fire(globalPoint, radian);
                _fire(globalPoint, radian - 6.f * dragonBones::ANGLE_TO_RADIAN);
                break;
            }
        }
    }
}

void Hero::_fire(const cocos2d::Vec2& firePoint, float radian)
{
    const auto bullet = new KnightBullet("arrow", radian, 20.f, firePoint);
    KnightGame::instance->addBullet(bullet);
}

void Hero::_updateAnimation()
{
    if (_isJumping)
    {
        return;
    }

    if (_moveDir == 0)
    {
        _speedX = 0.f;
        _armature->getAnimation().fadeIn("stand");
    }
    else
    {
        _speedX = MOVE_SPEED * _moveDir;
        _armature->getAnimation().fadeIn("run");
    }
}

void Hero::_updatePosition()
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
        if (_speedY > 0.f && _speedY + KnightGame::G <= 0.f)
        {
            _armature->getAnimation().fadeIn("fall");
        }

        _speedY += KnightGame::G;

        _armatureDisplay->setPosition(position.x, position.y + _speedY);
        if (position.y < KnightGame::GROUND)
        {
            _armatureDisplay->setPosition(position.x, KnightGame::GROUND);
            _isJumping = false;
            _speedY = 0.f;
            _speedX = 0.f;
            _updateAnimation();
        }
    }
}

KnightBullet::KnightBullet(const std::string & armatureName, float radian, float speed, const cocos2d::Vec2& position)
{
    _speedX = std::cos(radian) * speed;
    _speedY = -std::sin(radian) * speed;

    _armature = KnightGame::instance->factory.buildArmature(armatureName);
    _armatureDisplay = (dragonBones::CCArmatureDisplay*)_armature->getDisplay();
    _armatureDisplay->setPosition(position);
    _armatureDisplay->setRotation(radian * dragonBones::RADIAN_TO_ANGLE);
    _armature->getAnimation().play("idle");

    dragonBones::WorldClock::clock.add(_armature);
    KnightGame::instance->addChild(_armatureDisplay);
}

KnightBullet::~KnightBullet()
{
}
bool KnightBullet::update()
{
    const auto& position = _armatureDisplay->getPosition();

    _speedY += KnightGame::G;

    _armatureDisplay->setPosition(position.x + _speedX, position.y + _speedY);
    _armatureDisplay->setRotation(std::atan2(-_speedY, _speedX) * dragonBones::RADIAN_TO_ANGLE);

    if (
        position.x < -100.f || position.x >= 960.f + 100.f ||
        position.y < -100.f || position.y >= 640.f + 100.f
        )
    {
        dragonBones::WorldClock::clock.remove(_armature);
        KnightGame::instance->removeChild(_armatureDisplay);
        _armature->dispose();

        return true;
    }

    return false;
}