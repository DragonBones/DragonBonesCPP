#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "DemoBase.h"
#include "DBCCArmature.h"
#include "DBCCArmatureNode.h"
#include "cocos2d.h"
#include "2d/CCDrawNode.h"

class HelloWorld : public DemoBase
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

	virtual void demoInit() override;
	virtual void updateHandler(float passTime) override;

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	void armAnimationHandler(cocos2d::EventCustom *event);

	dragonBones::DBCCArmatureNode* createEffect(std::string dragonbones, std::string armature);

	dragonBones::DBCCArmatureNode *_armature;
	cocos2d::DrawNode *drawnode;
	bool _jump2Wait;
	std::string _curAction;

};

#endif // __HELLOWORLD_SCENE_H__
