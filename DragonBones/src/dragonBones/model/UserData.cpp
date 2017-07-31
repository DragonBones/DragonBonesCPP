#include "UserData.h"

DRAGONBONES_NAMESPACE_BEGIN

void UserData::_onClear()
{
    ints.clear();
    floats.clear();
    strings.clear();
}

int UserData::getInt(unsigned index) const
{
    return index < ints.size() ? ints[index] : 0;
}

float UserData::getFloat(unsigned index) const
{
    return index < floats.size() ? floats[index] : 0;
}

std::string UserData::getString(unsigned index) const
{
    return index < strings.size() ? strings[index] : 0;
}

void ActionData::_onClear()
{
    if (data != nullptr) 
    {
        data->returnToPool();
    }

    type = ActionType::Play;
    name = "";
    bone = nullptr;
    slot = nullptr;
    data = nullptr;
}

DRAGONBONES_NAMESPACE_END
