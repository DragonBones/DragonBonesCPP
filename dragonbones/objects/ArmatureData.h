#ifndef OBJECTS_ARMATURE_DATA_H
#define OBJECTS_ARMATURE_DATA_H

#include "../DragonBones.h"
#include "IAreaData.h"
#include "BoneData.h"
#include "SkinData.h"
#include "AnimationData.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class ArmatureData
{
private:
    static bool sortBone(const std::pair<int, BoneData*> &a, const std::pair<int, BoneData*> &b)
    {
        return a.first < b.first;
    }
    
public:
    std::string name;
    std::vector<IAreaData*> areaDataList;
    std::vector<BoneData*> boneDataList;
    std::vector<SkinData*> skinDataList;
    std::vector<AnimationData*> animationDataList;
    
public:
    ArmatureData() {}
    ArmatureData(const ArmatureData &copyData)
    {
        operator=(copyData);
    }
    ArmatureData& operator=(const ArmatureData &copyData)
    {
        dispose();
        name = copyData.name;
        areaDataList.reserve(copyData.areaDataList.size());
        
        for (size_t i = 0, l = areaDataList.size(); i < l; ++i)
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
        
        boneDataList.reserve(copyData.boneDataList.size());
        
        for (size_t i = 0, l = boneDataList.size(); i < l; ++i)
        {
            boneDataList.push_back(new BoneData());
            *(boneDataList[i]) = *(copyData.boneDataList[i]);
        }
        
        skinDataList.reserve(copyData.skinDataList.size());
        
        for (size_t i = 0, l = skinDataList.size(); i < l; ++i)
        {
            skinDataList.push_back(new SkinData());
            *(skinDataList[i]) = *(copyData.skinDataList[i]);
        }
        
        animationDataList.reserve(copyData.animationDataList.size());
        
        for (size_t i = 0, l = animationDataList.size(); i < l; ++i)
        {
            animationDataList.push_back(new AnimationData());
            *(animationDataList[i]) = *(copyData.animationDataList[i]);
        }
        
        return *this;
    }
    virtual ~ArmatureData()
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
        
        for (size_t i = 0, l = boneDataList.size(); i < l; ++i)
        {
            boneDataList[i]->dispose();
            delete boneDataList[i];
        }
        
        for (size_t i = 0, l = skinDataList.size(); i < l; ++i)
        {
            skinDataList[i]->dispose();
            delete skinDataList[i];
        }
        
        for (size_t i = 0, l = animationDataList.size(); i < l; ++i)
        {
            animationDataList[i]->dispose();
            delete animationDataList[i];
        }
        
        areaDataList.clear();
        boneDataList.clear();
        skinDataList.clear();
        animationDataList.clear();
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
    
    BoneData* getBoneData(const std::string &boneName) const
    {
        for (size_t i = 0, l = boneDataList.size(); i < l; ++i)
        {
            if (boneDataList[i]->name == boneName)
            {
                return boneDataList[i];
            }
        }
        
        return nullptr;
    }

    SkinData* getDefaultSkinData() const
    {
        if (skinDataList.empty())
        {
            return nullptr;
        }

        for (size_t i = 0, l = skinDataList.size(); i < l; ++i)
        {
            const std::string &skinDataName = skinDataList[i]->name;
            if (
                skinDataName.empty() || 
                skinDataName == "default"
                )
            {
                return skinDataList[i];
            }
        }

        return skinDataList.front();
    }
    
    SkinData* getSkinData(const std::string &skinName) const
    {
        if (skinDataList.empty())
        {
            return nullptr;
        }
        
        if (skinName.empty())
        {
            return getDefaultSkinData();
        }
        
        for (size_t i = 0, l = skinDataList.size(); i < l; ++i)
        {
            if (skinDataList[i]->name == skinName)
            {
                return skinDataList[i];
            }
        }
        
        return nullptr;
    }
    
    AnimationData* getAnimationData(const std::string &animationName) const
    {
        for (size_t i = 0, l = animationDataList.size(); i < l; ++i)
        {
            if (animationDataList[i]->name == animationName)
            {
                return animationDataList[i];
            }
        }
        
        return nullptr;
    }
    
    void sortBoneDataList()
    {
        if (boneDataList.empty())
        {
            return;
        }
        
        std::vector<std::pair<int , BoneData*>> sortedList;
        
        for (size_t i = 0, l = boneDataList.size(); i < l; ++i)
        {
            BoneData *boneData = boneDataList[i];
            BoneData *parentData = boneData;
            int level = 0;
            
            while (parentData)
            {
                parentData = getBoneData(parentData->parent);
                level ++;
            }
            
            sortedList.push_back(std::make_pair(level , boneData));
        }
        
        std::sort(sortedList.begin() , sortedList.end() , sortBone);
        
        for (size_t i = 0, l = sortedList.size(); i < l; ++i)
        {
            boneDataList[i] = sortedList[i].second;
        }
    }
};
NAME_SPACE_DRAGON_BONES_END
#endif  // OBJECTS_ARMATURE_DATA_H
