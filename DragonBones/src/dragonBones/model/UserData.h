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
    int getInt(unsigned index) const;
    float getFloat(unsigned index) const;
    std::string getString(unsigned index) const;
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
    BoneData* bone;
    SlotData* slot;
    UserData* data;

    ActionData() :
        data(nullptr)
    {
        _onClear();
    }
    ~ActionData()
    {
        _onClear();
    }

protected:
    virtual void _onClear() override;

public: // For WebAssembly.
    int getType() const { return (int)type; }
    BoneData* getBone() const { return bone; }
    SlotData* getSlot() const { return slot; }
    UserData* getData() const { return data; }

    void setType(int value) { type = (ActionType)value; }
    void setBone(BoneData* value) { bone = value; }
    void setSlot(SlotData* value) { slot = value; }
    void setData(UserData* value) { data = value; }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_USER_DATA_H
