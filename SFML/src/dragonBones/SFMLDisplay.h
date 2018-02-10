/*
*********************************************************************
* File          : SFMLDisplay.h
* Project		: DragonBonesSFML
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
* Thanks to		: Patryk Ludwikowski
* License   	: MIT License
*********************************************************************
*/

#pragma once

#include <memory>

#include <SFML\Graphics.hpp>

DRAGONBONES_NAMESPACE_BEGIN

class SFMLDisplay
{
public:
	const sf::Texture*			 	texture = nullptr;

	std::vector<std::vector<int>>	verticesInTriagles;

	std::vector<sf::Vertex>		 	verticesDisplay;

	bool							visible = true;

	sf::BlendMode					blendMode;

	sf::Vector2f					origin;

	sf::PrimitiveType				primitiveType = sf::PrimitiveType::TriangleStrip;

private:
	Matrix							_matrix;
	sf::Vector2f					_offset;
	float							_textureScale;

public:
	SFMLDisplay() = default;
	~SFMLDisplay() = default;

	void setMatrix(const Matrix& matrix, const sf::Vector2f& offset, float textureScale)
	{
		_matrix = matrix;
		_offset = offset;
		_textureScale = textureScale;
	}

	void setColor(const sf::Color& color)
	{
		for (auto& vert : verticesDisplay) 
		{
			vert.color = color;
		}
	}

	void draw(const sf::Vector2f& pos, sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (visible) 
		{
			states.blendMode = blendMode;
			states.texture = texture;
			transformMatrix(states.transform, pos);

			target.draw(&verticesDisplay[0], verticesDisplay.size(), primitiveType, states);
		}
	}

	sf::FloatRect getBoundingBox(const sf::Vector2f& pos)
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
		transformMatrix(matrix, pos);
		rect = matrix.transformRect(rect);

		return rect;
	}

private:
	void transformMatrix(sf::Transform &matrix, const sf::Vector2f& pos = sf::Vector2f()) const
	{
		matrix *= sf::Transform(_matrix.a * _textureScale, _matrix.c * _textureScale, _offset.x + pos.x,
								_matrix.b * _textureScale, _matrix.d * _textureScale, _offset.y + pos.y,
								0.f, 0.f, 1.f);

		matrix *= sf::Transform(1.f, 0.f, -origin.x,
								0.f, 1.f, -origin.y,
								0.f, 0.f, 1.f);
	}
};

DRAGONBONES_NAMESPACE_END
