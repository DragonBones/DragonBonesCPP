/*
*********************************************************************
* File          : SFMLArmatureDisplay.h
* Project		: DragonBonesSFML
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
* License   	: MIT License
*********************************************************************
*/

#pragma once

#include <dragonBones/DragonBonesHeaders.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "SFMLEventDispatcher.h"

DRAGONBONES_NAMESPACE_BEGIN

class SFMLArmatureDisplay : public IArmatureProxy, public sf::Drawable
{
protected:
	Armature*									_armature;
	SFMLEventDispatcher							_dispatcher;

	sf::Vector2f								_position;

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

	sf::FloatRect getBoundingBox() const;

	void setPosition(const sf::Vector2f& pos) { _position = pos; }
	auto& getPosition() { return _position; }

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

DRAGONBONES_NAMESPACE_END
