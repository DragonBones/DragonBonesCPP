#ifndef __PRE_DB_H__
#define __PRE_DB_H__

#include "BytesType.h"
#include "Point.h"
#include "ColorTransform.h"
#include "Rectangle.h"

#define DB_SAFE_DELETE(p)            do { if(p) { delete (p); (p) = 0; } } while(0)

//#define USE_QUICK_COCOS2DX_215// uncomment to enable quick-cocos2d-x 2.1.5 support

#ifdef USE_QUICK_COCOS2DX_215
    #define CCNodeRGBA CCNode
    #define _displayedColor m_displayedColor
    #define _displayedOpacity m_displayedOpacity
#endif

#endif // __PRE_DB_H__
