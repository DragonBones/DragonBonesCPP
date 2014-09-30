// Created by zrong(zengrong.net) 2014-04-23

#ifndef __DRAGON_CHASE_STARLING__
#define __DRAGON_CHASE_STARLING__

#include "example/DragonBonesDemo.h"

class DragonChaseStarling : public DragonBonesDemo
{
public:
    DragonChaseStarling();
    virtual std::string title();
    virtual std::string subtitle();
    virtual void update(float dt);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

private:
    int _moveDirection;
    float _speedX;
    float _touchX;
    float _touchY;
    float _r;
    float _distance;
    bool _isChasing;

    const int _footY; //= 100;
    CCSize _winSize;

    CCDragonBones* _db;
    CCSprite* _starlingBird;

    Bone* _head;
    Bone* _armR;
    Bone* _armL;

    void updatePosition(CCPoint& point);
    void updateBehavior(int direction);
	void updateMove();
	void updateBones();
	void checkDistance();

};

#endif // __DRAGON_CHASE_STARLING__