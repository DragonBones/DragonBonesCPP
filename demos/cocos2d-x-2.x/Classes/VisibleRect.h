#ifndef __VISIBLERECT_H__
#define __VISIBLERECT_H__

#include "cocos2d.h"

USING_NS_CC;

class VisibleRect
{
public:
    static CCRect getVisibleRect();
	static CCSize getWinSize();

    static CCPoint left();
	static CCPoint left(const float &ox, const float &oy);
    static CCPoint right();
	static CCPoint right(const float &ox, const float &oy);
    static CCPoint top();
	static CCPoint top(const float &ox, const float &oy);
    static CCPoint bottom();
	static CCPoint bottom(const float &ox, const float &oy);
    static CCPoint center();
	static CCPoint center(const float &ox, const float &oy);
    static CCPoint leftTop();
	static CCPoint leftTop(const float &ox, const float &oy);
    static CCPoint rightTop();
	static CCPoint rightTop(const float &ox, const float &oy);
    static CCPoint leftBottom();
	static CCPoint leftBottom(const float &ox, const float &oy);
    static CCPoint rightBottom();
	static CCPoint rightBottom(const float &ox, const float &oy);
private:
    static void lazyInit();
    static CCRect s_visibleRect;
	static CCSize _sSize;
};

#endif /* __VISIBLERECT_H__ */
