#include "DragonDemoEntry.h"

DragonDemoEntry::DragonDemoEntry()
: _footY(100.f)
, _db(NULL)
{
	_db = CCDragonBones::create("dragon/skeleton.xml", "dragon/texture.xml", "Dragon", "Dragon", "");
	this->addChild(_db);
	_db->addEventListener(AnimationEvent::LOOP_COMPLETE, "mykey", this, callfuncND_selector(DragonDemoEntry::animListener));
	_db->addEventListener(AnimationEvent::MOVEMENT_CHANGE, "mykey2", this, callfuncND_selector(DragonDemoEntry::animListener));
	_db->addEventListener(AnimationEvent::START, "mykey3", this, callfuncND_selector(DragonDemoEntry::animListener));
	_db->addEventListener(AnimationEvent::COMPLETE, "mykey4", this, callfuncND_selector(DragonDemoEntry::animListener));
	_db->setPosition(VisibleRect::bottom(0, _footY));
	_db->gotoAndPlay("walk");
	this->scheduleUpdate();
}

void DragonDemoEntry::onEnter()
{
    DragonBonesDemo::onEnter();
    CCMenu *menu = CCMenu::create(
        CCMenuItemFont::create("Stop", this, menu_selector(DragonDemoEntry::stopCallback)),
        CCMenuItemFont::create("Play", this, menu_selector(DragonDemoEntry::playCallback)),
        NULL);
    menu->alignItemsVertically();
    menu->setPosition(VisibleRect::left(100, 0));
    this->addChild(menu);
}

void DragonDemoEntry::stopCallback(CCObject* pSender)
{
    _db->getAnimation()->stop();
}

void DragonDemoEntry::playCallback(CCObject* pSender)
{
    _db->getAnimation()->play();
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