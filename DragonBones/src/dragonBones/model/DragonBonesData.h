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
    char* buffer;
    /**
    * @private
    */
    int16_t* intArray;
    /**
    * @private
    */
    float* floatArray;
    /**
    * @private
    */
    int16_t* frameIntArray;
    /**
    * @private
    */
    float* frameFloatArray;
    /**
    * @private
    */
    int16_t* frameArray;
    /**
    * @private
    */
    uint16_t* timelineArray;
    /**
    * @private
    */
    UserData* userData;
    /**
    * @private
    */
    DragonBonesData() :
        buffer(nullptr),
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
    //TODO
    static DragonBonesData* getDragonBoneData();
    static void setDragonBoneData(DragonBonesData* value);
    /**
    * dragonBones 5.0
    * @param intarray
    * @param floatArray
    * @param frameIntArray
    * @param frameFloatArray
    * @param frameArray
    * @param timelineArray
    * @return
    */
    void setBufferPtr(int16_t* intarray, float* floatArray, int16_t* frameIntArray, float* frameFloatArray, int16_t* frameArray, uint16_t* timelineArray);
    /**
     * @code
     * var buf = Module._malloc(myTypedArray.length*myTypedArray.BYTES_PER_ELEMENT);
     * Module.HEAPU8.set(myTypedArray, buf);
     * Module.ccall('my_function', 'number', ['number'], [buf]);
     * Module._free(buf);
     * @endcode
     * @param intArry
     * @param floatArray
     * @param frameIntArray
     * @param frameFloatArray
     * @param frameArray
     * @param timelineArray
     */
    void parseArrays(int16_t* intArry, float* floatArray, int16_t* frameIntArray, float* frameFloatArray, int16_t* frameArray, uint16_t* timelineArray);

    std::vector<std::string>* getArmatureNames() { return &armatureNames; }
    std::vector<unsigned>* getFrameIndices() { return &frameIndices; }
    const int16_t* getIntArray() const { return intArray; }
    const float* getFloatArray() const { return floatArray; }
    const int16_t* getFrameIntArray() const { return  frameIntArray; }
    const float* getFrameFloatArray() const { return frameFloatArray; }
    const int16_t* getFrameArray() const { return  frameArray; }
    const uint16_t* getTimelineArray() const { return timelineArray; }

private:
    /**
     * Egret 5.0
     */
    static DragonBonesData* _dragonBoneData;

};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_DRAGONBONES_DATA_H
