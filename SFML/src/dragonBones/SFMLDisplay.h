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
	std::unique_ptr<SFMLMesh>		meshDisplay;
	std::unique_ptr<sf::Sprite>		spriteDisplay;

	sf::BlendMode					blendMode;
	bool							visible;

	sf::Vector2f					position;

private:
	Matrix							matrix;
	sf::Vector2f					offset;
	float							textureScale;

public:
	SFMLDisplay()
	{
		meshDisplay = nullptr;
		spriteDisplay = nullptr;

		visible = true;
	}

	~SFMLDisplay() = default;

	void setMatrix(const Matrix& matrix, const sf::Vector2f& offset, float textureScale)
	{
		this->matrix = matrix;
		this->offset = offset;
		this->textureScale = textureScale;
	}

	void setColor(const sf::Color& color)
	{
		if (spriteDisplay)
			spriteDisplay->setColor(color);

		if (meshDisplay)
			meshDisplay->setColor(color);
	}

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (visible)
		{
			states.blendMode = blendMode;
			states.transform *= sf::Transform(matrix.a * textureScale, matrix.c * textureScale, (offset.x + position.x) * textureScale,
											  matrix.b * textureScale, matrix.d * textureScale, (offset.y + position.y) * textureScale,
											  0.f, 0.f, 1.f);

			if (spriteDisplay)
				target.draw(*spriteDisplay, states);

			if (meshDisplay)
				target.draw(*meshDisplay, states);
		}
	}
};

DRAGONBONES_NAMESPACE_END