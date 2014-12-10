#ifndef OBJECTS_SLOT_DATA_H
#define OBJECTS_SLOT_DATA_H

#include "../DragonBones.h"
#include "DisplayData.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class SlotData
{
public:
    float zOrder;
    std::string name;
    std::string parent;

    BlendMode blendMode;

    std::vector<DisplayData*> displayDataList;
    
public:
    SlotData():
        zOrder(0.f),
        name(),
        parent(),

        blendMode(BlendMode::BM_NORMAL),

        displayDataList()
    {

    }
    SlotData(const SlotData &copyData)
    {
        operator=(copyData);
    }
    SlotData& operator=(const SlotData &copyData)
    {
        dispose();

        zOrder = copyData.zOrder;
        name = copyData.name;
        parent = copyData.parent;

        blendMode = copyData.blendMode;

        displayDataList.reserve(copyData.displayDataList.size());
        for (size_t i = 0, l = displayDataList.size(); i < l; ++i)
        {
            displayDataList.push_back(new DisplayData());
            *(displayDataList[i]) = *(copyData.displayDataList[i]);
        }
        
        return *this;
    }
    virtual ~SlotData()
    {
        dispose();
    }

    void dispose()
    {
        zOrder = 0.f;
        name.clear();
        parent.clear();

        blendMode = BlendMode::BM_NORMAL;

        for (size_t i = 0, l = displayDataList.size(); i < l; ++i)
        {
            delete displayDataList[i];
        }
        displayDataList.clear();
    }
    
    const DisplayData* getDisplayData(const std::string &displayName) const
    {
        for (size_t i = 0, l = displayDataList.size(); i < l; ++i)
        {
            if (displayDataList[i]->name == displayName)
            {
                return displayDataList[i];
            }
        }
        
        return nullptr;
    }
};
NAME_SPACE_DRAGON_BONES_END
#endif  // OBJECTS_SLOT_DATA_H
