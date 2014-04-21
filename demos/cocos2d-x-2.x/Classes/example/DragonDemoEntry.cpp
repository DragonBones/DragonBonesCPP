#include "DragonDemoEntry.h"

DragonDemoEntry::DragonDemoEntry()
{
	CCDragonBones *m_bones = CCDragonBones::create("dragon/skeleton.xml", "dragon/texture.xml", "Dragon", "Dragon", "");
	this->addChild(m_bones);
	m_bones->addEventListener(AnimationEvent::LOOP_COMPLETE, "mykey", this, callfuncND_selector(DragonDemoEntry::animListener));
	m_bones->addEventListener(AnimationEvent::MOVEMENT_CHANGE, "mykey2", this, callfuncND_selector(DragonDemoEntry::animListener));
	m_bones->addEventListener(AnimationEvent::START, "mykey3", this, callfuncND_selector(DragonDemoEntry::animListener));
	m_bones->addEventListener(AnimationEvent::COMPLETE, "mykey4", this, callfuncND_selector(DragonDemoEntry::animListener));
	m_bones->getDisplayNode()->setPosition(VisibleRect::bottom(0, 100));
	m_bones->gotoAndPlay("walk");
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