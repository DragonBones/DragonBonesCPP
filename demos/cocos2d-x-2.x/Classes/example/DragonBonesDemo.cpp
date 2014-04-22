#include "DragonBonesDemo.h"

#include "example/DragonDemoEntry.h"

static int sceneIdx = -1;

CCLayer* nextDBDemoAction();
CCLayer* backDBDemoAction();
CCLayer* restartDBDemoAction();

typedef CCLayer* (*NEW_DB_DEMO_FUNC)();
#define DBDEMO_CREATE_FUNC(className) \
	static CCLayer* create##className() \
{ return new className(); }

DBDEMO_CREATE_FUNC(DragonDemoEntry);

static NEW_DB_DEMO_FUNC createFunctions[] =
{
	createDragonDemoEntry,
};

#define MAX_LAYER    (sizeof(createFunctions) / sizeof(createFunctions[0]))

CCLayer* nextDBDemoAction()
{
	sceneIdx++;
	sceneIdx = sceneIdx % MAX_LAYER;

	CCLayer* pLayer = (createFunctions[sceneIdx])();
	pLayer->autorelease();

	return pLayer;
}

CCLayer* backDBDemoAction()
{
	sceneIdx--;
	int total = MAX_LAYER;
	if (sceneIdx < 0)
		sceneIdx += total;

	CCLayer* pLayer = (createFunctions[sceneIdx])();
	pLayer->autorelease();

	return pLayer;
}

CCLayer* restartDBDemoAction()
{
	CCLayer* pLayer = (createFunctions[sceneIdx<0 ? 0 : sceneIdx])();
	pLayer->autorelease();

	return pLayer;
}

DragonBonesDemo::DragonBonesDemo(void)
{
}

DragonBonesDemo::~DragonBonesDemo(void)
{
}

std::string DragonBonesDemo::title()
{
	return "No title";
}

std::string DragonBonesDemo::subtitle()
{
	return "";
}

void DragonBonesDemo::onEnter()
{
	CCLayer::onEnter();

	CCLabelTTF* pLabel = CCLabelTTF::create("DragonBonesCPP for cocos2d-x 2.x", "Arial", 28);

	// position the label on the center of the screen
	pLabel->setPosition(VisibleRect::top(0, -20));

	// add the label as a child to this layer
	this->addChild(pLabel, 1);

	CCLabelTTF* label = CCLabelTTF::create(title().c_str(), "Arial", 24);
	addChild(label, 1);
	label->setPosition(VisibleRect::top(0, -50));

	std::string strSubtitle = subtitle();
	if (!strSubtitle.empty())
	{
		CCLabelTTF* l = CCLabelTTF::create(strSubtitle.c_str(), "Thonburi", 16);
		addChild(l, 1);
		l->setPosition(VisibleRect::top(0, -80));
	}

	CCMenuItemImage *item1 = CCMenuItemImage::create("b1.png", "b2.png", this, menu_selector(DragonBonesDemo::backCallback));
	CCMenuItemImage *item2 = CCMenuItemImage::create("r1.png", "r2.png", this, menu_selector(DragonBonesDemo::restartCallback));
	CCMenuItemImage *item3 = CCMenuItemImage::create("f1.png", "f2.png", this, menu_selector(DragonBonesDemo::nextCallback));
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		this,
		menu_selector(DragonBonesDemo::menuCloseCallback));

	CCMenu *menu = CCMenu::create(item1, item2, item3, pCloseItem, NULL);

	menu->setPosition(CCPointZero);
	item1->setPosition(VisibleRect::bottom(-item2->getContentSize().width * 2, item2->getContentSize().height / 2));
	item2->setPosition(VisibleRect::bottom(0, item2->getContentSize().height / 2));
	item3->setPosition(VisibleRect::bottom(item2->getContentSize().width * 2, item2->getContentSize().height / 2));
	pCloseItem->setPosition(VisibleRect::rightBottom(-pCloseItem->getContentSize().width / 2, pCloseItem->getContentSize().height / 2));

	addChild(menu, 1);
}

void DragonBonesDemo::restartCallback(CCObject* pSender)
{
	CCScene* s = new CCScene();
	s->addChild(restartDBDemoAction());

	CCDirector::sharedDirector()->replaceScene(s);
	s->release();
}

void DragonBonesDemo::nextCallback(CCObject* pSender)
{
	CCScene* s = new CCScene();
	s->addChild(nextDBDemoAction());
	CCDirector::sharedDirector()->replaceScene(s);
	s->release();
}

void DragonBonesDemo::backCallback(CCObject* pSender)
{
	CCScene* s = new CCScene();
	s->addChild(backDBDemoAction());
	CCDirector::sharedDirector()->replaceScene(s);
	s->release();
}

void DragonBonesDemo::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}