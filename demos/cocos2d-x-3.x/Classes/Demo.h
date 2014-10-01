#ifndef __DEMO_BASE_H__
#define __DEMO_BASE_H__

#include "cocos2d.h"
#include "VisibleRect.h"
#include "DBCCRenderHeaders.h"

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
    virtual void updateHandler(float passTime) = 0;
    std::string    m_strTitle;
    
    
};
#endif  // __DEMO_BASE_H__