#include "Slot.h"

#include "../model/DragonBonesData.h"
#include "../model/UserData.h"
#include "../model/ArmatureData.h"
#include "../model/DisplayData.h"
#include "../model/BoundingBoxData.h"
#include "../model/TextureAtlasData.h"
#include "Armature.h"
#include "Bone.h"
#include "../animation/Animation.h"

DRAGONBONES_NAMESPACE_BEGIN

void Slot::_onClear()
{
    TransformObject::_onClear();

    std::vector<std::pair<void*, DisplayType>> disposeDisplayList;
    for (const auto& pair : this->_displayList)
    {
        if (
            pair.first != _rawDisplay && pair.first != _meshDisplay &&
            std::find(disposeDisplayList.cbegin(), disposeDisplayList.cend(), pair) == disposeDisplayList.cend()
        )
        {
            disposeDisplayList.push_back(pair);
        }
    }

    for (const auto& pair : disposeDisplayList)
    {
        if (pair.second == DisplayType::Armature)
        {
            static_cast<Armature*>(pair.first)->returnToPool();
        }
        else
        {
            _disposeDisplay(pair.first);
        }
    }

    if (_meshDisplay && _meshDisplay != _rawDisplay) 
    {
        _disposeDisplay(_meshDisplay);
    }

    if (_rawDisplay) {
        _disposeDisplay(_rawDisplay);
    }

    displayController = "";
    slotData = nullptr;

    _displayDirty = false;
    _zOrderDirty = false;
    _blendModeDirty = false;
    _colorDirty = false;
    _meshDirty = false;
    _transformDirty = false;
    _visible = true;
    _blendMode = BlendMode::Normal;
    _displayIndex = -1;
    _animationDisplayIndex = -1;
    _zOrder = 0;
    _cachedFrameIndex = -1;
    _pivotX = 0.0f;
    _pivotY = 0.0f;
    _localMatrix.identity();
    _colorTransform.identity();
    _ffdVertices.clear();
    _displayList.clear();
    _displayDatas.clear();
    _meshBones.clear();
    _rawDisplayDatas = nullptr; //
    _displayData = nullptr;
    _textureData = nullptr;
    _meshData = nullptr;
    _boundingBoxData = nullptr;
    _rawDisplay = nullptr;
    _meshDisplay = nullptr;
    _display = nullptr;
    _childArmature = nullptr;
    _cachedFrameIndices = nullptr;
}

void Slot::_updateDisplayData()
{
    const auto prevDisplayData = _displayData;
    const auto prevTextureData = _textureData;
    const auto prevMeshData = _meshData;
    const auto rawDisplayData = _displayIndex >= 0 && (unsigned)_displayIndex < _rawDisplayDatas->size() ? (*_rawDisplayDatas)[_displayIndex] : nullptr;

    if (_displayIndex >= 0 && (unsigned)_displayIndex < _displayDatas.size())
    {
        _displayData = _displayDatas[_displayIndex];
    }
    else
    {
        _displayData = nullptr;
    }

    // Update texture and mesh data.
    if (_displayData != nullptr)
    {
        if (_displayData->type == DisplayType::Image || _displayData->type == DisplayType::Mesh)
        {
            _textureData = static_cast<ImageDisplayData*>(_displayData)->texture;
            if (_displayData->type == DisplayType::Mesh)
            {
                _meshData = static_cast<MeshDisplayData*>(_displayData);
            }
            else if (rawDisplayData != nullptr && rawDisplayData->type == DisplayType::Mesh)
            {
                _meshData = static_cast<MeshDisplayData*>(rawDisplayData);
            }
            else
            {
                _meshData = nullptr;
            }
        }
        else
        {
            _textureData = nullptr;
            _meshData = nullptr;
        }
    }
    else
    {
        _textureData = nullptr;
        _meshData = nullptr;
    }

    // Update bounding box data.
    if (_displayData != nullptr && _displayData->type == DisplayType::BoundingBox)
    {
        _boundingBoxData = static_cast<BoundingBoxDisplayData*>(_displayData)->boundingBox;
    }
    else if (rawDisplayData != nullptr && rawDisplayData->type == DisplayType::BoundingBox)
    {
        _boundingBoxData = static_cast<BoundingBoxDisplayData*>(rawDisplayData)->boundingBox;
    }
    else
    {
        _boundingBoxData = nullptr;
    }

    if (_displayData != prevDisplayData || _textureData != prevTextureData || _meshData != prevMeshData)
    {
        // Update pivot offset.
        if (_meshData != nullptr)
        {
            _pivotX = 0.0f;
            _pivotY = 0.0f;
        }
        else if (_textureData != nullptr) 
        {
            const auto imageDisplayData = static_cast<ImageDisplayData*>(_displayData);
            const auto scale = _armature->armatureData->scale;
            const auto frame = _textureData->frame;

            _pivotX = imageDisplayData->pivot.x;
            _pivotY = imageDisplayData->pivot.y;

            const auto& rect = frame != nullptr ? *frame : _textureData->region;
            float width = rect.width * scale;
            float height = rect.height * scale;

            if (_textureData->rotated && frame == nullptr) 
            {
                width = rect.height;
                height = rect.width;
            }

            _pivotX *= width;
            _pivotY *= height;

            if (frame != nullptr)
            {
                _pivotX += frame->x * scale;
                _pivotY += frame->y * scale;
            }
        }
        else
        {
            _pivotX = 0.0f;
            _pivotY = 0.0f;
        }

        // Update mesh bones and ffd vertices.
        if (_meshData != prevMeshData)
        {
            if (_meshData != nullptr)
            {
                if (_meshData->weight != nullptr)
                {
                    _ffdVertices.resize(_meshData->weight->count * 2);
                    _meshBones.resize(_meshData->weight->bones.size());

                    for (std::size_t i = 0, l = _meshBones.size(); i < l; ++i)
                    {
                        _meshBones[i] = _armature->getBone(_meshData->weight->bones[i]->name);
                    }
                }
                else
                {
                    const auto vertexCount = (_meshData->parent->parent->intArray)[_meshData->offset + (unsigned)BinaryOffset::MeshVertexCount];
                    _ffdVertices.resize(vertexCount * 2);
                    _meshBones.resize(0);
                }

                for (std::size_t i = 0, l = _ffdVertices.size(); i < l; ++i)
                {
                    _ffdVertices[i] = 0.0f;
                }

                _meshDirty = true;
            }
            else
            {
                _ffdVertices.resize(0);
                _meshBones.resize(0);
            }
        }
        else if (_meshData != nullptr && _textureData != prevTextureData) // Update mesh after update frame.
        {
            _meshDirty = true;
        }

        if (_displayData != nullptr && rawDisplayData != nullptr && _displayData != rawDisplayData && _meshData == nullptr)
        {
            rawDisplayData->transform.toMatrix(_helpMatrix);
            _helpMatrix.invert();
            _helpMatrix.transformPoint(0.0f, 0.0f, _helpPoint);
            _pivotX -= _helpPoint.x;
            _pivotY -= _helpPoint.y;

            _displayData->transform.toMatrix(_helpMatrix);
            _helpMatrix.invert();
            _helpMatrix.transformPoint(0.0f, 0.0f, _helpPoint);
            _pivotX += _helpPoint.x;
            _pivotY += _helpPoint.y;
        }

        // Update original transform.
        if (rawDisplayData != nullptr)
        {
            origin = &rawDisplayData->transform;
        }
        else if (_displayData != nullptr)
        {
            origin = &_displayData->transform;
        }

        _displayDirty = true;
        _transformDirty = true;
    }
}

void Slot::_updateDisplay()
{
    const auto prevDisplay = _display != nullptr ? _display : _rawDisplay;
    const auto prevChildArmature = _childArmature;

    // Update display and child armature.
    if (_displayIndex >= 0 && (std::size_t)_displayIndex < _displayList.size())
    {
        const auto& displayPair = _displayList[_displayIndex];
        _display = displayPair.first;
        if (_display != nullptr && displayPair.second == DisplayType::Armature)
        {
            _childArmature = static_cast<Armature*>(displayPair.first);
            _display = _childArmature->getDisplay();
        }
        else
        {
            _childArmature = nullptr;
        }
    }
    else
    {
        _display = nullptr;
        _childArmature = nullptr;
    }

    const auto currentDisplay = _display != nullptr ? _display : _rawDisplay;
    if (currentDisplay != prevDisplay)
    {
        _onUpdateDisplay();
        _replaceDisplay(prevDisplay, prevChildArmature != nullptr);

        _visibleDirty = true;
        _blendModeDirty = true;
        _colorDirty = true;
    }

    // Update frame.
    if (currentDisplay == _rawDisplay || currentDisplay == _meshDisplay)
    {
        _updateFrame();
    }

    // Update child armature.
    if (_childArmature != prevChildArmature)
    {
        if (prevChildArmature != nullptr)
        {
            prevChildArmature->_parent = nullptr; // Update child armature parent.
            prevChildArmature->setClock(nullptr);
            if (prevChildArmature->inheritAnimation)
            {
                prevChildArmature->getAnimation()->reset();
            }
        }

        if (_childArmature != nullptr)
        {
            _childArmature->_parent = this; // Update child armature parent.
            _childArmature->setClock(_armature->getClock());
            if (_childArmature->inheritAnimation) // Set child armature cache frameRate.
            {
                if (_childArmature->getCacheFrameRate() == 0)
                {
                    const auto chacheFrameRate = this->_armature->getCacheFrameRate();
                    if (chacheFrameRate != 0)
                    {
                        _childArmature->setCacheFrameRate(chacheFrameRate);
                    }
                }

                // Child armature action.
                std::vector<ActionData*>* actions = nullptr;
                if (_displayData != nullptr && _displayData->type == DisplayType::Armature) {
                    actions = &(static_cast<ArmatureDisplayData*>(_displayData)->actions);
                }
                else 
                {
                    const auto rawDisplayData = ( _displayIndex >= 0 && (unsigned)_displayIndex < _rawDisplayDatas->size()) ? (*_rawDisplayDatas)[_displayIndex] : nullptr;
                    if (rawDisplayData != nullptr && rawDisplayData->type == DisplayType::Armature) 
                    {
                        actions = &(static_cast<ArmatureDisplayData*>(rawDisplayData)->actions);
                    }
                }

                if (actions != nullptr && !actions->empty())
                {
                    for (const auto action : *actions)
                    {
                        _childArmature->_bufferAction(action, false);
                    }
                }
                else
                {
                    _childArmature->getAnimation()->play();
                }
            }
        }
    }
}

void Slot::_updateGlobalTransformMatrix(bool isCache)
{
    globalTransformMatrix = _localMatrix; // Copy.
    globalTransformMatrix.concat(_parent->globalTransformMatrix);
    if (isCache)
    {
        global.fromMatrix(globalTransformMatrix);
    }
    else
    {
        _globalDirty = true;
    }
}

bool Slot::_isMeshBonesUpdate() const
{
    for (const auto bone : _meshBones)
    {
        if (bone != nullptr && bone->_childrenTransformDirty)
        {
            return true;
        }
    }

    return false;
}

void Slot::_setArmature(Armature* value)
{
    if (this->_armature == value)
    {
        return;
    }

    if (this->_armature)
    {
        this->_armature->_removeSlotFromSlotList(this);
    }

    this->_armature = value;

    _onUpdateDisplay();

    if (this->_armature)
    {
        this->_armature->_addSlotToSlotList(this);
        _addDisplay();
    }
    else
    {
        _removeDisplay();
    }
}

bool Slot::_setDisplayIndex(int value, bool isAnimation)
{
    if (isAnimation)
    {
        if (_animationDisplayIndex == value)
        {
            return false;
        }

        _animationDisplayIndex = value;
    }

    if (_displayIndex == value)
    {
        return false;
    }

    _displayIndex = value;
    _displayDirty = true;

    _updateDisplayData();

    return _displayDirty;
}

bool Slot::_setZorder(int value)
{
    if (_zOrder == value)
    {
        //return false;
    }

    _zOrder = value;
    _zOrderDirty = true;

    return _zOrderDirty;
}

bool Slot::_setColor(const ColorTransform& value)
{
    _colorTransform = value; // copy
    _colorDirty = true;

    return true;
}

bool Slot::_setDisplayList(const std::vector<std::pair<void*, DisplayType>>& value)
{
    if (!value.empty())
    {
        if (_displayList.size() != value.size())
        {
            _displayList.resize(value.size());
        }

        for (std::size_t i = 0, l = value.size(); i < l; ++i)
        {
            const auto& eachPair = value[i];
            if (
                eachPair.first && eachPair.first != _rawDisplay && eachPair.first != _meshDisplay &&
                eachPair.second != DisplayType::Armature
                && std::find(_displayList.cbegin(), _displayList.cend(), eachPair) == _displayList.cend()
                )
            {
                _initDisplay(eachPair.first);
            }

            _displayList[i].first = eachPair.first;
            _displayList[i].second = eachPair.second;
        }
    }
    else if (!_displayList.empty())
    {
        _displayList.clear();
    }

    if (_displayIndex >= 0 && (std::size_t)_displayIndex < _displayList.size())
    {
        _displayDirty = _display != _displayList[_displayIndex].first;
    }
    else
    {
        _displayDirty = _display != nullptr;
    }

    _updateDisplayData();

    return _displayDirty;
}

void Slot::init(SlotData* pslotData, std::vector<DisplayData*>* displayDatas, void* rawDisplay, void* meshDisplay)
{
    if (pslotData == nullptr)
    {
        return;
    }

    slotData = pslotData;
    name = slotData->name;

    _visibleDirty = true;
    _blendModeDirty = true;
    _colorDirty = true;
    _blendMode = slotData->blendMode;
    _zOrder = slotData->zOrder;
    _colorTransform = *(slotData->color);
    _rawDisplayDatas = displayDatas;
    _rawDisplay = rawDisplay;
    _meshDisplay = meshDisplay;

    _displayDatas.resize(_rawDisplayDatas->size());
    for (std::size_t i = 0, l = _displayDatas.size(); i < l; ++i)
    {
        _displayDatas[i] = (*_rawDisplayDatas)[i];
    }
}

void Slot::update(int cacheFrameIndex)
{
    if (_displayDirty)
    {
        _displayDirty = false;
        _updateDisplay();

        if (_transformDirty) // Update local matrix. (Only updated when both display and transform are dirty.)
        {
            if (origin != nullptr) 
            {
                global = *origin;
                global.add(offset).toMatrix(_localMatrix);
            }
            else 
            {
                global = offset;
                global.toMatrix(_localMatrix);
            }
        }
    }

    if (_zOrderDirty) 
    {
        _zOrderDirty = false;
        _updateZOrder();
    }

    if (cacheFrameIndex >= 0 && _cachedFrameIndices != nullptr)
    {
        const auto cachedFrameIndex = (*_cachedFrameIndices)[cacheFrameIndex];
        if (cachedFrameIndex >= 0 && _cachedFrameIndex == cachedFrameIndex) // Same cache.
        {
            _transformDirty = false;
        }
        else if (cachedFrameIndex >= 0) // Has been Cached.
        {
            _transformDirty = true;
            _cachedFrameIndex = cachedFrameIndex;
        }
        else if (_transformDirty || _parent->_childrenTransformDirty) // Dirty.
        {
            _transformDirty = true;
            _cachedFrameIndex = -1;
        }
        else if (_cachedFrameIndex >= 0) // Same cache, but not set index yet.
        {
            _transformDirty = false;
            (*_cachedFrameIndices)[cacheFrameIndex] = _cachedFrameIndex;
        }
        else // Dirty.
        {
            _transformDirty = true;
            _cachedFrameIndex = -1;
        }
    }
    else if (_transformDirty || this->_parent->_childrenTransformDirty)
    {
        cacheFrameIndex = -1;
        _transformDirty = true;
        _cachedFrameIndex = -1;
    }

    if (_display == nullptr)
    {
        return;
    }

    if (_blendModeDirty)
    {
        _blendModeDirty = false;
        _updateBlendMode();
    }

    if (_colorDirty)
    {
        _colorDirty = false;
        _updateColor();
    }

    if (_meshData != nullptr && _display == _meshDisplay)
    {
        const auto isSkinned = _meshData->weight != nullptr;
        if (_meshDirty || (isSkinned && _isMeshBonesUpdate())) 
        {
            _meshDirty = false;
            _updateMesh();
        }

        if (isSkinned) 
        {
            if (_transformDirty) 
            {
                _transformDirty = false;
                _updateTransform(true);
            }

            return;
        }
    }

    if (_transformDirty)
    {
        _transformDirty = false;

        if (_cachedFrameIndex < 0)
        {
            const auto isCache = cacheFrameIndex >= 0;
            _updateGlobalTransformMatrix(isCache);

            if (isCache && _cachedFrameIndices != nullptr)
            {
                _cachedFrameIndex = (*_cachedFrameIndices)[cacheFrameIndex] = _armature->armatureData->setCacheFrame(globalTransformMatrix, global);
            }
        }
        else
        {
            _armature->armatureData->getCacheFrame(globalTransformMatrix, global, _cachedFrameIndex);
        }

        _updateTransform(false);
    }
}

bool Slot::containsPoint(float x, float y)
{
    if (_boundingBoxData == nullptr) 
    {
        return false;
    }

    updateTransformAndMatrix();

    _helpMatrix = globalTransformMatrix; // Copy.
    _helpMatrix.invert();
    _helpMatrix.transformPoint(x, y, _helpPoint);

    return _boundingBoxData->containsPoint(_helpPoint.x, _helpPoint.y);
}

int Slot::intersectsSegment(
    float xA, float yA, float xB, float yB,
    Point* intersectionPointA,
    Point* intersectionPointB,
    Point* normalRadians
)
{
    if (_boundingBoxData == nullptr) 
    {
        return 0;
    }

    updateTransformAndMatrix();
    _helpMatrix = globalTransformMatrix;
    _helpMatrix.invert();
    _helpMatrix.transformPoint(xA, yA, _helpPoint);
    xA = _helpPoint.x;
    yA = _helpPoint.y;
    _helpMatrix.transformPoint(xB, yB, _helpPoint);
    xB = _helpPoint.x;
    yB = _helpPoint.y;

    const auto intersectionCount = _boundingBoxData->intersectsSegment(xA, yA, xB, yB, intersectionPointA, intersectionPointB, normalRadians);
    if (intersectionCount > 0)
    {
        if (intersectionCount == 1 || intersectionCount == 2) 
        {
            if (intersectionPointA == nullptr) 
            {
                globalTransformMatrix.transformPoint(intersectionPointA->x, intersectionPointA->y, *intersectionPointA);
                if (intersectionPointB == nullptr) 
                {
                    intersectionPointB->x = intersectionPointA->x;
                    intersectionPointB->y = intersectionPointA->y;
                }
            }
            else if (intersectionPointB == nullptr) 
            {
                globalTransformMatrix.transformPoint(intersectionPointB->x, intersectionPointB->y, *intersectionPointB);
            }
        }
        else 
        {
            if (intersectionPointA == nullptr) 
            {
                globalTransformMatrix.transformPoint(intersectionPointA->x, intersectionPointA->y, *intersectionPointA);
            }

            if (intersectionPointB == nullptr) 
            {
                globalTransformMatrix.transformPoint(intersectionPointB->x, intersectionPointB->y, *intersectionPointB);
            }
        }

        if (normalRadians == nullptr)
        {
            globalTransformMatrix.transformPoint(cos(normalRadians->x), sin(normalRadians->x), _helpPoint, true);
            normalRadians->x = atan2(_helpPoint.y, _helpPoint.x);

            globalTransformMatrix.transformPoint(cos(normalRadians->y), sin(normalRadians->y), _helpPoint, true);
            normalRadians->y = atan2(_helpPoint.y, _helpPoint.x);
        }
    }

    return intersectionCount;
}

void Slot::setDisplayIndex(int value)
{
    if (_setDisplayIndex(value)) 
    {
        update(-1);
    }
}

//TODO lsc check
void Slot::setDisplayList(const std::vector<std::pair<void*, DisplayType>>& value)
{
    const auto backupDisplayList = _displayList; // copy
    auto disposeDisplayList = backupDisplayList; // copy
    disposeDisplayList.clear();

    if (_setDisplayList(value))
    {
        update(-1);
    }

    for (const auto& pair : backupDisplayList)
    {
        if (
            pair.first && pair.first != _rawDisplay && pair.first != _meshDisplay &&
            std::find(_displayList.cbegin(), _displayList.cend(), pair) == _displayList.cend() &&
            std::find(disposeDisplayList.cbegin(), disposeDisplayList.cend(), pair) == disposeDisplayList.cend()
        )
        {
            disposeDisplayList.push_back(pair);
        }
    }

    for (const auto& pair : disposeDisplayList)
    {
        if (pair.second == DisplayType::Armature)
        {
            static_cast<Armature*>(pair.first)->returnToPool();
        }
        else
        {
            _disposeDisplay(pair.first);
        }
    }
}

void Slot::setDisplay(void* value, DisplayType displayType)
{
    if (_display == value)
    {
        return;
    }

    const auto displayListLength = _displayList.size();
    if (_displayIndex < 0 && displayListLength == 0)  // Emprty
    {
        _displayIndex = 0;
    }

    if (_displayIndex < 0)
    {
        return;
    }
    else
    {
        auto relpaceDisplayList = _displayList; // copy
        if (displayListLength <= (std::size_t)_displayIndex)
        {
            relpaceDisplayList.resize(_displayIndex + 1);
        }

        relpaceDisplayList[_displayIndex].first = value;
        relpaceDisplayList[_displayIndex].second = displayType;

        setDisplayList(relpaceDisplayList);
    }
}

void Slot::setChildArmature(Armature* value)
{
    if (_childArmature == value)
    {
        return;
    }

    setDisplay(value, DisplayType::Armature);
}

void Slot::switchDisplayData(std::vector<DisplayData *> * displayDatas)
{
    _rawDisplayDatas = displayDatas;
    _displayDatas.resize(_rawDisplayDatas->size());
    for (std::size_t i = 0, l = _displayDatas.size(); i < l; ++i) {
        _displayDatas[i] = (*_rawDisplayDatas)[i];
    }
}

void Slot::replaceDisplayData(DisplayData *displayData, unsigned displayIndex)
{
    if (_displayDatas.size() <= displayIndex) {
        _displayDatas.resize(displayIndex + 1);
    }

    _displayDatas[displayIndex] = displayData;
}


DRAGONBONES_NAMESPACE_END
