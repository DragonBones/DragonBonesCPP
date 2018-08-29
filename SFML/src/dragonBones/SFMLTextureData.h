/** @file SFMLTextureData.h
 ** @author Piotr Krupa (piotrkrupa06@gmail.com)
 ** @license MIT License
 **/

#pragma once

#include <memory>

#include <dragonBones/DragonBonesHeaders.h>

#include <SFML/Graphics/Sprite.hpp>

DRAGONBONES_NAMESPACE_BEGIN

class SFMLTextureData : public TextureData
{
	BIND_CLASS_TYPE_B(SFMLTextureData);

public:
	sf::Texture*			texture;

public:
	SFMLTextureData()
	{
		_onClear();
	}

	virtual ~SFMLTextureData()
	{
		_onClear();
	}

	void _onClear() override
	{
		texture = nullptr;

		TextureData::_onClear();
	}

	void setTexture(sf::Texture *value)
	{
		auto size = value->getSize();

		region.x = 0.f;
		region.y = 0.f;
		region.width = static_cast<float>(size.x);
		region.height = static_cast<float>(size.y);

		texture = value;
	}
};

DRAGONBONES_NAMESPACE_END
