// Created by zrong(zengrong.net) 2014-04-21

#ifndef __DRAGON_DEMO_ENTRY__
#define __DRAGON_DEMO_ENTRY__

#include "example/DragonBonesDemo.h"

class DragonDemoEntry : public DragonBonesDemo
{
public:
	DragonDemoEntry();
	virtual std::string title();
    virtual void onEnter();
	void animListener(CCNode*node, void*e);

private:
	float _footY;
	CCDragonBones* _db;

    void playCallback(CCObject* pSender);
    void stopCallback(CCObject* pSender);
	void updateBehavior();
	
};

#endif //__DRAGON_DEMO_ENTRY__