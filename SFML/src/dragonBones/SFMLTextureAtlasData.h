/*
*********************************************************************
* File          : SFMLTextureAtlasData.h
* Project		: DragonBonesSFML
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
* License   	: MIT License
*********************************************************************
*/

#pragma once

#include <dragonBones/DragonBonesHeaders.h>
#include <SFML/Graphics/Texture.hpp>

DRAGONBONES_NAMESPACE_BEGIN

class SFMLTextureAtlasData : public TextureAtlasData
{
	BIND_CLASS_TYPE_B(SFMLTextureAtlasData);

private:
	sf::Texture* _renderTexture;

public:
	SFMLTextureAtlasData();
	~SFMLTextureAtlasData();

	virtual TextureData* createTexture() const override;

	sf::Texture* getRenderTexture() const { return _renderTexture; }

	void setRenderTexture(sf::Texture* value);
};

DRAGONBONES_NAMESPACE_END
