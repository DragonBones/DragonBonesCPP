/*
*********************************************************************
* File          : SFMLMesh.h
* Project		: DragonBonesSFML
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
* License   	: MIT License
*********************************************************************
*/

#pragma once

#include <SFML\Graphics.hpp>

DRAGONBONES_NAMESPACE_BEGIN

class SFMLMesh
{
public:
	const sf::Texture*									texture;

	std::vector<std::vector<int>> 						verticesInTriagles;

	std::vector<sf::Vertex>								verticesDisplay;

public:
	SFMLMesh()
	{
		texture = nullptr;
	}

	~SFMLMesh() = default;

	void render(sf::RenderTarget& target, sf::RenderStates states)
	{
		if (texture != nullptr && verticesDisplay.size() > 0)
		{
			states.texture = texture;

			target.draw(&verticesDisplay[0], verticesDisplay.size(), sf::PrimitiveType::Triangles, states);
		}
	}
};

DRAGONBONES_NAMESPACE_END
