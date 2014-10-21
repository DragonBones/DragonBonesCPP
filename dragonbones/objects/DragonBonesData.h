#ifndef OBJECTS_DRADON_BONES_DATA_H
#define OBJECTS_DRADON_BONES_DATA_H

#include "../DragonBones.h"
#include "ArmatureData.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class DragonBonesData
{
public:
    bool autoSearch;
    std::string name;
    std::vector<ArmatureData*> armatureDataList;
    
public:
    DragonBonesData()
    {
        autoSearch = false;
    }
    DragonBonesData(const DragonBonesData &copyData)
    {
        operator=(copyData);
    }
    DragonBonesData& operator=(const DragonBonesData &copyData)
    {
        dispose();
        autoSearch = copyData.autoSearch;
        name = copyData.name;
        armatureDataList.reserve(copyData.armatureDataList.size());
        
        for (size_t i = 0, l = armatureDataList.size(); i < l; ++i)
        {
            armatureDataList.push_back(new ArmatureData());
            *(armatureDataList[i]) = *(copyData.armatureDataList[i]);
        }
        
        return *this;
    }
    virtual ~DragonBonesData()
    {
        dispose();
    }
    void dispose()
    {
        for (size_t i = 0, l = armatureDataList.size(); i < l; ++i)
        {
            armatureDataList[i]->dispose();
            delete armatureDataList[i];
        }
        
        armatureDataList.clear();
    }
    
    ArmatureData* getArmatureData(const std::string &armatureName) const
    {
        for (size_t i = 0, l = armatureDataList.size(); i < l; ++i)
        {
            if (armatureDataList[i]->name == armatureName)
            {
                return armatureDataList[i];
            }
        }
        
        return nullptr;
    }
};
NAME_SPACE_DRAGON_BONES_END
#endif  // OBJECTS_DRADON_BONES_DATA_H
