#ifndef DRAGONBONES_CC_SLOT_H
#define DRAGONBONES_CC_SLOT_H

#include "dragonBones/DragonBonesHeaders.h"
#include "cocos2d.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* Cocos 插槽。
* @version DragonBones 3.0
* @language zh_CN
*/
class CCSlot : public Slot
{
    BIND_CLASS_TYPE_A(CCSlot);

private:
    cocos2d::Node* _renderDisplay;

protected:
    virtual void _onClear() override;

    virtual void _initDisplay(void* value) override;
    virtual void _disposeDisplay(void* value) override;
    virtual void _onUpdateDisplay() override;
    virtual void _addDisplay() override;
    virtual void _replaceDisplay(void* value, bool isArmatureDisplayContainer) override;
    virtual void _removeDisplay() override;
    virtual void _updateZOrder() override;
public:
    virtual void _updateVisible() override;
    virtual void _updateBlendMode() override;
    virtual void _updateColor() override;
protected:
    virtual void _updateFrame() override;
    virtual void _updateMesh() override;
    virtual void _updateTransform(bool isSkinnedMesh) override;

};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_CC_SLOT_H
