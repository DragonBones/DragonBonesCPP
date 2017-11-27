/*
*********************************************************************
* File          : SFMLArmatureDisplay.h
* Project		: DragonBonesSFML
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
* License   	: MIT License
*********************************************************************
*/

#pragma once

#include <dragonBones\DragonBonesHeaders.h>

#include <SFML\Graphics\RenderWindow.hpp>

#include "SFMLEventDispatcher.h"

DRAGONBONES_NAMESPACE_BEGIN

class SFMLArmatureDisplay : public IArmatureProxy
{
protected:
	Armature*									_armature;
	SFMLEventDispatcher							_dispatcher;

	sf::Vector2f								_pos;

public:
	SFMLArmatureDisplay();
	~SFMLArmatureDisplay();

	bool hasDBEventListener(const std::string& type) const override { return true; }
	void addDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener) override;
	void removeDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener) override;
	void dispatchDBEvent(const std::string& type, EventObject* value) override;

	void dbInit(Armature* armature) override;
	void dbClear() override;
	void dbUpdate() override;

	void dispose(bool disposeProxy) override;
	
	Armature* getArmature() const override { return _armature; }
	Animation* getAnimation() const override { return _armature->getAnimation(); }

	void setPosition(const sf::Vector2f& pos) { _pos = pos; }
	auto& getPosition() { return _pos; }

	void render(sf::RenderTarget& window, sf::RenderStates states);
};

DRAGONBONES_NAMESPACE_END
