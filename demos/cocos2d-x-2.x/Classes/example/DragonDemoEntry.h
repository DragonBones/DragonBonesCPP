#ifndef __DRAGON_DEMO_ENTRY__
#define __DRAGON_DEMO_ENTRY__

#include "example/DragonBonesDemo.h"

class DragonDemoEntry : public DragonBonesDemo
{
public:
	DragonDemoEntry();
	virtual std::string title();

	void animListener(CCNode*node, void*e);
};

#endif //__DRAGON_DEMO_ENTRY__