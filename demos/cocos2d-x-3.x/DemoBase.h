#ifndef __DEMO_BASE_H__
#define __DEMO_BASE_H__

#include "cocos2d.h"
#include "DBCCRenderHeaders.h"

class DemoBase : public cocos2d::LayerColor
{
public:
    virtual bool init()
    {
        if (!cocos2d::LayerColor::initWithColor(cocos2d::Color4B(128, 128, 128, 255)))
        {
            return false;
        }
        demoInit();
        cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(DemoBase::updateHandler), this, 0, false);
        return true;
    }
    
protected:
    virtual void demoInit() = 0;
    virtual void updateHandler(float passTime) = 0;
    
    
};
#endif  // __DEMO_BASE_H__