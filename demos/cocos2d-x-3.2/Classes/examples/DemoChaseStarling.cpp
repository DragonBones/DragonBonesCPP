#include "DemoChaseStarling.h"

DemoChaseStarling::DemoChaseStarling()
: _armature(nullptr)
, _starlingBird(nullptr)
, _head(nullptr)
, _armR(nullptr)
, _armL(nullptr)
, _speedX(0)
, _touchX(0)
, _touchY(0)
, _moveDirection(0)
, _r(0)
, _distance(0)
, _isChasing(false)
,_footY(100)
{
    _winSize = VisibleRect::getWinSize();
}

DemoChaseStarling::~DemoChaseStarling()
{
    Director::getInstance()->getScheduler()->unscheduleUpdate(this);
    dragonBones::WorldClock::clock.remove(_armature);
    _armature->dispose();

    CC_SAFE_DELETE(_armature);
}

std::string DemoChaseStarling::title()
{
    return "Chase Starling";
}

std::string DemoChaseStarling::subtitle()
{
    return "Please touch the screen.";
}

void DemoChaseStarling::demoInit()
{

    // factory
    dragonBones::DBCCFactory::factory.loadDragonBonesData("armatures/dragon/skeleton.xml", "Dragon");
    dragonBones::DBCCFactory::factory.loadTextureAtlas("armatures/dragon/texture.xml", "Dragon");
    // armature
    _armature = (dragonBones::DBCCArmature*)(dragonBones::DBCCFactory::factory.buildArmature("Dragon"));
    _armature->getCCDisplay()->setPosition(VisibleRect::bottom(0, _footY));
    _armature->getCCDisplay()->setContentSize(Size(360, 400));
    this->addChild(_armature->getCCDisplay());
    // update
    dragonBones::WorldClock::clock.add(_armature);

    _starlingBird = Sprite::create("starling.png");
    _starlingBird->setPosition(VisibleRect::left(20, 0));
    this->addChild(_starlingBird, 10);

    _head = _armature->getBone("head");
    _armR = _armature->getBone("armUpperR");
    _armL = _armature->getBone("armUpperL");

    addInteraction();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}

void DemoChaseStarling::addInteraction()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(DemoChaseStarling::touchBeganListener, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(DemoChaseStarling::touchMovedListener, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void DemoChaseStarling::update(float dt)
{
    checkDistance();
    updateMove();
    updateBones();
    dragonBones::WorldClock::clock.advanceTime(dt);
}

bool DemoChaseStarling::touchBeganListener(Touch *pTouche, Event *pEvent)
{
    Point loc = pTouche->getLocation();
    updatePosition(loc);
    if (!_isChasing) 
    {
        _isChasing = true;
        this->scheduleUpdate();
    }
    CCLOG("%s, %.2f, %.2f", __FUNCTION__, loc.x, loc.y);
    return true;
}

void DemoChaseStarling::touchMovedListener(Touch *pTouch, Event *pEvent)
{
    Point loc = pTouch->getLocation();
    updatePosition(loc);
    CCLOG("%s, %.2f, %.2f", __FUNCTION__, loc.x, loc.y);
}

void DemoChaseStarling::updatePosition(Point& point)
{
    _touchX = point.x;
    _touchY = point.y;
    _starlingBird->setPosition(point);
}

void DemoChaseStarling::checkDistance()
{
    _distance = _armature->getCCDisplay()->getPositionX() - _touchX;

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

void DemoChaseStarling::updateBones()
{
    //update the bones' pos or rotation
    Size csize = _armature->getCCDisplay()->getContentSize();
    _r = M_PI + atan2(_armature->getCCDisplay()->getPositionY() + csize.height / 2 - _touchY, 
        _touchX - _armature->getCCDisplay()->getPositionX());
    if (_r > M_PI)
    {
        _r -= float(M_PI * 2);
    }
    
    _head->offset.setRotation(_r*0.3);
    _armR->offset.setRotation(_r*0.8);
    _armL->offset.setRotation(_r*1.5);
    //CCLOG("updateBones %.2f", _r*0.2*(180 / M_PI));
    _starlingBird->setRotation(_r*0.2*(180/M_PI));
 
}

void DemoChaseStarling::updateMove()
{
    if (_speedX != 0)
    {
        _armature->getCCDisplay()->setPositionX(_armature->getCCDisplay()->getPositionX() + _speedX);
        if (_armature->getCCDisplay()->getPositionX() < 0)
        {
            _armature->getCCDisplay()->setPositionX(0);
        }
        else if (_armature->getCCDisplay()->getPositionX() > _winSize.width)
        {
            _armature->getCCDisplay()->setPositionX(_winSize.width);
        }
    }
}

void DemoChaseStarling::updateBehavior(int direction)
{
    if (_moveDirection == direction)
    {
        return;
    }
    _moveDirection = direction;

    if (_moveDirection == 0)
    {
        _speedX = 0;
        _armature->getAnimation()->gotoAndPlay("stand");
    }
    else
    {
        _speedX = 6 * _moveDirection;
        _armature->getAnimation()->gotoAndPlay("walk");
    }
}