/** @file SFMLNode.h
 ** @author Piotr Krupa (piotrkrupa06@gmail.com)
 ** @license MIT License
 **/

#pragma once

#include <dragonBones/DragonBonesHeaders.h>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

DRAGONBONES_NAMESPACE_BEGIN

class SFMLNode : public sf::Drawable
{
protected:
	sf::Transform _transform;

	bool _visible = true;

	int _zOffset = 0;

public:
	SFMLNode() = default;
	virtual ~SFMLNode() = default;

	void setZOffset(int zOffset) { _zOffset = zOffset; }
	int getZOffset() const { return _zOffset; }

	void setMatrix(const Matrix& matrix, const sf::Vector2f& offset, float scaleX, float scaleY)
	{
		_transform = sf::Transform(
			matrix.a * scaleX, matrix.c * scaleY, offset.x,
			matrix.b * scaleX, matrix.d * scaleY, offset.y,
			0.f, 0.f, 1.f
		);
	}

	virtual void setVisible(bool visible) { _visible = visible; }

	virtual void setColor(const sf::Color& color) = 0;

	virtual sf::FloatRect getBoundingBox() const = 0;
};

DRAGONBONES_NAMESPACE_END
