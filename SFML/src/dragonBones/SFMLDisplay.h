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
		for (auto& vert : verticesDisplay) {
			vert.color = color;
		}
	}

	void draw(const sf::Vector2f& pos, sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (visible) {
			states.blendMode = blendMode;
			states.texture = texture;
			states.transform *= sf::Transform(_matrix.a * _textureScale, _matrix.c * _textureScale, _offset.x + pos.x,
											  _matrix.b * _textureScale, _matrix.d * _textureScale, _offset.y + pos.y,
											  0.f, 0.f, 1.f);

			states.transform *= sf::Transform(1.f, 0.f, -origin.x,
											  0.f, 1.f, -origin.y,
											  0.f, 0.f, 1.f);

			target.draw(&verticesDisplay[0], verticesDisplay.size(), primitiveType, states);
		}
	}
};

DRAGONBONES_NAMESPACE_END