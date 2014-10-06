#ifndef DEMO_DRAGON_BOY_H
#define DEMO_DRAGON_BOY_H

#include "Demo.h"

class DemoDragonBoy : public DemoBase
{
public:
    CREATE_FUNC(DemoDragonBoy);
    DemoDragonBoy();
    ~DemoDragonBoy();
    virtual std::string title();
    virtual std::string subtitle();
    virtual void update(float dt) override;

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
    virtual void addInteraction() override;
    
private:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    cocos2d::EventListenerKeyboard *_keyboardListener;
    virtual void keyPressHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    virtual void keyReleaseHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
#endif
    cocos2d::EventListener *_fallEndFadeOutListener;
    
    void eventHandler(cocos2d::EventCustom *event);
    void jump();   
    void squat(bool isSquat);
    void move(int dir, bool isDown);  
    void updateAnimation();
    void changeClothe();
    void updateSpeed();
    void fallEndFadeOutCompleteHandler(cocos2d::EventCustom *event);
};
#endif  // DEMO_DRAGON_BOY_H