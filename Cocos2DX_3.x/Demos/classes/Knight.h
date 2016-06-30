#ifndef __KNIGHT_H__
#define __KNIGHT_H__

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"

class KnightBullet
{
private:
    float _speedX;
    float _speedY;

    dragonBones::Armature* _armature;
    dragonBones::CCArmatureDisplayContainer* _armatureDisplay;

public:
    KnightBullet(const std::string& armatureName, float radian, float speed, const cocos2d::Vec2& position);
    ~KnightBullet();

    bool update();
};

class Hero
{
public:
    static const int MAX_WEAPON_LEVEL;
    static const float JUMP_SPEED;
    static const float MOVE_SPEED;
    static std::vector<std::string> WEAPON_LIST;

private:
    bool _isJumping;
    bool _isAttacking;
    unsigned _hitCount;
    unsigned _weaponIndex;
    std::string _weaponName;
    std::vector<int> _weaponsLevel;
    int _faceDir;
    int _moveDir;
    float _speedX;
    float _speedY;

    dragonBones::Armature* _armature;
    dragonBones::CCArmatureDisplayContainer* _armatureDisplay;
    dragonBones::Armature* _armArmature;

public:
    Hero();
    ~Hero();

    void update();
    void move(int dir);
    void jump();
    void attack();
    void switchWeapon();
    void upgradeWeapon(int dir);

private:
    void _armEventHandler(cocos2d::EventCustom* event);
    void _fire(const cocos2d::Vec2& firePoint, float radian);
    void _updateAnimation();
    void _updatePosition();
};

class KnightGame : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(KnightGame);

public:
    static const int GROUND;
    static const float G;
    static KnightGame* instance;

public:
    dragonBones::CCFactory factory;

    void addBullet(KnightBullet* bullet);

private:
    bool _left;
    bool _right;
    Hero* _player;
    std::vector<KnightBullet*> _bullets;

    void _enterFrameHandler(float passedTime);
    void _keyBoardPressedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void _keyBoardReleasedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    bool _touchHandler(const cocos2d::Touch* touch, cocos2d::Event* event);
    void _updateMove(int dir) const;
};

#endif // __KNIGHT_H__