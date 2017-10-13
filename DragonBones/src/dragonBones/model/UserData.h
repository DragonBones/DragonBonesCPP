#ifndef DRAGONBONES_USER_DATA_H
#define DRAGONBONES_USER_DATA_H

#include "../core/BaseObject.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* 自定义数据。
* @version DragonBones 5.0
* @language zh_CN
*/
class UserData : public BaseObject
{
    BIND_CLASS_TYPE_A(UserData);

public:
    /**
    * 自定义整数。
    * @version DragonBones 5.0
    * @language zh_CN
    */
    std::vector<int> ints;
    /**
    * 自定义浮点数。
    * @version DragonBones 5.0
    * @language zh_CN
    */
    std::vector<float> floats;
    /**
    * 自定义字符串。
    * @version DragonBones 5.0
    * @language zh_CN
    */
    std::vector<std::string> strings;

protected:
    virtual void _onClear() override;

public:
    /**
    * @private
    */
    void addInt(int value);
    /**
    * @private
    */
    void addFloat(float value);
    /**
    * @private
    */
    void addString(std::string value);
    /**
    * 获取自定义整数。
    * @version DragonBones 5.0
    * @language zh_CN
    */
    int getInt(unsigned index) const;
    /**
    * 获取自定义浮点数。
    * @version DragonBones 5.0
    * @language zh_CN
    */
    float getFloat(unsigned index) const;
    /**
    * 获取自定义字符串。
    * @version DragonBones 5.0
    * @language zh_CN
    */
    std::string getString(unsigned index) const;

public: // For WebAssembly.
    const std::vector<int>& getInts() const { return ints; }
    const std::vector<float>& getFloats() const { return floats; }
    const std::vector<std::string>& getStrings() const { return strings; }
};
/**
* @private
*/
class ActionData : public BaseObject
{
    BIND_CLASS_TYPE_B(ActionData);

public:
    ActionType type;
    std::string name;
    const BoneData* bone;
	const SlotData* slot;
	UserData* data;

    ActionData() :
        data(nullptr)
    {
        _onClear();
    }
    virtual ~ActionData()
    {
        _onClear();
    }

protected:
    virtual void _onClear() override;

public: // For WebAssembly.
    int getType() const { return (int)type; }
    void setType(int value) { type = (ActionType)value; }

    const BoneData* getBone() const { return bone; }
    void setBone(const BoneData* value) { bone = value; }

    const SlotData* getSlot() const { return slot; }
    void setSlot(const SlotData* value) { slot = value; }

    const UserData* getData() const { return data; }
    void setData(UserData* value) { data = value; }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_USER_DATA_H
