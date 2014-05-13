//
//  CCDragonBones.cpp
//  quickcocos2dx
//
//  Created by Wayne Dimart on 14-4-18.
//  Copyright (c) 2014年 . All rights reserved.
//

#include "CCDragonBonesNode.h"
#include "cocos2d.h"
#include "DragonBonesHeaders.h"
#include "Armature.h"
namespace cocos2d {

	DragonBonesNode*	DragonBonesNode::create(const char* skeletonXMLFile,
		const char* dragonBonesName,
		const char* armatureName)
	{
		return DragonBonesNode::create( skeletonXMLFile,
			"texture.xml",
			dragonBonesName,
			armatureName,
			"");
	}


	DragonBonesNode*	DragonBonesNode::create(const char* skeletonXMLFile,
		const char* textureXMLFile,
		const char* dragonBonesName,
		const char* armatureName,
		const char* animationName)
	{
		DragonBonesNode* pNew = new DragonBonesNode();
		if(pNew && pNew->init())
		{
			dragonBones::Armature*arm = DragonBonesNode::buildArmature( skeletonXMLFile,
				textureXMLFile,
				dragonBonesName,
				armatureName,
				animationName);
			pNew->initWithArmature(arm);
			pNew->autorelease();
			return pNew;
		}
		CC_SAFE_DELETE(pNew);
		return NULL;
	}

	DragonBonesNode* DragonBonesNode::create(dragonBones::Armature*arm)
	{
		DragonBonesNode* pNew = new DragonBonesNode();
		if(pNew && pNew->init())
		{
			pNew->initWithArmature(arm);
			pNew->autorelease();
			return pNew;
		}
		CC_SAFE_DELETE(pNew);
		return NULL;
	}
	void DragonBonesNode::update(float dt)
	{
		if(m_Armature)
			m_Armature->advanceTime(dt);
	}

	Node*  DragonBonesNode::getDisplayNode()
	{
		return static_cast<dragonBones::CocosNode*>(m_Armature->getDisplay())->node;
	}

	dragonBones::Armature*  DragonBonesNode::getArmature()
	{
		return m_Armature;
	}

	void  DragonBonesNode::addEventListener(const std::string &type,const std::string &key, Ref*pObj,SEL_CallFuncND callback)
	{  
		auto f = [this , pObj , callback](dragonBones::Event* event)
		{
			(pObj->*callback)(this , event);   
		};
		m_Armature->addEventListener(type, f, key);
	}
	void DragonBonesNode::addEventListener(const std::string &type, dragonBones::EventDispatcher::Function listener , const std::string &key)
	{
		m_Armature->addEventListener(type, listener, key);
	}


	bool  DragonBonesNode:: hasEventListener(const std::string &type)
	{
		return m_Armature->hasEventListener(type);
	}
	void DragonBonesNode::removeEventListener(const std::string &type, const std::string &key)
	{
		m_Armature->removeEventListener(type,key);
	}
	void DragonBonesNode::dispatchEvent(dragonBones::Event *event)
	{
		m_Armature->dispatchEvent(event);
	}


	void DragonBonesNode::gotoAndPlay(
		const std::string &animationName,
		float fadeInTime ,
		float duration ,
		float loop ,
		unsigned int layer ,
		const std::string &group ,
		const std::string &fadeOutMode ,
		bool displayControl ,
		bool pauseFadeOut ,
		bool pauseFadeIn
		)
	{
		m_Armature->getAnimation()->gotoAndPlay(
			animationName,
			fadeInTime ,
			duration ,
			loop ,
			layer ,
			group ,
			fadeOutMode ,
			displayControl ,
			pauseFadeOut ,
			pauseFadeIn
			);
	}


	void DragonBonesNode::onExit()
	{
		DB_SAFE_DELETE(m_Armature);
		this->unscheduleAllSelectors();
		Node::onExit();
	}

	dragonBones::Armature*   DragonBonesNode::buildArmature(const char* skeletonXMLFile,
		const char* textureXMLFile,
		const char* dragonBonesName,
		const char* armatureName,
		const char* animationName)
	{
		dragonBones::Cocos2dxFactory *fac = dragonBones::Cocos2dxFactory::getInstance();
		fac->loadSkeletonFile(skeletonXMLFile);
		fac->loadTextureAtlasFile(textureXMLFile);
		return fac->buildArmature(armatureName, animationName ,dragonBonesName);
	}

	void DragonBonesNode::initWithArmature(dragonBones::Armature*arm)
	{
		this->m_Armature = arm;
		this->schedule(schedule_selector(DragonBonesNode::update), 0);
		this->addChild(static_cast<dragonBones::CocosNode*>(m_Armature->getDisplay())->node);
	}
}