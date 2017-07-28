#include "DragonBones.h"
#include "../armature/Armature.h"
#include "../animation/WorldClock.h"
#include "../animation/Animation.h"
#include "../events/EventObject.h"
#include "../events/IEventDispatcher.h"

DRAGONBONES_NAMESPACE_BEGIN

bool DragonBones::yDown = true;
bool DragonBones::debug = false;
bool DragonBones::debugDraw = false;

void DragonBones::advanceTime(float passedTime)
{
    if (!_objects.empty())
    {
        for (const auto object : _objects)
        {
            object->returnToPool();
        }

        _objects.clear();
    }

    _clock->advanceTime(passedTime);

    if (!_events.empty()) 
    {
        for (std::size_t i = 0; i < _events.size(); ++i)
        {
            const auto eventObject = _events[i];
            const auto armature = eventObject->armature;
            armature->getEventDispatcher()->_dispatchEvent(eventObject->type, eventObject);
            if (eventObject->type == EventObject::SOUND_EVENT)
            {
                _eventManager->_dispatchEvent(eventObject->type, eventObject);
            }

            bufferObject(eventObject);
        }

        _events.clear();
    }
}

void DragonBones::bufferEvent(EventObject* value)
{
    _events.push_back(value);
}

void DragonBones::bufferObject(BaseObject* object)
{
    _objects.push_back(object);
}

WorldClock* DragonBones::getClock()
{
    if (_clock == nullptr)
    {
        _clock = new WorldClock();
    }

    return _clock;
}


DRAGONBONES_NAMESPACE_END
