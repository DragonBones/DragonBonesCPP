#include "DBCCSlot.h"

NAME_SPACE_DRAGON_BONES_BEGIN
DBCCSlot::DBCCSlot(SlotData *slotData)
    : Slot(slotData)
{
    _spriteDisplay = nullptr;
}
DBCCSlot::~DBCCSlot()
{
    dispose();
}
void DBCCSlot::dispose()
{
    Slot::dispose();
    _spriteDisplay = nullptr;
}

int DBCCSlot::getDisplayZIndex() const
{
    if (_spriteDisplay)
    {
        return _spriteDisplay->getZOrder();
    }
    
    return -1;
}
void DBCCSlot::addDisplayToContainer(void *container, int zIndex)
{
    cocos2d::Node *nodeContainer = static_cast<cocos2d::Node *>(container);
    
    if (_spriteDisplay && nodeContainer)
    {
        if (zIndex < 0)
        {
            nodeContainer->addChild(_spriteDisplay);
        }
        
        else
        {
            nodeContainer->addChild(_spriteDisplay, zIndex);
        }
    }
}
void DBCCSlot::removeDisplayFromContainer()
{
    if (_spriteDisplay && _spriteDisplay->getParent())
    {
        _spriteDisplay->getParent()->removeChild(_spriteDisplay);
    }
}

void DBCCSlot::updateDisplay(void *display, bool disposeExisting)
{
    if (disposeExisting)
    {
        if (_spriteDisplay)
        {
            _spriteDisplay->cleanup();
        }
    }
    
    _spriteDisplay = static_cast<cocos2d::Sprite *>(display);
}

void DBCCSlot::updateDisplayBlendMode(const BlendMode &blendMode)
{
}

void DBCCSlot::updateDisplayColor(int aOffset, int rOffset, int gOffset, int bOffset, float aMultiplier, float rMultiplier, float gMultiplier, float bMultiplier)
{
    if (_spriteDisplay)
    {
        //_spriteDisplay->setOpacity();
        //_spriteDisplay->setColor();
    }
}

void DBCCSlot::updateDisplayVisible(bool visible)
{
    if (_spriteDisplay)
    {
        _spriteDisplay->setVisible(_parent->getVisible() && _visible && visible);
    }
}

void DBCCSlot::updateDisplayTransform()
{
    if (_spriteDisplay)
    {
        _spriteDisplay->setPosition(globalTransformMatrix.tx , -globalTransformMatrix.ty);
        _spriteDisplay->setScaleX(global.scaleX);
        _spriteDisplay->setScaleY(global.scaleY);
        _spriteDisplay->setRotationSkewX(CC_RADIANS_TO_DEGREES(global.skewX));
        _spriteDisplay->setRotationSkewY(CC_RADIANS_TO_DEGREES(global.skewY));
    }
}
NAME_SPACE_DRAGON_BONES_END