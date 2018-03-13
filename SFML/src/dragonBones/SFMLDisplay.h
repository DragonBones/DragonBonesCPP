/*
*********************************************************************
* File			: SFMLDisplay.h
* Project		: DragonBonesSFML
* Developers	: Piotr Krupa (piotrkrupa06@gmail.com)
*				: Patryk (PsychoX) Ludwikowski <psychoxivi@gmail.com>
* License   	: MIT License
*********************************************************************
*/

#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

DRAGONBONES_NAMESPACE_BEGIN

class SFMLDisplay
{
public:
	const sf::Texture*			 	texture = nullptr;

	std::vector<std::vector<int>>	verticesInTriagles;

	std::vector<sf::Vertex>		 	verticesDisplay;

	bool							visible = true;

	sf::BlendMode					blendMode;
	
	sf::PrimitiveType				primitiveType = sf::PrimitiveType::TriangleStrip;
private:
	sf::Transform					transform;
	

public:
	SFMLDisplay() = default;
	~SFMLDisplay() = default;

	void setMatrix(const Matrix& matrix, const sf::Vector2f& offset = {0.f, 0.f}, const float& scale = 0.f)
	{
		this->transform = sf::Transform(
			matrix.a * scale,	matrix.c * scale,	offset.x,
			matrix.b * scale,	matrix.d * scale,	offset.y,
			0.f,				0.f,				1.f
		);
	}

	void setColor(const sf::Color& color)
	{
		for (auto& vert : verticesDisplay) 
		{
			vert.color = color;
		}
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (visible) 
		{
			states.blendMode = blendMode;
			states.texture = texture;
			
			states.transform.combine(this->transform);

			target.draw(&verticesDisplay[0], verticesDisplay.size(), primitiveType, states);
		}
	}

	sf::FloatRect getBoundingBox(const sf::Vector2f& position)
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
		matrix.translate(position).combine(this->transform);
		rect = matrix.transformRect(rect);

		return rect;
	}
};

DRAGONBONES_NAMESPACE_END
