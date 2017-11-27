/*
*********************************************************************
* File          : SFMLEventDispatcher.h
* Project		: DragonBonesSFML
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
* License   	: MIT License
*********************************************************************
*/

#pragma once

#include <unordered_map>
#include <functional>
#include <vector>

#include <dragonBones\DragonBonesHeaders.h>

DRAGONBONES_NAMESPACE_BEGIN

class SFMLEventDispatcher : public IEventDispatcher
{
	using Func_t = std::function<void(EventObject*)>;

private:
	std::unordered_map<std::string, std::vector<Func_t>> _listeners;

public:
	SFMLEventDispatcher() = default;
	~SFMLEventDispatcher() = default;

	virtual void addDBEventListener(const std::string& type, const Func_t& listener) override
	{
		_listeners[type].push_back(listener);
	}

	virtual void removeDBEventListener(const std::string& type, const Func_t& listener) override
	{
		/*for (auto& callback : _listeners[type])
		{
		}*/
	}

	virtual void dispatchDBEvent(const std::string& type, EventObject * value) override
	{
		for (auto& listener : _listeners[type])
		{
			listener(value);
		}
	}

	virtual bool hasDBEventListener(const std::string& type) const { return true; };
};

DRAGONBONES_NAMESPACE_END
