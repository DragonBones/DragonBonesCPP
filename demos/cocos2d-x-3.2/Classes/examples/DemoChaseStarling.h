// Created by zrong(zengrong.net) 2014-10-06

#ifndef DEMO_CHASE_STARLING_H
#define DEMO_CHASE_STARLING_H

#include "Demo.h"
USING_NS_DB;

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

    DBCCArmatureNode *_armatureNode;
    Sprite *_starlingBird;

    Bone *_head;
    Bone *_armR;
    Bone *_armL;

    bool touchBeganListener(Touch *pTouch, Event *pEvent);
    void touchMovedListener(Touch *pTouch, Event *pEvent);

    void updatePosition(Vec2& point);
    void updateBehavior(int direction);
	void updateMove();
	void updateBones();
	void checkDistance();

};

#endif // DEMO_CHASE_STARLING_H