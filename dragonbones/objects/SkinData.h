#ifndef OBJECTS_SKIN_DATA_H
#define OBJECTS_SKIN_DATA_H

#include "../DragonBones.h"
#include "SlotData.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class SkinData
{
public:
    std::string name;
    std::vector<SlotData*> slotDataList;
    
public:
    SkinData() {}
    SkinData(const SkinData &copyData)
    {
        operator=(copyData);
    }
    SkinData& operator=(const SkinData &copyData)
    {
        dispose();
        name = copyData.name;
        slotDataList.reserve(copyData.slotDataList.size());
        
        for (size_t i = 0, l = slotDataList.size(); i < l; ++i)
        {
            slotDataList.push_back(new SlotData());
            *(slotDataList[i]) = *(copyData.slotDataList[i]);
        }
        
        return *this;
    }
    virtual ~SkinData()
    {
        dispose();
    }
    void dispose()
    {
        for (size_t i = 0, l = slotDataList.size(); i < l; ++i)
        {
            slotDataList[i]->dispose();
            delete slotDataList[i];
        }
        
        slotDataList.clear();
    }
    
    SlotData* getSlotData(const std::string &slotName) const
    {
        for (size_t i = 0, l = slotDataList.size(); i < l; ++i)
        {
            if (slotDataList[i]->name == slotName)
            {
                return slotDataList[i];
            }
        }
        
        return nullptr;
    }
};
NAME_SPACE_DRAGON_BONES_END
#endif  // OBJECTS_SKIN_DATA_H
