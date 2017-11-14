#ifndef DRAGONBONES_CC_SLOT_H
#define DRAGONBONES_CC_SLOT_H

#include "dragonBones/DragonBonesHeaders.h"
#include "cocos2d.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
 * The Cocos2d slot.
 * @version DragonBones 3.0
 * @language en_US
 */
/**
 * Cocos2d 插槽。
 * @version DragonBones 3.0
 * @language zh_CN
 */
class CCSlot : public Slot
{
    BIND_CLASS_TYPE_A(CCSlot);

private:
    float _textureScale;
    cocos2d::Node* _renderDisplay;

protected:
    /**
     * @inheritDoc
     */
    virtual void _onClear() override;
    /**
     * @inheritDoc
     */
    virtual void _initDisplay(void* value) override;
    /**
     * @inheritDoc
     */
    virtual void _disposeDisplay(void* value) override;
    /**
     * @inheritDoc
     */
    virtual void _onUpdateDisplay() override;
    /**
     * @inheritDoc
     */
    virtual void _addDisplay() override;
    /**
     * @inheritDoc
     */
    virtual void _replaceDisplay(void* value, bool isArmatureDisplay) override;
    /**
     * @inheritDoc
     */
    virtual void _removeDisplay() override;
    /**
     * @inheritDoc
     */
    virtual void _updateZOrder() override;

public:
    /**
     * @inheritDoc
     */
    virtual void _updateVisible() override;
    /**
     * @inheritDoc
     */
    virtual void _updateBlendMode() override;
    /**
     * @inheritDoc
     */
    virtual void _updateColor() override;

protected:
    /**
     * @inheritDoc
     */
    virtual void _updateFrame() override;
    /**
     * @inheritDoc
     */
    virtual void _updateMesh() override;
    /**
     * @inheritDoc
     */
    virtual void _updateTransform(bool isSkinnedMesh) override;

public:
    /**
     * The display object that the slot displays at this time.
     * @see #display
     * @version DragonBones 3.0
     * @language en_US
     */
    /**
     * 插槽此时显示的显示对象。
     * @see #display
     * @version DragonBones 3.0
     * @language zh_CN
     */
    inline cocos2d::Node* getCCDisplay() const
    {
        return static_cast<cocos2d::Node*>(_display);
    }
    inline void setCCDisplay(cocos2d::Node* value)
    {
        setDisplay(value, DisplayType::Image);
    }
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_CC_SLOT_H
