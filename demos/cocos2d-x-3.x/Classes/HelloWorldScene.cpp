#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void HelloWorld::updateHandler(float passTime)
{
	dragonBones::WorldClock::clock.advanceTime(passTime);
	Rect rect = _armature->getBoundingBox();
	Vec2 vec2s[4];
	vec2s[0].x = rect.getMidX();
	vec2s[0].y = rect.getMidY();
	vec2s[1].x = rect.getMidX();
	vec2s[1].y = rect.getMaxY();
	vec2s[2].x = rect.getMaxX();
	vec2s[2].y = rect.getMaxY();
	vec2s[3].x = rect.getMaxX();
	vec2s[3].y = rect.getMidY();

	log("rect = x=%f, y=%f, w=%f, h=%f", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
	log("rect: left=%f, right=%f, top=%f, bottom=%f", rect.getMinX(), rect.getMaxX(), rect.getMaxY(), rect.getMinY());

	drawnode->clear();
	drawnode->drawPolygon(vec2s, 4, Color4F::WHITE, 1, Color4F::RED);

}

// on "init" you need to initialize your instance
void HelloWorld::demoInit()
{
    //////////////////////////////
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);


	// factory
	dragonBones::DBCCFactory::factory.loadDragonBonesData("zhugeliang/zhugeliang_skeleton.xml");
	dragonBones::DBCCFactory::factory.loadTextureAtlas("zhugeliang/zhugeliang.xml");
	// armature
	auto armature = (dragonBones::DBCCArmature *)(dragonBones::DBCCFactory::factory.buildArmature("main", "zhugeliang"));
	_armature = dragonBones::DBCCArmatureNode::create(armature);

	drawnode = DrawNode::create();
	//_armature->addChild(drawnode, -1);
	this->addChild(drawnode);
	
	_armature->getAnimation()->gotoAndPlay("walk");
	_armature->setPosition(480.f, 200.f);
	this->addChild(_armature);
	// armature event
	_armature->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::START, std::bind(&HelloWorld::armAnimationHandler, this, std::placeholders::_1));
	_armature->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::FADE_IN, std::bind(&HelloWorld::armAnimationHandler, this, std::placeholders::_1));
	_armature->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::COMPLETE, std::bind(&HelloWorld::armAnimationHandler, this, std::placeholders::_1));
	_armature->getCCEventDispatcher()->addCustomEventListener(dragonBones::EventData::ANIMATION_FRAME_EVENT, std::bind(&HelloWorld::armAnimationHandler, this, std::placeholders::_1));
	// update
	dragonBones::WorldClock::clock.add(_armature->getArmature());

	// key
	cocos2d::EventListenerKeyboard *listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event){
		log("key pressed code=%d", keyCode);
		switch (keyCode)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_A:
			_armature->getAnimation()->gotoAndPlay("wait");
			_armature->getAnimation()->gotoAndPlay("skill1");
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_S:
			_armature->getAnimation()->gotoAndPlay("wait");
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_D:
			_armature->getAnimation()->gotoAndPlay("atk");
			break;
		default:
			break;
		}
	};
	//listener->onKeyReleased = std::bind(&DemoKnight::keyReleaseHandler, this, std::placeholders::_1, std::placeholders::_2);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::armAnimationHandler(cocos2d::EventCustom *event)
{
	dragonBones::EventData *eventData = (dragonBones::EventData *)(event->getUserData());

	switch (eventData->getType())
	{
	case dragonBones::EventData::EventType::START:
		cocos2d::log("animation start: %s %f", eventData->animationState->name.c_str(), utils::gettime());
		break;
	case dragonBones::EventData::EventType::FADE_IN:
		cocos2d::log("animation fade in: %s %f", eventData->animationState->name.c_str(), utils::gettime());
		break;

	case dragonBones::EventData::EventType::COMPLETE:
		cocos2d::log("animation complete: %s  %f", eventData->animationState->name.c_str(), utils::gettime());
		
		break;

	case dragonBones::EventData::EventType::ANIMATION_FRAME_EVENT:
		cocos2d::log("animation frame event: %s %s %f", eventData->animationState->name.c_str(), eventData->frameLabel, utils::gettime());
		break;
	}
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
