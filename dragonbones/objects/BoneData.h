#ifndef OBJECTS_BONE_DATA_H
#define OBJECTS_BONE_DATA_H

#include "../DragonBones.h"
#include "IAreaData.h"
#include "EllipseData.h"
#include "RectangleData.h"
#include "../geoms/Transform.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class BoneData
{
public:
    bool inheritScale;
    bool inheritRotation;
    float length;
    
    std::string name;
    std::string parent;
    Transform global;
    Transform transform;
    std::vector<IAreaData*> areaDataList;
    
public:
    BoneData()
    {
        inheritScale = false;
        inheritRotation = false;
        length = 0.f;
    }
    BoneData(const BoneData &copyData)
    {
        operator=(copyData);
    }
    BoneData& operator=(const BoneData &copyData)
    {
        dispose();
        inheritScale = copyData.inheritScale;
        inheritRotation = copyData.inheritRotation;
        length = copyData.length;
        name = copyData.name;
        parent = copyData.parent;
        global = copyData.global;
        transform = copyData.transform;
        areaDataList.reserve(copyData.areaDataList.size());
        
        for (size_t i = 0, l = copyData.areaDataList.size(); i < l; ++i)
        {
            switch (copyData.areaDataList[i]->areaType)
            {
                case IAreaData::AreaType::AT_ELLIPSE:
                    areaDataList.push_back(new EllipseData());
                    *(areaDataList[i]) = *(static_cast<EllipseData*>(copyData.areaDataList[i]));
                    break;
                    
                case IAreaData::AreaType::AT_RECTANGLE:
                    areaDataList.push_back(new RectangleData());
                    *(areaDataList[i]) = *(static_cast<RectangleData*>(copyData.areaDataList[i]));
                    break;
                    
                default:
                    // throw
                    break;
            }
        }
        
        return *this;
    }
    virtual ~BoneData()
    {
        dispose();
    }
    void dispose()
    {
        for (size_t i = 0, l = areaDataList.size(); i < l; ++i)
        {
            areaDataList[i]->dispose();
            delete areaDataList[i];
        }
        
        areaDataList.clear();
    }
    
    IAreaData* getAreaData(const std::string &areaName) const
    {
        if (areaDataList.empty())
        {
            return nullptr;
        }
        
        if (areaName.empty())
        {
            return areaDataList.front();
        }
        
        for (size_t i = 0, l = areaDataList.size(); i < l; ++i)
        {
            if (areaDataList[i]->name == areaName)
            {
                return areaDataList[i];
            }
        }
        
        return nullptr;
    }
};
NAME_SPACE_DRAGON_BONES_END
#endif  // OBJECTS_BONE_DATA_H