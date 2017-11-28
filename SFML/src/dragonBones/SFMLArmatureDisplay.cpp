/*
*********************************************************************
* File          : SFMLArmatureDisplay.cpp
* Project		: DragonBonesSFML
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
* License   	: MIT License
*********************************************************************
*/

#include "SFMLArmatureDisplay.h"

#include <SFML\Graphics.hpp>

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

void SFMLArmatureDisplay::render(sf::RenderTarget& window, sf::RenderStates states)
{
	auto arr = _armature->getSlots();

	for (auto item : arr)
	{
		if (!item)
			continue;

		auto display = item->getRawDisplay();

		if (!display)
			continue;

		SFMLDisplay* wrapperDisplay = static_cast<SFMLDisplay*>(display);

		wrapperDisplay->render(window, states);
	}
}

DRAGONBONES_NAMESPACE_END
