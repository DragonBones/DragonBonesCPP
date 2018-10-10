/** @file SFMLDisplay.h
 ** @author Piotr Krupa (piotrkrupa06@gmail.com)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi@gmail.com>
 ** @license MIT License 
 **/

#pragma once

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <dragonBones/DragonBonesHeaders.h>

#include "SFMLNode.h"

DRAGONBONES_NAMESPACE_BEGIN

class SFMLDisplay : public SFMLNode
{
public:
	const sf::Texture*			 	texture = nullptr;

	std::vector<std::vector<int>>	verticesInTriagles;

	std::vector<sf::Vertex>		 	verticesDisplay;

	sf::BlendMode					blendMode;

	sf::PrimitiveType				primitiveType = sf::PrimitiveType::TriangleStrip;

protected:

public:
	SFMLDisplay() = default;
	~SFMLDisplay() = default;

public:
	void setColor(const sf::Color& color) override
	{
		for (auto& vert : verticesDisplay)
		{
			vert.color = color;
		}
	}

	sf::FloatRect getBoundingBox() const override
	{
		if (texture == nullptr)
			return sf::FloatRect();

		if (verticesDisplay.empty())
			return sf::FloatRect();

		sf::Vector2f min = verticesDisplay[0].position;
		sf::Vector2f max = verticesDisplay[0].position;

		for (auto& vert : verticesDisplay)
		{
			min.x = std::min(min.x, vert.position.x);
			min.y = std::min(min.y, vert.position.y);
			max.x = std::max(max.x, vert.position.x);
			max.y = std::max(max.y, vert.position.y);
		}

		sf::FloatRect rect(min, max - min);

		sf::Transform matrix;
		matrix.combine(_transform);
		rect = matrix.transformRect(rect);

		return rect;
	}

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		if (_visible)
		{
			states.blendMode = blendMode;
			states.texture = texture;

			states.transform.combine(_transform);

			target.draw(&verticesDisplay[0], verticesDisplay.size(), primitiveType, states);
		}
	}
};

DRAGONBONES_NAMESPACE_END
