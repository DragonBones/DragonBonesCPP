// Created by zrong(zengrong.net) 2014-10-06

#ifndef DEMO_CHASE_STARLING_H
#define DEMO_CHASE_STARLING_H

#include "Demo.h"

class DemoChaseStarling : public DemoBase
{
public:
    CREATE_FUNC(DemoChaseStarling);
    DemoChaseStarling();
    ~DemoChaseStarling();
    virtual std::string title();
    virtual std::string subtitle();
    virtual void update(float dt) override;

protected:
    virtual void demoInit() override;
    virtual void addInteraction() override;

private:
    int _moveDirection;
    float _speedX;
    float _touchX;
    float _touchY;
    float _r;
    float _distance;
    bool _isChasing;

    const int _footY; //= 100;
    Size _winSize;

    dragonBones::DBCCArmature *_armature;
    Sprite *_starlingBird;

    dragonBones::Bone *_head;
    dragonBones::Bone *_armR;
    dragonBones::Bone *_armL;

    bool touchBeganListener(Touch *pTouch, Event *pEvent);
    void touchMovedListener(Touch *pTouch, Event *pEvent);

    void updatePosition(Point& point);
    void updateBehavior(int direction);
	void updateMove();
	void updateBones();
	void checkDistance();

};

#endif // DEMO_CHASE_STARLING_H