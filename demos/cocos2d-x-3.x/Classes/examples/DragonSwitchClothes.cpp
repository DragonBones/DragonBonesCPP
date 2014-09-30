#include "DragonSwitchClothes.h"

DragonSwitchClothes::DragonSwitchClothes()
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
	_db->setPosition(VisibleRect::bottom(0, _footY));
	_db->gotoAndPlay("stand");
	this->scheduleUpdate();
}

void DragonSwitchClothes::onEnter()
{
	DragonBonesDemo::onEnter();

	CCMenuItemFont* moveLeftItem = CCMenuItemFont::create("Move Left", this, menu_selector(DragonSwitchClothes::moveLeftCallback));
	moveLeftItem->setAnchorPoint(ccp(0, 0));
	CCMenuItemFont* moveRightItem = CCMenuItemFont::create("Move Right", this, menu_selector(DragonSwitchClothes::moveRightCallback));
	moveRightItem->setAnchorPoint(ccp(0, 0));
	CCMenuItemFont* stopItem = CCMenuItemFont::create("Stop", this, menu_selector(DragonSwitchClothes::stopCallback));
	stopItem->setAnchorPoint(ccp(0, 0));
	CCMenuItemFont* jumpItem = CCMenuItemFont::create("Jump", this, menu_selector(DragonSwitchClothes::jumpCallback));
	jumpItem->setAnchorPoint(ccp(0, 0));
	CCMenuItemFont* changeClothesItem = CCMenuItemFont::create("Change Clothes", this, menu_selector(DragonSwitchClothes::changeClothesCallback));
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

void DragonSwitchClothes::menuCallback(CCObject* pSender)
{
	CCMenuItemFont* menuItem = static_cast<CCMenuItemFont*>(pSender);
	CCLOG("MenuITem getTag:%d", menuItem->getTag());
	CCLOG("MenuItem label:%s", static_cast<CCLabelTTF*>(menuItem->getLabel())->getString());
}

void DragonSwitchClothes::moveLeftCallback(CCObject* pSender)
{
	_moveDirection = -1;
	updateBehavior();
}

void DragonSwitchClothes::moveRightCallback(CCObject* pSender)
{
	_moveDirection = 1;
	updateBehavior();
}

void DragonSwitchClothes::stopCallback(CCObject* pSender)
{
	_moveDirection = 0;
	updateBehavior();
}

void DragonSwitchClothes::jumpCallback(CCObject* pSender)
{
	if (_isJumping)
	{
		return;
	}
	_speedY = 17;
	_isJumping = true;
	_db->gotoAndPlay("jump");
}

void DragonSwitchClothes::changeClothesCallback(CCObject* pSender)
{
	_textureIndex++;
	if (_textureIndex >= _textures.size()) {
		_textureIndex = _textureIndex - _textures.size();
	}

    _db->setBoneTexture("clothes", _textures[_textureIndex].c_str(), "Dragon");
}

void DragonSwitchClothes::update(float dt)
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

void DragonSwitchClothes::updateBehavior()
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

std::string DragonSwitchClothes::title()
{
	return "Dragon Switch Clothes";
}