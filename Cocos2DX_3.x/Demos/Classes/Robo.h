#ifndef __ROBO_DRAGONBONES_H__
#define __ROBO_DRAGONBONES_H__

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"

class Robo : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(Robo);

private:
    dragonBones::DragonBonesData* _dragonBonesData;
    dragonBones::Armature* _armature;
    dragonBones::CCArmatureDisplay* _armatureDisplay;
    dragonBones::CCFactory _factory;

    void _changeArmature();
    void _changeAnimation(int i);
};

#endif // __ROBO_DRAGONBONES_H__
