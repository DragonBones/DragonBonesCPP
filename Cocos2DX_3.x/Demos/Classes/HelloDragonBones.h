#ifndef __HELLO_DRAGONBONES_H__
#define __HELLO_DRAGONBONES_H__

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"

class HelloDragonBones : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(HelloDragonBones);
};

#endif // __HELLO_DRAGONBONES_H__
