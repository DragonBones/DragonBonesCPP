#include "Slot.h"

NAME_SPACE_DRAGON_BONES_BEGIN
int Slot::getDisplayIndex() const
{
    return _displayIndex;
}

SlotData* Slot::getSlotData() const
{
    return _slotData;
}

float Slot::getZOrder() const
{
    return _originZOrder + _tweenZOrder + _offsetZOrder;
}
void Slot::setZOrder(float value)
{
    if (getZOrder() != value)
    {
        _offsetZOrder = value - _originZOrder - _tweenZOrder;
        
        if (_armature)
        {
            _armature->_slotsZOrderChanged = true;
        }
    }
}

void* Slot::getDisplay() const
{
    return _display;
}
void Slot::setDisplay(void *display, DisplayType displayType, bool disposeExisting)
{
    if (_displayIndex < 0)
    {
        _isShowDisplay = true;
        _displayIndex = 0;
    }
    
    if (_displayIndex >= (int)(_displayList.size()))
    {
        _displayList.resize(_displayIndex + 1);
    }
    
    if (_displayList[_displayIndex].first == display)
    {
        return;
    }
    
    _displayList[_displayIndex].first = display;
    _displayList[_displayIndex].second = displayType;
    updateSlotDisplay(disposeExisting);
}

Armature* Slot::getChildArmature() const
{
    return _childArmature;
}
void Slot::setChildArmature(Armature *childArmature, bool disposeExisting)
{
    setDisplay(childArmature, DisplayType::DT_ARMATURE, disposeExisting);
}

const std::vector<std::pair<void*, DisplayType>>& Slot::getDisplayList() const
{
    return _displayList;
}
void Slot::setDisplayList(const std::vector<std::pair<void*, DisplayType>> &displayList, bool disposeExisting)
{
    if (_displayIndex < 0)
    {
        _isShowDisplay = true;
        _displayIndex = 0;
    }
    
    if (disposeExisting)
    {
        disposeDisplayList();
        _childArmature = nullptr;
        _display = nullptr;
    }
    
    // copy
    _displayList = displayList;
    int displayIndexBackup = _displayIndex;
    _displayIndex = -1;
    changeDisplay(displayIndexBackup);
}

void Slot::setVisible(bool visible)
{
    if (_visible != visible)
    {
        _visible = visible;
        updateDisplayVisible(_visible);
    }
}

void Slot::setArmature(Armature *armature)
{
    Object::setArmature(armature);
    
    if (_armature)
    {
        _armature->_slotsZOrderChanged = true;
        addDisplayToContainer(_armature->_display, -1);
    }
    else
    {
        removeDisplayFromContainer();
    }
}

Slot::Slot(SlotData *slotData)
{
    _isShowDisplay = false;
    _displayIndex = -1;
    _originZOrder = 0.f;
    _tweenZOrder = 0.f;
    _offsetZOrder = 0.f;
    _blendMode = BlendMode::BM_NORMAL;
    //_colorTransform
    _slotData = slotData;
    _childArmature = nullptr;
    _display = nullptr;
    inheritRotation = true;
    inheritScale = true;
}
Slot::~Slot()
{
    dispose();
}
void Slot::dispose()
{
    Object::dispose();
    //
    _displayList.clear();
    _slotData = nullptr;
    _childArmature = nullptr;
    _display = nullptr;
}

void Slot::update()
{
    if (_parent->_needUpdate <= 0)
    {
        return;
    }
    
    const float x = origin.x + offset.x + _parent->_tweenPivot.x;
    const float y = origin.y + offset.y + _parent->_tweenPivot.y;
    const Matrix &parentMatrix = _parent->globalTransformMatrix;
    globalTransformMatrix.tx = global.x = parentMatrix.a * x + parentMatrix.c * y + parentMatrix.tx;
    globalTransformMatrix.ty = global.y = parentMatrix.d * y + parentMatrix.b * x + parentMatrix.ty;
    //globalTransformMatrix.tx = global.x = parentMatrix.a * x * _parent->global.scaleX + parentMatrix.c * y * _parent->global.scaleY + parentMatrix.tx;
    //globalTransformMatrix.ty = global.y = parentMatrix.d * y * _parent->global.scaleY + parentMatrix.b * x * _parent->global.scaleX + parentMatrix.ty;
    
    if (inheritRotation)
    {
        global.skewX = origin.skewX + offset.skewX + _parent->global.skewX;
        global.skewY = origin.skewY + offset.skewY + _parent->global.skewY;
    }
    else
    {
        global.skewX = origin.skewX + offset.skewX;
        global.skewY = origin.skewY + offset.skewY;
    }
    
    if (inheritScale)
    {
        global.scaleX = origin.scaleX * offset.scaleX * _parent->global.scaleX;
        global.scaleY = origin.scaleY * offset.scaleY * _parent->global.scaleY;
    }
    else
    {
        global.scaleX = origin.scaleX * offset.scaleX;
        global.scaleY = origin.scaleY * offset.scaleY;
    }
    
    globalTransformMatrix.a = global.scaleX * cos(global.skewY);
    globalTransformMatrix.b = global.scaleX * sin(global.skewY);
    globalTransformMatrix.c = -global.scaleY * sin(global.skewX);
    globalTransformMatrix.d = global.scaleY * cos(global.skewX);
    updateDisplayTransform();
}

void Slot::changeDisplay(int displayIndex)
{
    if (displayIndex < 0)
    {
        if (_isShowDisplay)
        {
            _isShowDisplay = false;
            removeDisplayFromContainer();
            updateChildArmatureAnimation();
        }
    }
    else if (!_displayList.empty())
    {
        if (displayIndex >= (int)(_displayList.size()))
        {
            displayIndex = _displayList.size() - 1;
        }
        
        if (_displayIndex != displayIndex)
        {
            _isShowDisplay = true;
            _displayIndex = displayIndex;
            updateSlotDisplay(false);
            
            if (
                _slotData &&
                !_slotData->displayDataList.empty() &&
                _displayIndex < (int)(_slotData->displayDataList.size())
            )
            {
                origin = _slotData->displayDataList[_displayIndex]->transform;
            }
        }
        else if (!_isShowDisplay)
        {
            _isShowDisplay = true;
            
            if (_armature)
            {
                _armature->_slotsZOrderChanged = true;
                addDisplayToContainer(_armature->_display, -1);
            }
            
            updateChildArmatureAnimation();
        }
    }
}

void Slot::updateChildArmatureAnimation()
{
    if (_isShowDisplay)
    {
        playChildArmatureAnimation();
    }
    else
    {
        stopChildArmatureAnimation();
    }
}

void Slot::playChildArmatureAnimation()
{
    if (_childArmature && _childArmature->_isInheritAnimation)
    {
        if (
            _armature &&
            _armature->_animation->_lastAnimationState &&
            _childArmature->_animation->hasAnimation(_armature->_animation->_lastAnimationState->name)
            )
        {
            _childArmature->_animation->gotoAndPlay(_armature->_animation->_lastAnimationState->name);
        }
        else
        {
            _childArmature->_animation->play();
        }
    }
}

void Slot::stopChildArmatureAnimation()
{
    if (_childArmature)
    {
        _childArmature->_animation->stop();
        _childArmature->_animation->_lastAnimationState = nullptr;
    }
}

void Slot::updateSlotDisplay(bool disposeExisting)
{
    int currentDisplayIndex = -1;
    
    if (_display)
    {
        currentDisplayIndex = getDisplayZIndex();
        removeDisplayFromContainer();
    }

    if (disposeExisting)
    {
        if (_childArmature)
        {
            _childArmature->dispose();
            delete _childArmature;
            _childArmature = nullptr;
        }
        else if (_display)
        {
            disposeDisplay();
            _display = nullptr;
        }
    }

    stopChildArmatureAnimation();

    void *display = _displayList[_displayIndex].first;
    DisplayType displayType = _displayList[_displayIndex].second;
    
    if (display)
    {
        if (displayType == DisplayType::DT_ARMATURE)
        {
            _childArmature = static_cast<Armature*>(display);
            _display = _childArmature->_display;
        }
        else
        {
            _childArmature = nullptr;
            _display = display;
        }
    }
    else
    {
        _display = nullptr;
        _childArmature = nullptr;
    }

    playChildArmatureAnimation();
    
    updateDisplay(_display);
    
    if (_display)
    {
        if (_armature && _isShowDisplay)
        {
            if (currentDisplayIndex < 0)
            {
                _armature->_slotsZOrderChanged = true;
                addDisplayToContainer(_armature->_display, currentDisplayIndex);
            }
            else
            {
                addDisplayToContainer(_armature->_display, currentDisplayIndex);
            }
        }
        
        if (_blendMode != BlendMode::BM_NORMAL)
        {
            updateDisplayBlendMode(_blendMode);
        }
        else if (_slotData)
        {
            updateDisplayBlendMode(_slotData->blendMode);
        }
        
        updateDisplayColor(
            _colorTransform.alphaOffset, _colorTransform.redOffset, _colorTransform.greenOffset, _colorTransform.blueOffset,
            _colorTransform.alphaMultiplier, _colorTransform.redMultiplier, _colorTransform.greenMultiplier, _colorTransform.blueMultiplier
            );
        updateDisplayVisible(_visible);
        updateDisplayTransform();
    }
}

void Slot::updateDisplayColor(int aOffset, int rOffset, int gOffset, int bOffset, float aMultiplier, float rMultiplier, float gMultiplier, float bMultiplier)
{
    _colorTransform.alphaOffset = aOffset;
    _colorTransform.redOffset = rOffset;
    _colorTransform.greenOffset = gOffset;
    _colorTransform.blueOffset = bOffset;
    _colorTransform.alphaMultiplier = aMultiplier;
    _colorTransform.redMultiplier = rMultiplier;
    _colorTransform.greenMultiplier = gMultiplier;
    _colorTransform.blueMultiplier = bMultiplier;
}
NAME_SPACE_DRAGON_BONES_END
