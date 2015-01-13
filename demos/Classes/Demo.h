#ifndef DEMO_BASE_H
#define DEMO_BASE_H

#include "cocos2d.h"
#include "VisibleRect.h"
#include "DBCCRenderHeaders.h"

#include "2d/CCLayer.h"

USING_NS_CC;

class DemoBase : public cocos2d::LayerColor
{
public:
    static DemoBase* create(int index);

    virtual bool init();
    virtual std::string title();
    virtual std::string subtitle();
    virtual void onEnter();
    
    void restartCallback(cocos2d::Ref* pSender);
    void nextCallback(cocos2d::Ref* pSender);
    void backCallback(cocos2d::Ref* pSender);
    void menuCloseCallback(cocos2d::Ref* pSender);
    
protected:
    virtual void demoInit() = 0;
    virtual void addInteraction() = 0;
};
#endif  // DEMO_BASE_H