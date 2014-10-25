#ifndef __VISIBLERECT_H__
#define __VISIBLERECT_H__

#include "cocos2d.h"

USING_NS_CC;

class VisibleRect
{
public:
    static Rect getVisibleRect();
	static Size getWinSize();

    static Point left();
	static Point left(const float &ox, const float &oy);
    static Point right();
	static Point right(const float &ox, const float &oy);
    static Point top();
	static Point top(const float &ox, const float &oy);
    static Point bottom();
	static Point bottom(const float &ox, const float &oy);
    static Point center();
	static Point center(const float &ox, const float &oy);
    static Point leftTop();
	static Point leftTop(const float &ox, const float &oy);
    static Point rightTop();
	static Point rightTop(const float &ox, const float &oy);
    static Point leftBottom();
	static Point leftBottom(const float &ox, const float &oy);
    static Point rightBottom();
	static Point rightBottom(const float &ox, const float &oy);
private:
    static void lazyInit();
    static Rect s_visibleRect;
	static Size _sSize;
};

#endif /* __VISIBLERECT_H__ */
