#include "example/DragonChaseStarling.h"

DragonChaseStarling::DragonChaseStarling()
: _db(NULL)
, _starlingBird(NULL)
, _head(NULL)
, _armR(NULL)
, _armL(NULL)
, _speedX(0)
, _touchX(0)
, _touchY(0)
, _moveDirection(0)
, _r(0)
, _distance(0)
, _isChasing(false)
{
    _winSize = VisibleRect::getWinSize();

    _db = CCDragonBones::create("dragon/skeleton.xml", "dragon/texture.xml", "Dragon", "Dragon", "");
    this->addChild(_db, 0);
    _db->setPosition(VisibleRect::bottom(0, _footY));
    _db->gotoAndPlay("stand");
    _db->setContentSize(CCSizeMake(360, 400));
    
    //CCLOG("getDisplay %s", typeid(_db->getArmature()->getDisplay()).raw_name());
    //CCLOG("SIZE %f", _db->getDisplayNode()->getContentSize());
    CCLOG("SIZE %f", _db->getContentSize().width);

    _starlingBird = CCSprite::create("starling.png");
    _starlingBird->setPosition(VisibleRect::left(20, 0));
    this->addChild(_starlingBird, 10);

    _head = _db->getArmature()->getBone("head");
    _armR = _db->getArmature()->getBone("armUpperR");
    _armL = _db->getArmature()->getBone("armUpperL");

    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
}

std::string DragonChaseStarling::title()
{
    return "Dragon Chase Starling";
}

std::string DragonChaseStarling::subtitle()
{
    return "Please touch screen.";
}

void DragonChaseStarling::update(float dt)
{
    DragonBonesDemo::update(dt);
    checkDistance();
    updateMove();
    updateBones();
}

bool DragonChaseStarling::ccTouchBegan(CCTouch *pTouche, CCEvent *pEvent)
{
    CCPoint loc = pTouche->getLocation();
    updatePosition(loc);
    if (!_isChasing) 
    {
        _isChasing = true;
        this->scheduleUpdate();
    }
    //CCLOG("ccTouchBegan, %.2f, %.2f", loc.x, loc.y);
    return true;
}

void DragonChaseStarling::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint loc = pTouch->getLocation();
    updatePosition(loc);
    //CCLOG("ccTouchMoved, %.2f, %.2f", loc.x, loc.y);
}


void DragonChaseStarling::updatePosition(CCPoint& point)
{
    _touchX = point.x;
    _touchY = point.y;
    _starlingBird->setPosition(point);
}

void DragonChaseStarling::checkDistance()
{
    _distance = _db->getPositionX() - _touchX;

    if (_distance<150)
    {
        updateBehavior(1);
    }
    else if (_distance>190)
    {
        updateBehavior(-1);
    }
    else
    {
        updateBehavior(0);
    }
}

void DragonChaseStarling::updateBones()
{
    //update the bones' pos or rotation
    _r = M_PI + atan2(_db->getPositionY() + _db->getContentSize().height / 2-_touchY , _touchX - _db->getPositionX());
    if (_r > M_PI)
    {
        _r -= M_PI * 2;
    }
    
    _head->offset.setRotation(_r*0.3);
    _armR->offset.setRotation(_r*0.8);
    _armL->offset.setRotation(_r*1.5);
    //CCLOG("updateBones %.2f", _r*0.2*(180 / M_PI));
    _starlingBird->setRotation(_r*0.2*(180/M_PI));
 
}

void DragonChaseStarling::updateMove()
{
    if (_speedX != 0)
    {
        _db->setPositionX(_db->getPositionX() + _speedX);
        if (_db->getPositionX() < 0)
        {
            _db->setPositionX(0);
        }
        else if (_db->getPositionX() > _winSize.width)
        {
            _db->setPositionX(_winSize.width);
        }
    }
}

void DragonChaseStarling::updateBehavior(int direction)
{
    if (_moveDirection == direction)
    {
        return;
    }
    _moveDirection = direction;

    if (_moveDirection == 0)
    {
        _speedX = 0;
        _db->gotoAndPlay("stand");
    }
    else
    {
        _speedX = 6 * _moveDirection;
        _db->gotoAndPlay("walk");
    }
}