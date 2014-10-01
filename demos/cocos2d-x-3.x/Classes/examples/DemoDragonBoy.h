#ifndef __DEMO_DRAGON_BOY_H__
#define __DEMO_DRAGON_BOY_H__

#include "Demo.h"

class DemoDragonBoy : public DemoBase
{
public:
    virtual std::string title();
    virtual std::string subtitle();
private:
    dragonBones::DBCCArmature *_armature;
    
    bool _isLeft;
    bool _isRight;
    bool _isJump;
    bool _isSquat;
    int _moveDir;
    int _currentClotheIndex;
    float _speedX;
    float _speedY;
    
    std::vector<std::string> _clothesList;
    
protected:
    virtual void demoInit() override;    
    virtual void updateHandler(float passTime) override;
    
private:
    cocos2d::EventListener *_fallEndFadeOutListener;
    virtual void keyPressHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    virtual void keyReleaseHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    
    void eventHandler(cocos2d::EventCustom *event);
    void jump();   
    void squat(bool isSquat);
    void move(int dir, bool isDown);  
    void updateAnimation();
    void changeClothe();
    void updateSpeed();
    void fallEndFadeOutCompleteHandler(cocos2d::EventCustom *event);
};
#endif  // __DEMO_DRAGON_BOY_H__