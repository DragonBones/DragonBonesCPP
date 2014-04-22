#ifndef __DRAGON_DEMO_ENTRY__
#define __DRAGON_DEMO_ENTRY__

#include "example/DragonBonesDemo.h"

class DragonDemoEntry : public DragonBonesDemo
{
public:
	DragonDemoEntry();
	virtual std::string title();
	virtual void onEnter();
	virtual void update(float dt);

	void animListener(CCNode*node, void*e);
	void menuCallback(CCObject* pSender);
	void moveLeftCallback(CCObject* pSender);
	void moveRightCallback(CCObject* pSender);
	void stopCallback(CCObject* pSender);
	void jumpCallback(CCObject* pSender);
	void changeClothesCallback(CCObject* pSender);

private:
	std::vector<std::string> _textures;
	size_t _textureIndex;
	int _moveDirection;
	float _speedX;
	float _speedY;
	bool _isJumping;
	float _footY;
	CCSize _winSize;
	CCDragonBones* _db;

	void updateBehavior();
	
};

#endif //__DRAGON_DEMO_ENTRY__