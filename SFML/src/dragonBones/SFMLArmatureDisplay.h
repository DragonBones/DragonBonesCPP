#pragma once

/** @file SFMLArmatureDisplay.h
 ** @author Piotr Krupa (piotrkrupa06@gmail.com)
 ** @license MIT License
 **/

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <dragonBones/DragonBonesHeaders.h>

DRAGONBONES_NAMESPACE_BEGIN

class SFMLArmatureProxy;
class SFMLEventDispatcher;

class SFMLArmatureDisplay : public sf::Drawable
{
private:
	SFMLArmatureProxy* _proxy = nullptr;

	sf::Vector2f _pos;

public:
	SFMLArmatureDisplay(const std::string& armatureName, const std::string& dragonBonesName = "", const std::string& skinName = "", const std::string& textureAtlasName = "");
	~SFMLArmatureDisplay();

	void setPosition(const sf::Vector2f& pos) { _pos = pos; }
	const sf::Vector2f& getPosition() { return _pos; }

	Armature* getArmature() const;
	Animation* getAnimation() const;

	SFMLEventDispatcher* getEventDispatcher();

	SFMLArmatureProxy* getArmatureProxy() const;

	sf::FloatRect getBoundingBox();

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
};

DRAGONBONES_NAMESPACE_END
