/*
*********************************************************************
* File          : SFMLFactory.h
* Project		: DragonBonesSFML
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
* License   	: MIT License
*********************************************************************
*/

#pragma once

#include <string>
#include <vector>

#include <dragonBones\DragonBonesHeaders.h>

#include <SFML\Graphics\Sprite.hpp>

#include "SFMLArmatureDisplay.h"
#include "SFMLSlot.h"
#include "SFMLTextureData.h"

DRAGONBONES_NAMESPACE_BEGIN

class SFMLFactory : public BaseFactory
{
protected:
	static DragonBones*											_dragonBonesInstance;
	static SFMLFactory*											_factory;

	std::unique_ptr<SFMLEventDispatcher>						_soundEventDispatcher;

	mutable std::vector<std::unique_ptr<SFMLSlot>>				_wrapperSlots;
	mutable std::vector<std::unique_ptr<SFMLTextureData>>		_wrapperTexturesData;

public:
	SFMLFactory();
	~SFMLFactory();

public:
	DragonBonesData* loadDragonBonesData(const std::string& filePath, const std::string& name = "");
	TextureAtlasData* loadTextureAtlasData(const std::string& filePath, sf::Texture *atlasTexture, const std::string& name = "", float scale = 1.0f);
	SFMLArmatureDisplay* buildArmatureDisplay(const std::string& armatureName, const std::string& dragonBonesName = "", const std::string& skinName = "", const std::string& textureAtlasName = "") const;
	sf::Texture* getTextureDisplay(const std::string& textureName, const std::string& dragonBonesName = "") const;

	void addSoundEventListener(const std::function<void(EventObject*)>& listener)
	{
		_soundEventDispatcher->addDBEventListener(EventObject::SOUND_EVENT, listener);
	}

	void update(float lastUpdate);

	std::vector<SFMLTextureData*> getTexturesData(DragonBonesData* dragonBonesData, const std::string& folderPath);
	TextureAtlasData* createTextureAtlasData(std::vector<SFMLTextureData*>& texturesData, DragonBonesData* dragonBonesData);

	static SFMLFactory* get() { return _factory; }

protected:
	TextureAtlasData* _buildTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const override;
	Armature* _buildArmature(const BuildArmaturePackage& dataPackage) const override;
	Slot* _buildSlot(const BuildArmaturePackage& dataPackage, const SlotData* slotData, Armature* armature) const override;
};

DRAGONBONES_NAMESPACE_END
