#ifndef __DEMO_KNIGHT_H__
#define __DEMO_KNIGHT_H__

#include "Demo.h"

class DemoKnight : public DemoBase
{
public:
    CREATE_FUNC(DemoKnight);
    //DemoKnight();
    //~DemoKnight();
    virtual std::string title();
    virtual std::string subtitle();
    
private:
    dragonBones::DBCCArmature *_armature;
    dragonBones::DBCCArmature *_armArmature;
    
    bool _isLeft;
    bool _isRight;
    bool _isJump;
    int _moveDir;
    int _weaponIndex;
    float _speedX;
    float _speedY;
    
    bool _isAttacking;
    bool _isComboAttack;
    int _hitCount;
    
    std::vector<std::string> _weaponList;
    std::vector<int> _weaponLevelList;
    std::vector<cocos2d::Node*> _arrowList;
    
protected:
    virtual void demoInit() override;    
    virtual void updateHandler(float passTime) override;
private:

    inline std::string getWeaponName(const std::string &name, int level)
    {
        char weapon[512];
        sprintf(weapon, "knightFolder/%s_%d", name.c_str(), level);
        CCLOG("getWeaponNAme:%s", weapon);
        return std::string(weapon);
    }

    virtual void keyPressHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);    
    virtual void keyReleaseHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);    
    void jump();    
    void move(int dir, bool isDown);    
    void armReady();    
    void updateAnimation();    
    void changeWeapon();    
    void upgradeWeaponLevel();    
    void attack();    
    void armAnimationHandler(cocos2d::EventCustom *event);    
    void createArrow(float r, const cocos2d::Point &point);    
    void updateArrows();    
    void updateSpeed();
};
#endif  // __DEMO_KNIGHT_H__