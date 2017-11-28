/*
*********************************************************************
* File          : SFMLTextureData.h
* Project		: DragonBonesSFML
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
* License   	: MIT License
*********************************************************************
*/

#pragma once

#include <memory>

#include <dragonBones\DragonBonesHeaders.h>

#include <SFML\Graphics\Sprite.hpp>

DRAGONBONES_NAMESPACE_BEGIN

class SFMLTextureData : public TextureData
{
	BIND_CLASS_TYPE_B(SFMLTextureData);

public:
	sf::Texture*			texture;
	sf::IntRect				textureRect;

	std::string				path;

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
		region.width = size.x;
		region.height = size.y;

		texture = value;
		textureRect = { 0, 0, static_cast<int>(size.x), static_cast<int>(size.y) };
	}
};

DRAGONBONES_NAMESPACE_END
