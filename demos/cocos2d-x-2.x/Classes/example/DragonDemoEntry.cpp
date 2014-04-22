#include "DragonDemoEntry.h"

DragonDemoEntry::DragonDemoEntry()
: _moveDirection(0)
, _isJumping(false)
, _speedX(0.f)
, _speedY(0.f)
, _textureIndex(0)
, _footY(100.f)
, _db(NULL)
{
	_textures.push_back("parts/clothes1");
	_textures.push_back("parts/clothes2");
	_textures.push_back("parts/clothes3");
	_textures.push_back("parts/clothes4");

	_winSize = VisibleRect::getWinSize();
	_db = CCDragonBones::create("dragon/skeleton.xml", "dragon/texture.xml", "Dragon", "Dragon", "");
	this->addChild(_db);
	_db->addEventListener(AnimationEvent::LOOP_COMPLETE, "mykey", this, callfuncND_selector(DragonDemoEntry::animListener));
	_db->addEventListener(AnimationEvent::MOVEMENT_CHANGE, "mykey2", this, callfuncND_selector(DragonDemoEntry::animListener));
	_db->addEventListener(AnimationEvent::START, "mykey3", this, callfuncND_selector(DragonDemoEntry::animListener));
	_db->addEventListener(AnimationEvent::COMPLETE, "mykey4", this, callfuncND_selector(DragonDemoEntry::animListener));
	_db->setPosition(VisibleRect::bottom(0, _footY));
	_db->gotoAndPlay("stand");
	this->scheduleUpdate();
}

void DragonDemoEntry::onEnter()
{
	DragonBonesDemo::onEnter();

	CCMenuItemFont* moveLeftItem = CCMenuItemFont::create("Move Left", this, menu_selector(DragonDemoEntry::moveLeftCallback));
	moveLeftItem->setAnchorPoint(ccp(0, 0));
	CCMenuItemFont* moveRightItem = CCMenuItemFont::create("Move Right", this, menu_selector(DragonDemoEntry::moveRightCallback));
	moveRightItem->setAnchorPoint(ccp(0, 0));
	CCMenuItemFont* stopItem = CCMenuItemFont::create("Stop", this, menu_selector(DragonDemoEntry::stopCallback));
	stopItem->setAnchorPoint(ccp(0, 0));
	CCMenuItemFont* jumpItem = CCMenuItemFont::create("Jump", this, menu_selector(DragonDemoEntry::jumpCallback));
	jumpItem->setAnchorPoint(ccp(0, 0));
	CCMenuItemFont* changeClothesItem = CCMenuItemFont::create("Change Clothes", this, menu_selector(DragonDemoEntry::changeClothesCallback));
	changeClothesItem->setAnchorPoint(ccp(0, 0));

	CCMenu *menu = CCMenu::create();
	menu->addChild(moveLeftItem, 0, 0);
	menu->addChild(moveRightItem, 0, 1);
	menu->addChild(stopItem, 0, 2);
	menu->addChild(jumpItem, 0, 3);
	menu->addChild(changeClothesItem, 0, 4);

	menu->setPosition(VisibleRect::left(10, 0));
	menu->alignItemsVertically();

	this->addChild(menu);
}

void DragonDemoEntry::menuCallback(CCObject* pSender)
{
	CCMenuItemFont* menuItem = static_cast<CCMenuItemFont*>(pSender);
	CCLOG("MenuITem getTag:%d", menuItem->getTag());
	CCLOG("MenuItem label:%s", static_cast<CCLabelTTF*>(menuItem->getLabel())->getString());
}

void DragonDemoEntry::moveLeftCallback(CCObject* pSender)
{
	_moveDirection = -1;
	updateBehavior();
}

void DragonDemoEntry::moveRightCallback(CCObject* pSender)
{
	_moveDirection = 1;
	updateBehavior();
}

void DragonDemoEntry::stopCallback(CCObject* pSender)
{
	_moveDirection = 0;
	updateBehavior();
}

void DragonDemoEntry::jumpCallback(CCObject* pSender)
{
	if (_isJumping)
	{
		return;
	}
	_speedY = 17;
	_isJumping = true;
	_db->gotoAndPlay("jump");
}

void DragonDemoEntry::changeClothesCallback(CCObject* pSender)
{
	_textureIndex++;
	if (_textureIndex >= _textures.size()) {
		_textureIndex = _textureIndex - _textures.size();
	}

	Cocos2dxFactory* fac = Cocos2dxFactory::getInstance();
	String textureName = _textures[_textureIndex];
	Object* clothesObj = fac->getTextureDisplay(textureName, "Dragon");

	CCLOG("CLOSE %d", clothesObj);

	Bone* bone = _db->getArmature()->getBone("clothes");
	CocosNode* oldClothesObj = static_cast<CocosNode*>(bone->getDisplay());
	bone->setDisplay(clothesObj);

}

void DragonDemoEntry::update(float dt)
{
	if (_speedX != 0) {
		_db->setPositionX(_db->getPositionX() + _speedX);
		if (_db->getPositionX()< 0) {
			_db->setPositionX(0);
		}
		else if (_db->getPositionX() > _winSize.width) {
			_db->setPositionX(_winSize.width);
		}
	}
	if (_isJumping) {
		if (_speedY <= 0 && _speedY - 1 < 0) {
			_db->gotoAndPlay("fall");
		}
		_speedY -= 1;
	}
	if (_speedY != 0)
	{
		_db->setPositionY(_db->getPositionY() + _speedY);
		if (_db->getPositionY() < _footY)
		{
			_db->setPositionY(_footY);
			_isJumping = false;
			_speedY = 0;
			updateBehavior();
		}
	}
}

void DragonDemoEntry::updateBehavior()
{
	if (_isJumping)
	{
		return;
	}
	if (_moveDirection == 0)
	{
		_speedX = 0;
		_db->gotoAndPlay("stand");
	}
	else
	{
		_speedX = 6 * _moveDirection;
		_db->setScaleX(-_moveDirection);
		_db->gotoAndPlay("walk");
	}
}

void DragonDemoEntry::animListener(CCNode*node, void*e)
{
	dragonBones::Event *_event = (dragonBones::Event *) e;
	CCLOG("Event Triggered:%s", _event->getType().c_str());
}

std::string DragonDemoEntry::title()
{
	return "Dragon Demo Entry";
}