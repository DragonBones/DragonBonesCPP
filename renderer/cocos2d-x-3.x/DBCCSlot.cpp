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
    disposeDisplayList();
    Slot::dispose();
    _nodeDisplay = nullptr;
}

int DBCCSlot::getDisplayZIndex() const
{
    if (_nodeDisplay)
    {
        return _nodeDisplay->getLocalZOrder();
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
            nodeContainer->addChild(_nodeDisplay, nodeContainer->getChildrenCount());
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
        _nodeDisplay->removeFromParentAndCleanup(false);
    }
}

void DBCCSlot::disposeDisplayList()
{
    for (size_t i = 0, l = _displayList.size(); i < l; ++i)
    {
        if (_displayList[i].second == DisplayType::DT_ARMATURE)
        {
            Armature *armature = static_cast<Armature *>(_displayList[i].first);
            armature->dispose();
        }
        else
        {
            cocos2d::Node *display = static_cast<cocos2d::Node *>(_displayList[i].first);
            display->cleanup();
            display->release();
        }
    }
}

void DBCCSlot::updateDisplay(void *display, bool disposeExisting)
{
    if (disposeExisting)
    {
        if (_nodeDisplay)
        {
            _nodeDisplay->cleanup();
            _nodeDisplay->release();
        }
    }
    _nodeDisplay = static_cast<cocos2d::Sprite *>(display);
}

void DBCCSlot::updateDisplayBlendMode(BlendMode blendMode)
{
    if (_nodeDisplay)
    {
        switch (blendMode)
        {
            case BlendMode::BM_ADD:
                break;
            case BlendMode::BM_ALPHA:
                break;
            case BlendMode::BM_DARKEN:
                break;
            case BlendMode::BM_DIFFERENCE:
                break;
            case BlendMode::BM_ERASE:
                break;
            case BlendMode::BM_HARDLIGHT:
                break;
            case BlendMode::BM_INVERT:
                break;
            case BlendMode::BM_LAYER:
                break;
            case BlendMode::BM_LIGHTEN:
                break;
            case BlendMode::BM_MULTIPLY:
                break;
            case BlendMode::BM_NORMAL:
                break;
            case BlendMode::BM_OVERLAY:
                break;
            case BlendMode::BM_SCREEN:
                break;
            case BlendMode::BM_SHADER:
                break;
            case BlendMode::BM_SUBTRACT:
                break;
            default:
                break;
        }
    }
}

void DBCCSlot::updateDisplayColor(int aOffset, int rOffset, int gOffset, int bOffset, float aMultiplier, float rMultiplier, float gMultiplier, float bMultiplier)
{
    if (_nodeDisplay)
    {
        _nodeDisplay->setOpacity(aOffset + aMultiplier * 255.f);
        _nodeDisplay->setColor(cocos2d::Color3B(rOffset + rMultiplier * 255.f , gOffset + gMultiplier * 255.f , bOffset + bMultiplier * 255.f));
    }
}

void DBCCSlot::updateDisplayVisible(bool visible)
{
    if (_nodeDisplay && _parent)
    {
        _nodeDisplay->setVisible(_parent->getVisible() && _visible && visible);
    }
}

void DBCCSlot::updateDisplayTransform()
{
    if (_nodeDisplay)
    {
        _nodeDisplay->setScaleX(global.scaleX);
        _nodeDisplay->setScaleY(global.scaleY);
        _nodeDisplay->setRotationSkewX(global.skewX * RADIAN_TO_ANGLE);
        _nodeDisplay->setRotationSkewY(global.skewY * RADIAN_TO_ANGLE);
        _nodeDisplay->setPosition(global.x , -global.y);
    }
}
NAME_SPACE_DRAGON_BONES_END