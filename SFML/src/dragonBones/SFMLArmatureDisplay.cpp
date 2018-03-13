/*
*********************************************************************
* File          : SFMLArmatureDisplay.cpp
* Project		: DragonBonesSFML
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
* License   	: MIT License
*********************************************************************
*/

#include "SFMLArmatureDisplay.h"

#include <SFML/Graphics.hpp>

#include "SFMLSlot.h"
#include "SFMLDisplay.h"

DRAGONBONES_NAMESPACE_BEGIN

SFMLArmatureDisplay::SFMLArmatureDisplay()
{
	_armature = nullptr;
}

SFMLArmatureDisplay::~SFMLArmatureDisplay()
{
	_armature = nullptr;
}

void SFMLArmatureDisplay::dbInit(Armature* armature)
{
	_armature = armature;
}

void SFMLArmatureDisplay::dbClear()
{
	_armature = nullptr;
}

void SFMLArmatureDisplay::dbUpdate()
{
}

void SFMLArmatureDisplay::addDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener)
{
	_dispatcher.addDBEventListener(type, listener);
}

void SFMLArmatureDisplay::removeDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener)
{
}

void SFMLArmatureDisplay::dispatchDBEvent(const std::string& type, EventObject* value)
{
	_dispatcher.dispatchDBEvent(type, value);
}

void SFMLArmatureDisplay::dispose(bool disposeProxy)
{
	if (_armature)
	{
		delete _armature;
		_armature = nullptr;
	}
}

sf::FloatRect SFMLArmatureDisplay::getBoundingBox() const
{
	auto slots = _armature->getSlots();
	bool isFirst = true;

	sf::Vector2f min;
	sf::Vector2f max;

	for (const auto slot : _armature->getSlots())
	{
		if (!slot->getVisible() || !slot->getDisplay())
		{
			continue;
		}

		auto display = static_cast<SFMLDisplay*>(slot->getRawDisplay());
		const auto bounds = display->getBoundingBox(_position);
		if (isFirst)
		{
			isFirst = false;
			min = { bounds.left, bounds.top };
			max = { bounds.left + bounds.width, bounds.top + bounds.height };
		}
		else
		{
			min.x = std::min(min.x, bounds.left);
			min.y = std::min(min.y, bounds.top);
			max.x = std::max(max.x, bounds.left + bounds.width);
			max.y = std::max(max.y, bounds.top + bounds.height);
		}
	}

	return sf::FloatRect(min, max - min);
}

void SFMLArmatureDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform.translate(this->_position);
	
	for (auto slot : _armature->getSlots())
	{
		if (!slot)
			continue;

		auto display = static_cast<SFMLDisplay*>(slot->getRawDisplay());

		if (!display)
			continue;

		display->draw(target, states);
	}
}

DRAGONBONES_NAMESPACE_END
