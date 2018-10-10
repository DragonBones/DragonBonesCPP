#pragma once

/** @file SFMLArmatureProxy.hp
 ** @author Piotr Krupa (piotrkrupa06@gmail.com)
 ** @license MIT License
 **/

#include <vector>

#include <dragonBones/DragonBonesHeaders.h>
#include <SFML/Graphics/Rect.hpp>

#include "SFMLNode.h"
#include "SFMLFactory.h"
#include "SFMLEventDispatcher.h"

DRAGONBONES_NAMESPACE_BEGIN

class SFMLDisplay;
class SFMLNode;

class SFMLArmatureProxy : public SFMLNode, public IArmatureProxy
{
	friend SFMLFactory;

protected:
	Armature*				_armature = nullptr;
	SFMLEventDispatcher		_dispatcher;

	sf::Vector2f			_position;

	std::vector<SFMLNode*>	_nodes;

protected:
	SFMLArmatureProxy() = default;
	~SFMLArmatureProxy() = default;

public:
	bool hasDBEventListener(const std::string& type) const override { return true; }
	void addDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener) override;
	void removeDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener) override;
	void dispatchDBEvent(const std::string& type, EventObject* value) override;

	SFMLEventDispatcher* getEventDispatcher() { return &_dispatcher; }

	void addNode(SFMLNode* node);
	void removeNode(SFMLNode* node);
	void sortNodes();

	void dbInit(Armature* armature) override;
	void dbClear() override;
	void dbUpdate() override;

	void dispose(bool disposeProxy) override;

	Armature* getArmature() const override { return _armature; }
	Animation* getAnimation() const override { return _armature->getAnimation(); }

	void setVisible(bool visible) override;
	void setColor(const sf::Color& color) override;

	sf::FloatRect getBoundingBox() const override;

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};

DRAGONBONES_NAMESPACE_END
