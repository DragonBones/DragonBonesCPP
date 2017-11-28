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

class SFMLDisplay
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

	void render(sf::RenderTarget& window, sf::RenderStates states)
	{
		if (visible)
		{
			states.blendMode = blendMode;
			states.transform *= matrix;

			if (_spriteDisplay)
				window.draw(*_spriteDisplay, states);

			if (_meshDisplay)
				_meshDisplay->render(window, states);
		}
	}
};

DRAGONBONES_NAMESPACE_END