/*
*********************************************************************
* File          : SFMLTextureAtlasData.cpp
* Project		: DragonBonesSFML
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
* License   	: MIT License
*********************************************************************
*/

#include "SFMLTextureAtlasData.h"

#include <memory>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "SFMLTextureData.h"

DRAGONBONES_NAMESPACE_BEGIN

SFMLTextureAtlasData::SFMLTextureAtlasData()
{
	_onClear();
}

SFMLTextureAtlasData::~SFMLTextureAtlasData()
{
	_onClear();
}

void SFMLTextureAtlasData::setRenderTexture(sf::Texture* value)
{
	if (_renderTexture == value)
	{
		return;
	}

	_renderTexture = value;

	if (_renderTexture != nullptr)
	{
		for (const auto& pair : textures)
		{
			const auto textureData = static_cast<SFMLTextureData*>(pair.second);

			if (textureData->texture == nullptr)
			{
				dragonBones::Rectangle region;
				region.x = textureData->region.x;
				region.y = textureData->region.y;
				region.width = textureData->rotated ? textureData->region.height : textureData->region.width;
				region.height = textureData->rotated ? textureData->region.width : textureData->region.height;

				textureData->texture = _renderTexture;
				textureData->region = std::move(region);
			}
		}
	}
	else
	{
		for (const auto& pair : textures)
		{
			const auto textureData = static_cast<SFMLTextureData*>(pair.second);

			// textureData->Sprite.reset();

			// nothing to release
		}
	}
}

TextureData* SFMLTextureAtlasData::createTexture() const
{
	return BaseObject::borrowObject<SFMLTextureData>();
}

DRAGONBONES_NAMESPACE_END
