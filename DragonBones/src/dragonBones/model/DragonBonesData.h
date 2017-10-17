#ifndef DRAGONBONES_DRAGONBONES_DATA_H
#define DRAGONBONES_DRAGONBONES_DATA_H

#include "../core/BaseObject.h"
#include "ArmatureData.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* 龙骨数据。
* 一个龙骨数据包含多个骨架数据。
* @see dragonBones.ArmatureData
* @version DragonBones 3.0
* @language zh_CN
*/
class DragonBonesData : public BaseObject
{
    BIND_CLASS_TYPE_B(DragonBonesData);

public:
    /**
    * 是否开启共享搜索。
    * @default false
    * @version DragonBones 4.5
    * @language zh_CN
    */
    bool autoSearch;
    /**
    * 动画帧频。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    unsigned frameRate;
    /**
    * 数据版本。
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::string version;
    /**
    * 数据名称。(该名称与龙骨项目名保持一致)
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::string name;
    /**
    * @private
    */
    std::vector<unsigned> frameIndices;
    /**
    * @private
    */
    std::vector<float> cachedFrames;
    /**
    * 所有骨架数据名称。
    * @see #armatures
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::vector<std::string> armatureNames;
    /**
    * 所有骨架数据。
    * @see dragonBones.ArmatureData
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::map<std::string, ArmatureData*> armatures;
    /**
    * @private
    */
    const char* binary;
    /**
    * @private
    */
	const int16_t* intArray;
    /**
    * @private
    */
	const float* floatArray;
    /**
    * @private
    */
	const int16_t* frameIntArray;
    /**
    * @private
    */
	const float* frameFloatArray;
    /**
    * @private
    */
	const int16_t* frameArray;
    /**
    * @private
    */
	const uint16_t* timelineArray;
    /**
    * @private
    */
    UserData* userData;
    /**
    * @private
    */
    DragonBonesData() :
        binary(nullptr),
        userData(nullptr)
    {
        _onClear();
    }
    ~DragonBonesData()
    {
        _onClear();
    }
    /**
    * @private
    */
    void addArmature(ArmatureData* value);
    /**
    * 获取骨架数据。
    * @param name 骨架数据名称。
    * @see dragonBones.ArmatureData
    * @version DragonBones 3.0
    * @language zh_CN
    */
    inline ArmatureData* getArmature(const std::string& name) const
    {
        return mapFind<ArmatureData>(armatures, name);
    }

protected:
    virtual void _onClear() override;

public: // For WebAssembly.
    std::vector<unsigned>* getFrameIndices() { return &frameIndices; }
    const std::vector<std::string>& getArmatureNames() const { return armatureNames; }

#if EGRET_WASM
    unsigned getBinary() const
    {
        return (unsigned)binary;
    }
#endif // EGRET_WASM

    const UserData* getUserData() const { return userData; }
    void setUserData(UserData* value) { userData = value; }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_DRAGONBONES_DATA_H
