#include "DBCCSlot.h"

NAME_SPACE_DRAGON_BONES_BEGIN
DBCCSlot::DBCCSlot(SlotData *slotData)
    : Slot(slotData)
{
    _nodeDisplay = nullptr;
}
DBCCSlot::~DBCCSlot()
{
    dispose();
}
void DBCCSlot::dispose()
{
	for(size_t i = 0, l = _displayList.size(); i < l; ++i)
	{
		if(_displayList[i].second != DisplayType::DT_ARMATURE)
		{
			cocos2d::Node* display = static_cast<cocos2d::Node *>(_displayList[i].first);
			display->cleanup();
			display->release();
		}
	}
    Slot::dispose();
    _nodeDisplay = nullptr;
}

int DBCCSlot::getDisplayZIndex() const
{
    if (_nodeDisplay)
    {
        return _nodeDisplay->getZOrder();
    }
    return -1;
}
void DBCCSlot::addDisplayToContainer(void *container, int zIndex)
{
    cocos2d::Node *nodeContainer = static_cast<cocos2d::Node *>(container);
    if (_nodeDisplay && nodeContainer)
    {
        removeDisplayFromContainer();
        if (zIndex < 0)
        {
            nodeContainer->addChild(_nodeDisplay);
        }
        else
        {
            nodeContainer->addChild(_nodeDisplay, zIndex);
        }
    }
}
void DBCCSlot::removeDisplayFromContainer()
{
    if (_nodeDisplay && _nodeDisplay->getParent())
    {
        _nodeDisplay->getParent()->removeChild(_nodeDisplay);
    }
}

void DBCCSlot::updateDisplay(void *display, bool disposeExisting)
{
    if (disposeExisting)
    {
        if (_nodeDisplay)
        {
            _nodeDisplay->cleanup();
        }
    }
    _nodeDisplay = static_cast<cocos2d::Sprite *>(display);
}

void DBCCSlot::updateDisplayBlendMode(const BlendMode &blendMode)
{
}

void DBCCSlot::updateDisplayColor(int aOffset, int rOffset, int gOffset, int bOffset, float aMultiplier, float rMultiplier, float gMultiplier, float bMultiplier)
{
    if (_nodeDisplay)
    {
        //_spriteDisplay->setOpacity();
        //_spriteDisplay->setColor();
    }
}

void DBCCSlot::updateDisplayVisible(bool visible)
{
    if (_nodeDisplay)
    {
        _nodeDisplay->setVisible(_parent->getVisible() && _visible && visible);
    }
}

void DBCCSlot::updateDisplayTransform()
{
    if (_nodeDisplay)
    {
        _nodeDisplay->setPosition(globalTransformMatrix.tx , -globalTransformMatrix.ty);
        _nodeDisplay->setScaleX(global.scaleX);
        _nodeDisplay->setScaleY(global.scaleY);
        _nodeDisplay->setRotationSkewX(CC_RADIANS_TO_DEGREES(global.skewX));
        _nodeDisplay->setRotationSkewY(CC_RADIANS_TO_DEGREES(global.skewY));
    }
}
NAME_SPACE_DRAGON_BONES_END