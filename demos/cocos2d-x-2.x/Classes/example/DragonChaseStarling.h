#ifndef __DRAGON_CHASE_STARLING__
#define __DRAGON_CHASE_STARLING__

#include "example/DragonBonesDemo.h"

class DragonChaseStarling : public DragonBonesDemo
{
public:
    DragonChaseStarling();
    virtual std::string title();
    virtual void onEnter();
    virtual void update(float dt);

    void menuCallback(CCObject* pSender);
    void moveLeftCallback(CCObject* pSender);
    void moveRightCallback(CCObject* pSender);
    void stopCallback(CCObject* pSender);
    void jumpCallback(CCObject* pSender);
    void changeClothesCallback(CCObject* pSender);

private:
    int _moveDirection;
    float _speedX;
    float _speedY;
    float _footY;
    CCSize _winSize;

    CCDragonBones* _db;
    CCSprite* _starlingBird;

    Bone* _head;
    Bone* _armR;
    Bone* _armL;

    void updateBehavior();

};

#endif // __DRAGON_CHASE_STARLING__