//
//  DemoBase.cpp
//  DragonBonesCPPDemos
//
//  Created by zrong on 9/30/14.
//
//

#include "DemoBase.h"

#include "DemoDragonBoy.h"
#include "DemoKnight.h"
//#include "example/DragonDemoEntry.h"
//#include "example/DragonSwitchClothes.h"
//#include "example/DragonChaseStarling.h"

static int sceneIdx = 0;

DemoBase* nextDBDemoAction();
DemoBase* backDBDemoAction();
DemoBase* restartDBDemoAction();

typedef DemoBase* (*NEW_DB_DEMO_FUNC)();
#define DBDEMO_CREATE_FUNC(__TYPE__) \
static DemoBase* create##__TYPE__() \
{ \
    DemoBase *pRet = new __TYPE__(); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
        else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } \
}

DBDEMO_CREATE_FUNC(DemoDragonBoy);
DBDEMO_CREATE_FUNC(DemoKnight);
//DBDEMO_CREATE_FUNC(DragonDemoEntry);
//DBDEMO_CREATE_FUNC(DragonSwitchClothes);
//DBDEMO_CREATE_FUNC(DragonChaseStarling);

static NEW_DB_DEMO_FUNC createFunctions[] =
{
    createDemoDragonBoy,
    createDemoKnight,
//    createDragonDemoEntry,
//    createDragonSwitchClothes,
//    createDragonChaseStarling,
};

#define MAX_LAYER    (sizeof(createFunctions) / sizeof(createFunctions[0]))

DemoBase* nextDBDemoAction()
{
    sceneIdx++;
    sceneIdx = sceneIdx % MAX_LAYER;
    
    DemoBase* pLayer = (createFunctions[sceneIdx])();
    return pLayer;
}

DemoBase* backDBDemoAction()
{
    sceneIdx--;
    int total = MAX_LAYER;
    if (sceneIdx < 0)
        sceneIdx += total;
    
    DemoBase* pLayer = (createFunctions[sceneIdx])();
    return pLayer;
}

DemoBase* restartDBDemoAction()
{
    DemoBase* pLayer = (createFunctions[sceneIdx<0 ? 0 : sceneIdx])();
    return pLayer;
}

DemoBase* DemoBase::create(int index)
{
    DemoBase* pLayer = (createFunctions[index])();
    sceneIdx = index;
    return pLayer;
}

std::string DemoBase::title()
{
    return "No title";
}

std::string DemoBase::subtitle()
{
    return "";
}

bool DemoBase::init()
{
    if (!LayerColor::initWithColor(cocos2d::Color4B(128, 128, 128, 255)))
    {
        return false;
    }
    
    demoInit();
    cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(DemoBase::updateHandler), this, 0, false);
    return true;
}

void DemoBase::onEnter()
{
    LayerColor::onEnter();
    
    Label* pLabel = Label::createWithSystemFont("DragonBonesCPP for cocos2d-x 3.2", "Arial", 28);
    
    // position the label on the center of the screen
    pLabel->setPosition(VisibleRect::top(0, -20));
    
    // add the label as a child to this layer
    this->addChild(pLabel, 1);
    
    Label* label = Label::createWithSystemFont(title().c_str(), "Arial", 24);
    addChild(label, 1);
    label->setPosition(VisibleRect::top(0, -50));
    
    std::string strSubtitle = subtitle();
    if (!strSubtitle.empty())
    {
        Label* l = Label::createWithSystemFont(strSubtitle.c_str(), "Thonburi", 20);
        addChild(l, 1);
        l->setPosition(VisibleRect::top(0, -80));
    }
    
    MenuItemImage *item1 = MenuItemImage::create("b1.png", "b2.png", CC_CALLBACK_1(DemoBase::backCallback, this));
    MenuItemImage *item2 = MenuItemImage::create("r1.png", "r2.png", CC_CALLBACK_1(DemoBase::restartCallback, this));
    MenuItemImage *item3 = MenuItemImage::create("f1.png", "f2.png", CC_CALLBACK_1(DemoBase::nextCallback, this));
    MenuItemImage *pCloseItem = MenuItemImage::create("CloseNormal.png",
                                                       "CloseSelected.png",
                                                       CC_CALLBACK_1(DemoBase::menuCloseCallback, this));
    
    Menu *menu = Menu::create(item1, item2, item3, pCloseItem, NULL);
    
    menu->setPosition(Point());
    item1->setPosition(VisibleRect::bottom(-item2->getContentSize().width * 2, item2->getContentSize().height / 2));
    item2->setPosition(VisibleRect::bottom(0, item2->getContentSize().height / 2));
    item3->setPosition(VisibleRect::bottom(item2->getContentSize().width * 2, item2->getContentSize().height / 2));
    pCloseItem->setPosition(VisibleRect::rightBottom(-pCloseItem->getContentSize().width / 2, pCloseItem->getContentSize().height / 2));
    
    addChild(menu, 1);
}

void DemoBase::restartCallback(Ref* pSender)
{
    Scene* s = Scene::create();
    s->addChild(restartDBDemoAction());
    Director::getInstance()->replaceScene(s);
}

void DemoBase::nextCallback(Ref* pSender)
{
    Scene* s = Scene::create();
    s->addChild(nextDBDemoAction());
    Director::getInstance()->replaceScene(s);
}

void DemoBase::backCallback(Ref* pSender)
{
    Scene* s = Scene::create();
    s->addChild(backDBDemoAction());
    Director::getInstance()->replaceScene(s);
}

void DemoBase::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
    Director::getInstance()->end();
    exit(0);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}