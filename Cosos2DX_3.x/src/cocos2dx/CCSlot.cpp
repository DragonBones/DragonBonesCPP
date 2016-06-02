#include "CCSlot.h"

NAMESPACE_DRAGONBONES_BEGIN

CCSlot::CCSlot() 
{
    _onClear();
}
CCSlot::~CCSlot() 
{
    _onClear();
}

void CCSlot::_onClear()
{
    // _displayList // TODO

    Slot::_onClear();

    _renderDisplay = nullptr;
}

void CCSlot::_initDisplay(void * value)
{
    // TODO
}

void CCSlot::_onUpdateDisplay()
{
    _renderDisplay = static_cast<cocos2d::Node*>(this->_display ? this->_display : this->_rawDisplay);
}

void CCSlot::_addDisplay()
{
    const auto container = static_cast<CCArmatureDisplayContainer*>(this->_armature->getDisplay());
    container->addChild(_renderDisplay);
}

void CCSlot::_replaceDisplay(void * value)
{
    const auto container = static_cast<CCArmatureDisplayContainer*>(this->_armature->getDisplay());
    const auto prevDisplay = static_cast<cocos2d::Node*>(value);
    container->addChild(_renderDisplay, prevDisplay->getLocalZOrder());
    container->removeChild(prevDisplay);
}

void CCSlot::_removeDisplay()
{
    _renderDisplay->removeFromParent();
}

void CCSlot::_disposeDisplay(void * value)
{
    // TODO
}

void CCSlot::_updateColor()
{
    _renderDisplay->setOpacity(this->_colorTransform.alphaMultiplier * 255.f);
    _helpColor.r = this->_colorTransform.redMultiplier * 255.f;
    _helpColor.g = this->_colorTransform.greenMultiplier * 255.f;
    _helpColor.b = this->_colorTransform.blueMultiplier * 255.f;
    _renderDisplay->setColor(_helpColor);
}

void CCSlot::_updateFilters() 
{
}

void CCSlot::_updateFrame()
{
    const auto frameDisplay = static_cast<cocos2d::Sprite*>(_renderDisplay);
    const unsigned displayIndex = this->_displayIndex;

    if (this->_display && displayIndex >= 0 && displayIndex < this->_displayDataSet->displays.size())
    {
        const auto displayData = this->_displayDataSet->displays[displayIndex];
        const auto textureData = static_cast<CCTextureData*>(displayData->textureData);

        if (textureData && !textureData->texture)
        {
            const auto textureAtlasTexture = static_cast<CCTextureAtlasData*>(textureData->parent)->texture;
            if (textureAtlasTexture)
            {
                cocos2d::Rect rect(textureData->region.x, textureData->region.y, textureData->region.width, textureData->region.height);
                cocos2d::Vec2 offset(0.f, 0.f);
                cocos2d::Size originSize(textureData->region.width, textureData->region.height);
                textureData->texture = cocos2d::SpriteFrame::createWithTexture(textureAtlasTexture, rect, textureData->rotated, offset, originSize);
                textureData->texture->retain();
            }
        }

        if (textureData && textureData->texture)
        {
            const auto& rect = textureData->frame ? *textureData->frame : textureData->region;

            auto width = rect.width;
            auto height = rect.height;
            if (textureData->rotated)
            {
                width = rect.height;
                height = rect.width;
            }

            cocos2d::Vec2 pivot(displayData->pivot.x, displayData->pivot.y);
            if (displayData->isRelativePivot)
            {
                pivot.x *= width;
                pivot.y *= height;
            }

            if (textureData->frame)
            {
                pivot.x -= textureData->frame->x;
                pivot.y -= textureData->frame->y;
            }

            if (textureData->rotated)
            {
                pivot.x = pivot.x / textureData->region.height;
                pivot.y = 1.f - pivot.y / textureData->region.width;
            }
            else
            {
                pivot.x = pivot.x / textureData->region.width;
                pivot.y = 1.f - pivot.y / textureData->region.height;
            }

            frameDisplay->setSpriteFrame(textureData->texture);
            frameDisplay->setAnchorPoint(pivot);
            this->_updateVisible();
            return;
        }
    }

    frameDisplay->setTexture(nullptr);
    frameDisplay->setVisible(false);
    /*
    frameDisplay.readjustSize();
    frameDisplay.pivotX = 0;
    frameDisplay.pivotY = 0;
    */
}

void CCSlot::_updateMesh() 
{
}

void CCSlot::_updateTransform()
{
    if (_renderDisplay)
    {
        _renderDisplay->setScale(this->global.scaleX, this->global.scaleY);
        _renderDisplay->setRotationSkewX(this->global.skewX * RADIAN_TO_ANGLE);
        _renderDisplay->setRotationSkewY(this->global.skewY * RADIAN_TO_ANGLE);
        _renderDisplay->setPosition(this->global.x, -this->global.y);
    }
}

void CCSlot::_updateVisible()
{
    this->_renderDisplay->setVisible(this->_parent->getVisible());
}

void CCSlot::_updateBlendMode()
{
    cocos2d::Sprite* spriteDisplay = dynamic_cast<cocos2d::Sprite*>(_renderDisplay);
    if (spriteDisplay)
    {
        switch (this->_blendMode)
        {
            case BlendMode::Normal:
                break;

            case BlendMode::Add:
            {
                const auto texture = spriteDisplay->getTexture();
                if (texture && texture->hasPremultipliedAlpha())
                {
                    cocos2d::BlendFunc blendFunc = { GL_ONE, GL_ONE };
                    spriteDisplay->setBlendFunc(blendFunc);
                }
                else
                {
                    spriteDisplay->setBlendFunc(cocos2d::BlendFunc::ADDITIVE);
                }
                break;
            }

            default:
                break;
        }
    }
    else if (this->_childArmature)
    {
        const auto& slots = this->_childArmature->getSlots();
        for (const auto slot : slots)
        {
            slot->_blendMode = this->_blendMode;
            slot->_updateBlendMode();
        }
    }
}

NAMESPACE_DRAGONBONES_END
