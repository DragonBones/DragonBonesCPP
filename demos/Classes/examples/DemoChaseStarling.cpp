#include "DemoChaseStarling.h"

USING_NS_DB;

DemoChaseStarling::DemoChaseStarling()
: _armatureNode(nullptr)
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
    _armatureNode = nullptr;
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
    DBCCFactory::getInstance()->loadDragonBonesData("armatures/dragon/skeleton.xml", "Dragon");
    DBCCFactory::getInstance()->loadTextureAtlas("armatures/dragon/texture.xml", "Dragon");
    // armature
    _armatureNode = DBCCFactory::getInstance()->buildArmatureNode("Dragon");
    _armatureNode->setPosition(VisibleRect::bottom(0, _footY));
    _armatureNode->setContentSize(Size(360, 400));
    addChild(_armatureNode);

    _starlingBird = Sprite::create("starling.png");
    _starlingBird->setPosition(VisibleRect::left(20, 0));
    addChild(_starlingBird, 10);

    _head = _armatureNode->getArmature()->getBone("head");
    _armR = _armatureNode->getArmature()->getBone("armUpperR");
    _armL = _armatureNode->getArmature()->getBone("armUpperL");

    addInteraction();
    scheduleUpdate();
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
}

bool DemoChaseStarling::touchBeganListener(Touch *pTouche, Event *pEvent)
{
    auto loc = pTouche->getLocation();
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
    auto loc = pTouch->getLocation();
    updatePosition(loc);
    CCLOG("%s, %.2f, %.2f", __FUNCTION__, loc.x, loc.y);
}

void DemoChaseStarling::updatePosition(Vec2& point)
{
    _touchX = point.x;
    _touchY = point.y;
    _starlingBird->setPosition(point);
}

void DemoChaseStarling::checkDistance()
{
    _distance = _armatureNode->getPositionX() - _touchX;

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
    Size csize = _armatureNode->getContentSize();
    _r = M_PI + atan2(_armatureNode->getPositionY() + csize.height / 2 - _touchY,
        _touchX - _armatureNode->getPositionX());
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
        _armatureNode->setPositionX(_armatureNode->getPositionX() + _speedX);
        if (_armatureNode->getPositionX() < 0)
        {
            _armatureNode->setPositionX(0);
        }
        else if (_armatureNode->getPositionX() > _winSize.width)
        {
            _armatureNode->setPositionX(_winSize.width);
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
        _armatureNode->getAnimation()->gotoAndPlay("stand");
    }
    else
    {
        _speedX = 6 * _moveDirection;
        _armatureNode->getAnimation()->gotoAndPlay("walk");
    }
}