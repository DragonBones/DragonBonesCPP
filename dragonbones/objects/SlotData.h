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
    SlotData()
    {
        zOrder = 0.f;
        blendMode = BlendMode::BM_NORMAL;
    }
    SlotData(const SlotData &copyData)
    {
        operator=(copyData);
    }
    SlotData& operator=(const SlotData &copyData)
    {
        dispose();
        name = copyData.name;
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
