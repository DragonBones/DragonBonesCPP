// Created by zrong(zengrong.net) 2014-04-21

#ifndef __DRAGON_BONES_DEMO__
#define __DRAGON_BONES_DEMO__

#include "cocos2d.h"
#include "VisibleRect.h"
#include "DragonBonesHeaders.h"

USING_NS_CC;
using namespace dragonBones;

class DragonBonesDemo : public CCLayer
{
public:
    static DragonBonesDemo* create(int index);

	DragonBonesDemo(void);
	~DragonBonesDemo(void);

	virtual std::string title();
	virtual std::string subtitle();
	virtual void onEnter();

	void restartCallback(CCObject* pSender);
	void nextCallback(CCObject* pSender);
	void backCallback(CCObject* pSender);
	void menuCloseCallback(CCObject* pSender);
protected:
    std::string    m_strTitle;
};

#endif // __DRAGON_BONES_DEMO__