#include "DBCCSlot.h"
#include "DBCCArmature.h"

NAME_SPACE_DRAGON_BONES_BEGIN

cocos2d::Node* DBCCSlot::getCCDisplay() const
{
    return _nodeDisplay;
}

DBCCArmature* DBCCSlot::getCCChildArmature() const
{
    return static_cast<DBCCArmature*>(_childArmature);
}

cocos2d::Vec2 DBCCSlot::getGlobalPosition() const
{
    return cocos2d::Vec2(global.x, global.y);
}

void DBCCSlot::setDisplayImage(cocos2d::Node *display, bool disposeExisting/* = true*/)
{
    Slot::setDisplay(display, DisplayType::DT_IMAGE, disposeExisting);
}

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

Rectangle DBCCSlot::getBoundingBox()
{
    if (_displayIndex < 0)
    {
        return Rectangle();
    }

    if (_displayList[_displayIndex].second == DisplayType::DT_ARMATURE)
    {
        return getCCChildArmature()->getBoundingBox();
    }

    auto display = getCCDisplay();
    if (display)
    {
    auto r = getCCDisplay()->getBoundingBox();
        return Rectangle(r.origin.x, r.origin.y, r.size.width, r.size.height); 
    }

    return Rectangle();
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
    cocos2d::Node *nodeContainer = static_cast<cocos2d::Node*>(container);
    
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
    std::vector<cocos2d::Node*> releasedNodeList;
    
    for (size_t i = 0, l = _displayList.size(); i < l; ++i)
    {
        if (_displayList[i].second == DisplayType::DT_ARMATURE)
        {
            Armature *armature = static_cast<Armature*>(_displayList[i].first);
            
            if (armature)
            {
                //armature->dispose();
                delete armature;
                _displayList[i].first = NULL;
            }
        }
        else
        {
            cocos2d::Node *display = static_cast<cocos2d::Node*>(_displayList[i].first);
            
            if (display && std::find(releasedNodeList.cbegin(), releasedNodeList.cend(), display) == releasedNodeList.cend())
            {
                display->cleanup();
                display->release();
                releasedNodeList.push_back(display);
            }
        }
    }
    
    releasedNodeList.clear();
}
void DBCCSlot::disposeDisplay()
{
    if (_nodeDisplay)
    {
        _nodeDisplay->cleanup();
        _nodeDisplay->release();
        _nodeDisplay = nullptr;
    }
}

void DBCCSlot::updateDisplay(void *display)
{
    _nodeDisplay = static_cast<cocos2d::Sprite*>(display);
}

void DBCCSlot::updateDisplayBlendMode(BlendMode blendMode)
{
    cocos2d::Sprite *spriteDisplay = dynamic_cast<cocos2d::Sprite*>(_nodeDisplay);
    
    if (spriteDisplay)
    {
        switch (blendMode)
        {
            case BlendMode::BM_ADD:
            {
                auto texture = spriteDisplay->getTexture();
                if (texture && texture->hasPremultipliedAlpha())
                {
                    cocos2d::BlendFunc func = {GL_ONE, GL_ONE};
                    spriteDisplay->setBlendFunc(func);
                }else
                {
                    spriteDisplay->setBlendFunc(cocos2d::BlendFunc::ADDITIVE);
                }
                break;
            }
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
        
    if (_childArmature)
    {
        for (size_t i = 0, l = _childArmature->getSlots().size(); i < l; ++i)
        {
            DBCCSlot *slot = static_cast<DBCCSlot*>(_childArmature->getSlots()[i]);
            slot->_blendMode = blendMode;
            slot->updateDisplayBlendMode(blendMode);
        }
    }
}

void DBCCSlot::updateDisplayColor(int aOffset, int rOffset, int gOffset, int bOffset, float aMultiplier, float rMultiplier, float gMultiplier, float bMultiplier)
{
    if (_nodeDisplay)
    {
        //record colorTransform
        Slot::updateDisplayColor(aOffset, rOffset, gOffset, bOffset, aMultiplier, rMultiplier, gMultiplier, bMultiplier);

        // cocos2dx does not support offset of color.
        _nodeDisplay->setOpacity(aMultiplier * 255.f);
        _nodeDisplay->setColor(cocos2d::Color3B(rMultiplier * 255.f , gMultiplier * 255.f , bMultiplier * 255.f));
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
