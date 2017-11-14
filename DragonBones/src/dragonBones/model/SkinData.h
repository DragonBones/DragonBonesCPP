#ifndef DRAGONBONES_SKIN_DATA_H
#define DRAGONBONES_SKIN_DATA_H

#include "../core/BaseObject.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* Ƥ�����ݡ���ͨ��һ���Ǽ��������ٰ���һ��Ƥ�����ݣ�
* @version DragonBones 3.0
* @language zh_CN
*/
class SkinData : public BaseObject
{
    BIND_CLASS_TYPE_A(SkinData);

public:
    /**
    * �������ơ�
    * @version DragonBones 3.0
    * @language zh_CN
    */
    std::string name;
    /**
    * @private
    */
    std::map<std::string, std::vector<DisplayData*>> displays;
    /**
    * @private
    */
    ArmatureData* parent;

protected:
    virtual void _onClear() override;

public:
    /**
    * @private
    */
    void addDisplay(const std::string& slotName, DisplayData* value);
    /**
    * @private
    */
    DisplayData* getDisplay(const std::string& slotName, const std::string& displayName);
    /**
    * @private
    */
    std::vector<DisplayData*>* getDisplays(const std::string& slotName)
    {
        return mapFindB(displays, slotName);
    }

public: // For WebAssembly. TODO parent
    const std::map<std::string, std::vector<DisplayData*>>& getSlotDisplays() const { return displays; }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_SKIN_DATA_H
