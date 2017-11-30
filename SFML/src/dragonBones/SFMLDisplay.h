/*
*********************************************************************
* File          : SFMLDisplay.h
* Project		: DragonBonesSFML
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
* License   	: MIT License
*********************************************************************
*/

#pragma once

#include <memory>

#include "SFMLMesh.h"

DRAGONBONES_NAMESPACE_BEGIN

class SFMLDisplay : public sf::Drawable
{
public:
	std::unique_ptr<SFMLMesh>		_meshDisplay;
	std::unique_ptr<sf::Sprite>		_spriteDisplay;

	sf::Transform					matrix;
	sf::BlendMode					blendMode;
	bool							visible;

public:
	SFMLDisplay()
	{
		_meshDisplay = nullptr;
		_spriteDisplay = nullptr;

		visible = true;
	}

	~SFMLDisplay() = default;

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (visible)
		{
			states.blendMode = blendMode;
			states.transform *= matrix;

			if (_spriteDisplay)
				target.draw(*_spriteDisplay, states);

			if (_meshDisplay)
				target.draw(*_meshDisplay, states);
		}
	}
};

DRAGONBONES_NAMESPACE_END