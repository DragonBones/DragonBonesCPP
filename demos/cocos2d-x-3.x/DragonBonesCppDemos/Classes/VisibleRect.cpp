#include "VisibleRect.h"

CCRect VisibleRect::s_visibleRect;
CCSize VisibleRect::_sSize;

void VisibleRect::lazyInit()
{
    if (s_visibleRect.size.width == 0.0f && s_visibleRect.size.height == 0.0f)
    {
        GLView* pEGLView = Director::getInstance()->getOpenGLView();
        s_visibleRect.origin = pEGLView->getVisibleOrigin();
        s_visibleRect.size = pEGLView->getVisibleSize();
		_sSize = CCDirector::sharedDirector()->getWinSize();
    }
}

CCRect VisibleRect::getVisibleRect()
{
    lazyInit();
    return CCRectMake(s_visibleRect.origin.x, s_visibleRect.origin.y, s_visibleRect.size.width, s_visibleRect.size.height);
}

CCSize VisibleRect::getWinSize()
{
	lazyInit();
	return CCSizeMake(_sSize.width, _sSize.height);
}

CCPoint VisibleRect::left()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

CCPoint VisibleRect::left(const float &ox, const float &oy)
{
	CCPoint __ccp = left();
	return ccp(__ccp.x + ox, __ccp.y + oy);
}

CCPoint VisibleRect::right()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

CCPoint VisibleRect::right(const float &ox, const float &oy)
{
	CCPoint __ccp = right();
	return ccp(__ccp.x + ox, __ccp.y + oy);
}

CCPoint VisibleRect::top()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height);
}

CCPoint VisibleRect::top(const float &ox, const float &oy)
{
	CCPoint __ccp = top();
	return ccp(__ccp.x + ox, __ccp.y + oy);
}

CCPoint VisibleRect::bottom()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y);
}

CCPoint VisibleRect::bottom(const float &ox, const float &oy)
{
	CCPoint __ccp = bottom();
	return ccp(__ccp.x + ox, __ccp.y + oy);
}

CCPoint VisibleRect::center()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

CCPoint VisibleRect::center(const float &ox, const float &oy)
{
	CCPoint __ccp = center();
	return ccp(__ccp.x + ox, __ccp.y + oy);
}

CCPoint VisibleRect::leftTop()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height);
}

CCPoint VisibleRect::leftTop(const float &ox, const float &oy)
{
	CCPoint __ccp = leftTop();
	return ccp(__ccp.x + ox, __ccp.y + oy);
}

CCPoint VisibleRect::rightTop()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height);
}

CCPoint VisibleRect::rightTop(const float &ox, const float &oy)
{
	CCPoint __ccp = rightTop();
	return ccp(__ccp.x + ox, __ccp.y + oy);
}

CCPoint VisibleRect::leftBottom()
{
    lazyInit();
    return s_visibleRect.origin;
}

CCPoint VisibleRect::leftBottom(const float &ox, const float &oy)
{
	CCPoint __ccp = leftBottom();
	return ccp(__ccp.x + ox, __ccp.y + oy);
}

CCPoint VisibleRect::rightBottom()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y);
}

CCPoint VisibleRect::rightBottom(const float &ox, const float &oy)
{
	CCPoint __ccp = rightBottom();
	return ccp(__ccp.x + ox, __ccp.y + oy);
}
