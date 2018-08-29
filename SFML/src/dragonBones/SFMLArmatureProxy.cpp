/** @file SFMLArmatureProxy.cpp
 ** @author Piotr Krupa (piotrkrupa06@gmail.com)
 ** @license MIT License
 **/

#include "SFMLArmatureProxy.h"

#include <SFML/Graphics.hpp>

#include "SFMLSlot.h"
#include "SFMLDisplay.h"

DRAGONBONES_NAMESPACE_BEGIN

void SFMLArmatureProxy::dbInit(Armature* armature)
{
	_armature = armature;
}

void SFMLArmatureProxy::dbClear()
{
	_armature = nullptr;
}

void SFMLArmatureProxy::dbUpdate()
{
}

void SFMLArmatureProxy::addDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener)
{
	_dispatcher.addDBEventListener(type, listener);
}

void SFMLArmatureProxy::removeDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener)
{
}

void SFMLArmatureProxy::dispatchDBEvent(const std::string& type, EventObject* value)
{
	_dispatcher.dispatchDBEvent(type, value);
}

void SFMLArmatureProxy::addNode(SFMLNode* node)
{
	_nodes.push_back(node);
}

void SFMLArmatureProxy::removeNode(SFMLNode* node)
{
	auto it = std::find_if(_nodes.begin(), _nodes.end(), [=] (SFMLNode* _node) { return node == _node; });

	if (it != _nodes.end())
	{
		_nodes.erase(it);
	}
}

void SFMLArmatureProxy::sortNodes()
{
	std::sort(_nodes.begin(), _nodes.end(), [&] (SFMLNode* a, SFMLNode* b) {
		return (a->getZOffset() < b->getZOffset());
	});
}

void SFMLArmatureProxy::dispose(bool disposeProxy)
{
	if (_armature)
	{
		_armature->dispose();
		_armature = nullptr;
	}
}

sf::FloatRect SFMLArmatureProxy::getBoundingBox() const
{
	auto slots = _armature->getSlots();
	bool isFirst = true;

	sf::Vector2f min;
	sf::Vector2f max;

	for (const auto slot : _armature->getSlots())
	{
		if (!slot->getVisible() || !slot->getDisplay())
		{
			continue;
		}

		auto display = static_cast<SFMLDisplay*>(slot->getRawDisplay());
		const auto bounds = display->getBoundingBox();
		if (isFirst)
		{
			isFirst = false;
			min = { bounds.left, bounds.top };
			max = { bounds.left + bounds.width, bounds.top + bounds.height };
		}
		else
		{
			min.x = std::min(min.x, bounds.left);
			min.y = std::min(min.y, bounds.top);
			max.x = std::max(max.x, bounds.left + bounds.width);
			max.y = std::max(max.y, bounds.top + bounds.height);
		}
	}

	return sf::FloatRect(min, max - min);
}

void SFMLArmatureProxy::setVisible(bool visible)
{
	for (auto node : _nodes)
	{
		node->setVisible(visible);
	}
}

void SFMLArmatureProxy::setColor(const sf::Color& color)
{
	for (auto node : _nodes)
	{
		node->setColor(color);
	}
}

void SFMLArmatureProxy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= this->_transform;

	for (auto node : _nodes)
	{
		if (!node)
			continue;

		target.draw(*node, states);
	}
}

DRAGONBONES_NAMESPACE_END
