#include "TimelineState.h"
#include "AnimationState.h"
#include "../armature/Bone.h"
#include "../armature/Slot.h"

NAMESPACE_DRAGONBONES_BEGIN

AnimationTimelineState::AnimationTimelineState() 
{
    _onClear();
}
AnimationTimelineState::~AnimationTimelineState() 
{
    _onClear();
}

void AnimationTimelineState::_onClear()
{
    _isStarted = false;
}

void AnimationTimelineState::_onCrossFrame(FrameData * frame)
{
}

void AnimationTimelineState::update(int time)
{
    TimelineState::update(time);
}

BoneTimelineState::BoneTimelineState() 
{
    _onClear();
}
BoneTimelineState::~BoneTimelineState() 
{
    _onClear();
}

void BoneTimelineState::_onClear()
{
    TweenTimelineState::_onClear();

    bone = nullptr;

    _tweenTransform = TweenType::None;
    _tweenRotate = TweenType::None;
    _tweenScale = TweenType::None;
    _boneTransform = nullptr;
    _originTransform = nullptr;
    _transform.identity();
    _currentTransform.identity();
    _durationTransform.identity();
}

void BoneTimelineState::_onFadeIn()
{
    _originTransform = &static_cast<BoneTimelineData*>(this->_timeline)->originTransform;
    _boneTransform = &bone->_animationPose;
}

void BoneTimelineState::_onArriveAtFrame(bool isUpdate)
{
    TweenTimelineState::_onArriveAtFrame(isUpdate);

    const auto& currentFrame = *reinterpret_cast<BoneFrameData*>(this->_currentFrame);

    _currentTransform = currentFrame.transform; // copy
    _tweenTransform = TweenType::Once;
    _tweenRotate = TweenType::Once;
    _tweenScale = TweenType::Once;

    if (this->_keyFrameCount > 1 && (this->_tweenEasing != TweenFrameData::NO_TWEEN || this->_curve))
    {
        const auto& nextFrame = *reinterpret_cast<BoneFrameData*>(this->_currentFrame->next);
        const auto& nextTransform = nextFrame.transform;

        // Transform
        _durationTransform.x = nextTransform.x - _currentTransform.x;
        _durationTransform.y = nextTransform.y - _currentTransform.y;
        if (_durationTransform.x != 0.f || _durationTransform.y != 0.f)
        {
            _tweenTransform = TweenType::Always;
        }

        // Rotate
        if (currentFrame.tweenRotate == currentFrame.tweenRotate)
        {
            if (currentFrame.tweenRotate)
            {
                if (currentFrame.tweenRotate > 0 ? nextTransform.skewY >= _currentTransform.skewY : nextTransform.skewY <= _currentTransform.skewY) {
                    const auto rotate = currentFrame.tweenRotate > 0 ? currentFrame.tweenRotate - 1 : currentFrame.tweenRotate + 1;
                    _durationTransform.skewX = nextTransform.skewX - _currentTransform.skewX + PI_D * rotate;
                    _durationTransform.skewY = nextTransform.skewY - _currentTransform.skewY + PI_D * rotate;
                }
                else
                {
                    _durationTransform.skewX = nextTransform.skewX - _currentTransform.skewX + PI_D * currentFrame.tweenRotate;
                    _durationTransform.skewY = nextTransform.skewY - _currentTransform.skewY + PI_D * currentFrame.tweenRotate;
                }
            }
            else
            {
                _durationTransform.skewX = Transform::normalizeRadian(nextTransform.skewX - _currentTransform.skewX);
                _durationTransform.skewY = Transform::normalizeRadian(nextTransform.skewY - _currentTransform.skewY);
            }

            if (_durationTransform.skewX != 0.f || _durationTransform.skewY != 0.f)
            {
                _tweenRotate = TweenType::Always;
            }
        }
        else
        {
            _durationTransform.skewX = 0.f;
            _durationTransform.skewY = 0.f;
        }

        // Scale
        if (currentFrame.tweenScale)
        {
            _durationTransform.scaleX = nextTransform.scaleX - _currentTransform.scaleX;
            _durationTransform.scaleY = nextTransform.scaleY - _currentTransform.scaleY;
            if (_durationTransform.scaleX != 0.f || _durationTransform.scaleY != 0.f)
            {
                _tweenScale = TweenType::Always;
            }
        }
        else
        {
            _durationTransform.scaleX = 0.f;
            _durationTransform.scaleY = 0.f;
        }
    }
    else
    {
        _durationTransform.x = 0.f;
        _durationTransform.y = 0.f;
        _durationTransform.skewX = 0.f;
        _durationTransform.skewY = 0.f;
        _durationTransform.scaleX = 0.f;
        _durationTransform.scaleY = 0.f;
    }
}

void BoneTimelineState::_onUpdateFrame(bool isUpdate)
{
    if (_tweenTransform != TweenType::None || _tweenRotate != TweenType::None || _tweenScale != TweenType::None)
    {
        TweenTimelineState::_onUpdateFrame(isUpdate);

        if (_tweenTransform != TweenType::None)
        {
            if (_tweenTransform == TweenType::Once)
            {
                _tweenTransform = TweenType::None;
            }

            if (this->_animationState->additiveBlending) // Additive blending
            {
                _transform.x = _currentTransform.x + _durationTransform.x * this->_tweenProgress;
                _transform.y = _currentTransform.y + _durationTransform.y * this->_tweenProgress;
            }
            else // Normal blending
            {
                _transform.x = _originTransform->x + _currentTransform.x + _durationTransform.x * this->_tweenProgress;
                _transform.y = _originTransform->y + _currentTransform.y + _durationTransform.y * this->_tweenProgress;
            }
        }

        if (_tweenRotate != TweenType::None)
        {
            if (_tweenRotate == TweenType::Once)
            {
                _tweenRotate = TweenType::None;
            }

            if (this->_animationState->additiveBlending) // Additive blending
            {
                _transform.skewX = _currentTransform.skewX + _durationTransform.skewX * this->_tweenProgress;
                _transform.skewY = _currentTransform.skewY + _durationTransform.skewY * this->_tweenProgress;
            }
            else // Normal blending
            {
                _transform.skewX = _originTransform->skewX + _currentTransform.skewX + _durationTransform.skewX * this->_tweenProgress;
                _transform.skewY = _originTransform->skewY + _currentTransform.skewY + _durationTransform.skewY * this->_tweenProgress;
            }
        }

        if (_tweenScale != TweenType::None)
        {
            if (_tweenScale == TweenType::Once)
            {
                _tweenScale = TweenType::None;
            }

            if (this->_animationState->additiveBlending) // Additive blending
            {
                _transform.scaleX = _currentTransform.scaleX + _durationTransform.scaleX * this->_tweenProgress;
                _transform.scaleY = _currentTransform.scaleY + _durationTransform.scaleY * this->_tweenProgress;
            }
            else // Normal blending
            {
                _transform.scaleX = _originTransform->scaleX * (_currentTransform.scaleX + _durationTransform.scaleX * this->_tweenProgress);
                _transform.scaleY = _originTransform->scaleY * (_currentTransform.scaleY + _durationTransform.scaleY * this->_tweenProgress);
            }
        }

        bone->invalidUpdate();
    }
}

void BoneTimelineState::fadeOut()
{
    _transform.skewX = Transform::normalizeRadian(_transform.skewX);
    _transform.skewY = Transform::normalizeRadian(_transform.skewY);
}

void BoneTimelineState::update(int time)
{
    TweenTimelineState::update(time);

    const auto weight = this->_animationState->_weightResult;
    if (weight > 0.f)
    {
        if (this->_animationState->_index <= 1)
        {
            _boneTransform->x = _transform.x * weight;
            _boneTransform->y = _transform.y * weight;
            _boneTransform->skewX = _transform.skewX * weight;
            _boneTransform->skewY = _transform.skewY * weight;
            _boneTransform->scaleX = (_transform.scaleX - 1.f) * weight + 1.f;
            _boneTransform->scaleY = (_transform.scaleY - 1.f) * weight + 1.f;
        }
        else
        {
            _boneTransform->x += _transform.x * weight;
            _boneTransform->y += _transform.y * weight;
            _boneTransform->skewX += _transform.skewX * weight;
            _boneTransform->skewY += _transform.skewY * weight;
            _boneTransform->scaleX += (_transform.scaleX - 1.f) * weight;
            _boneTransform->scaleY += (_transform.scaleY - 1.f) * weight;
        }

        const auto fadeProgress = this->_animationState->_fadeProgress;
        if (fadeProgress < 1.f)
        {
            bone->invalidUpdate();
        }
    }
}

SlotTimelineState::SlotTimelineState() 
{
    _onClear();
}
SlotTimelineState::~SlotTimelineState() 
{
    _onClear();
}

void SlotTimelineState::_onClear()
{
    TweenTimelineState::_onClear();

    slot = nullptr;

    _tweenColor = TweenType::None;
    _slotColor = nullptr;
    _durationColor.identity();
}

void SlotTimelineState::_onFadeIn()
{
    _slotColor = &slot->_colorTransform;
}

void SlotTimelineState::_onArriveAtFrame(bool isUpdate)
{
    TweenTimelineState::_onArriveAtFrame(isUpdate);

    if (this->_animationState->_isDisabled(*slot))
    {
        this->_tweenEasing = TweenFrameData::NO_TWEEN;
        this->_curve = nullptr;
        _tweenColor = TweenType::None;
        return;
    }

    const auto& currentFrame = *reinterpret_cast<SlotFrameData*>(this->_currentFrame);

    if (slot->_displayDataSet)
    {
        const auto displayIndex = currentFrame.displayIndex;
        if (slot->getDisplayIndex() >= 0 && displayIndex >= 0)
        {
            if (!slot->_displayDataSet->displays.empty())
            {
                slot->_setDisplayIndex(displayIndex);
            }
        }
        else
        {
            slot->_setDisplayIndex(displayIndex);
        }

        slot->_updateMeshData(true);
    }

    if (slot->getDisplayIndex() >= 0)
    {
        _tweenColor = TweenType::None;

        const auto& nextFrame = *static_cast<SlotFrameData*>(this->_currentFrame->next);
        const auto& currentColor = *currentFrame.color;

        if (this->_keyFrameCount > 1 && (this->_tweenEasing != TweenFrameData::NO_TWEEN || this->_curve))
        {
            if (currentFrame.color != nextFrame.color)
            {
                const auto& nextColor = *nextFrame.color;
                _durationColor.alphaMultiplier = nextColor.alphaMultiplier - currentColor.alphaMultiplier;
                _durationColor.redMultiplier = nextColor.redMultiplier - currentColor.redMultiplier;
                _durationColor.greenMultiplier = nextColor.greenMultiplier - currentColor.greenMultiplier;
                _durationColor.blueMultiplier = nextColor.blueMultiplier - currentColor.blueMultiplier;
                _durationColor.alphaOffset = nextColor.alphaOffset - currentColor.alphaOffset;
                _durationColor.redOffset = nextColor.redOffset - currentColor.redOffset;
                _durationColor.greenOffset = nextColor.greenOffset - currentColor.greenOffset;
                _durationColor.blueOffset = nextColor.blueOffset - currentColor.blueOffset;

                if (
                    _durationColor.alphaMultiplier != 0.f ||
                    _durationColor.redMultiplier != 0.f ||
                    _durationColor.greenMultiplier != 0.f ||
                    _durationColor.blueMultiplier != 0.f ||
                    _durationColor.alphaOffset != 0.f ||
                    _durationColor.redOffset != 0.f ||
                    _durationColor.greenOffset != 0.f ||
                    _durationColor.blueOffset != 0.f
                    )
                {
                    _tweenColor = TweenType::Always;
                }
            }
        }

        if (_tweenColor == TweenType::None &&
            (
                currentColor.alphaMultiplier - _slotColor->alphaMultiplier != 0.f ||
                currentColor.redMultiplier - _slotColor->redMultiplier != 0.f ||
                currentColor.greenMultiplier - _slotColor->greenMultiplier != 0.f ||
                currentColor.blueMultiplier - _slotColor->blueMultiplier != 0.f ||
                currentColor.alphaOffset - _slotColor->alphaOffset != 0.f ||
                currentColor.redOffset - _slotColor->redOffset != 0.f ||
                currentColor.greenOffset - _slotColor->greenOffset != 0.f ||
                currentColor.blueOffset - _slotColor->blueOffset != 0.f
                )
            )
        {
            _tweenColor = TweenType::Once;

            _durationColor.alphaMultiplier = 0.f;
            _durationColor.redMultiplier = 0.f;
            _durationColor.greenMultiplier = 0.f;
            _durationColor.blueMultiplier = 0.f;
            _durationColor.alphaOffset = 0.f;
            _durationColor.redOffset = 0.f;
            _durationColor.greenOffset = 0.f;
            _durationColor.blueOffset = 0.f;
        }
    }
    else
    {
        this->_tweenEasing = TweenFrameData::NO_TWEEN;
        this->_curve = nullptr;
        _tweenColor = TweenType::None;
    }
}

void SlotTimelineState::_onUpdateFrame(bool isUpdate)
{
    TweenTimelineState::_onUpdateFrame(isUpdate);

    const auto& currentFrame = *reinterpret_cast<SlotFrameData*>(this->_currentFrame);

    if (_tweenColor != TweenType::None && this->_animationState->_fadeProgress >= 1.f)
    {
        if (_tweenColor == TweenType::Once)
        {
            _tweenColor = TweenType::None;
        }

        const auto& currentColor = *currentFrame.color;
        _slotColor->alphaMultiplier = currentColor.alphaMultiplier + _durationColor.alphaMultiplier * this->_tweenProgress;
        _slotColor->redMultiplier = currentColor.redMultiplier + _durationColor.redMultiplier * this->_tweenProgress;
        _slotColor->greenMultiplier = currentColor.greenMultiplier + _durationColor.greenMultiplier * this->_tweenProgress;
        _slotColor->blueMultiplier = currentColor.blueMultiplier + _durationColor.blueMultiplier * this->_tweenProgress;
        _slotColor->alphaOffset = currentColor.alphaOffset + _durationColor.alphaOffset * this->_tweenProgress;
        _slotColor->redOffset = currentColor.redOffset + _durationColor.redOffset * this->_tweenProgress;
        _slotColor->greenOffset = currentColor.greenOffset + _durationColor.greenOffset * this->_tweenProgress;
        _slotColor->blueOffset = currentColor.blueOffset + _durationColor.blueOffset * this->_tweenProgress;

        slot->_colorDirty = true;
    }
}

void SlotTimelineState::fadeOut()
{
    _tweenColor = TweenType::None;
}

void SlotTimelineState::update(int time)
{
    TweenTimelineState::update(time);

    if (_tweenColor != TweenType::None)
    {
        const auto weight = this->_animationState->_weightResult;
        if (weight > 0.f)
        {
            const auto fadeProgress = this->_animationState->_fadeProgress;
            if (fadeProgress < 1.f)
            {
                const auto& currentColor = *static_cast<SlotFrameData*>(this->_currentFrame)->color;
                _slotColor->alphaMultiplier += (currentColor.alphaMultiplier + _durationColor.alphaMultiplier * this->_tweenProgress - _slotColor->alphaMultiplier) * fadeProgress;
                _slotColor->redMultiplier += (currentColor.redMultiplier + _durationColor.redMultiplier * this->_tweenProgress - _slotColor->redMultiplier) * fadeProgress;
                _slotColor->greenMultiplier += (currentColor.greenMultiplier + _durationColor.greenMultiplier * this->_tweenProgress - _slotColor->greenMultiplier) * fadeProgress;
                _slotColor->blueMultiplier += (currentColor.blueMultiplier + _durationColor.blueMultiplier * this->_tweenProgress - _slotColor->blueMultiplier) * fadeProgress;
                _slotColor->alphaOffset += (currentColor.alphaOffset + _durationColor.alphaOffset * this->_tweenProgress - _slotColor->alphaOffset) * fadeProgress;
                _slotColor->redOffset += (currentColor.redOffset + _durationColor.redOffset * this->_tweenProgress - _slotColor->redOffset) * fadeProgress;
                _slotColor->greenOffset += (currentColor.greenOffset + _durationColor.greenOffset * this->_tweenProgress - _slotColor->greenOffset) * fadeProgress;
                _slotColor->blueOffset += (currentColor.blueOffset + _durationColor.blueOffset * this->_tweenProgress - _slotColor->blueOffset) * fadeProgress;
                
                slot->_colorDirty = true;
            }
        }
    }
}

FFDTimelineState::FFDTimelineState() :
    _durationFFDFrame(nullptr)
{
    _onClear();
}
FFDTimelineState::~FFDTimelineState() 
{
    _onClear();
}

void FFDTimelineState::_onClear()
{
    TweenTimelineState::_onClear();

    slot = nullptr;

    _tweenFFD = TweenType::None;
    _ffdVertices = nullptr;

    if (_durationFFDFrame)
    {
        _durationFFDFrame->returnToPool();
        _durationFFDFrame = nullptr;
    }
}

void FFDTimelineState::_onFadeIn()
{
    _ffdVertices = &slot->_ffdVertices;

    _durationFFDFrame = BaseObject::borrowObject<ExtensionFrameData>();
    if (_durationFFDFrame->tweens.size() != _ffdVertices->size())
    {
        _durationFFDFrame->tweens.resize(_ffdVertices->size(), 0.f);
    }
}

void FFDTimelineState::_onArriveAtFrame(bool isUpdate)
{
    TweenTimelineState::_onArriveAtFrame(isUpdate);

    if (this->_animationState->_isDisabled(*slot))
    {
        this->_tweenEasing = TweenFrameData::NO_TWEEN;
        this->_curve = nullptr;
        _tweenFFD = TweenType::None;
        return;
    }

    const auto& currentFrame = *reinterpret_cast<ExtensionFrameData*>(this->_currentFrame);

    _tweenFFD = TweenType::None;

    if (this->_tweenEasing != TweenFrameData::NO_TWEEN || this->_curve)
    {
        _tweenFFD = this->_updateExtensionKeyFrame(currentFrame, *reinterpret_cast<ExtensionFrameData*>(currentFrame.next), *_durationFFDFrame);
    }

    if (_tweenFFD == TweenType::None)
    {
        for (std::size_t i = 0, l = currentFrame.tweens.size(); i < l; ++i)
        {
            if ((*_ffdVertices)[i] != currentFrame.tweens[i])
            {
                _tweenFFD = TweenType::Once;
                break;
            }
        }
    }
}

void FFDTimelineState::_onUpdateFrame(bool isUpdate)
{
    TweenTimelineState::_onUpdateFrame(isUpdate);

    if (_tweenFFD != TweenType::None && this->_animationState->_fadeProgress >= 1.f)
    {
        if (_tweenFFD == TweenType::Once)
        {
            _tweenFFD = TweenType::None;
        }

        const auto& currentFFD = static_cast<ExtensionFrameData*>(this->_currentFrame)->tweens;
        for (std::size_t i = 0, l = currentFFD.size(); i < l; ++i)
        {
            (*_ffdVertices)[i] = currentFFD[i] + _durationFFDFrame->tweens[i] * this->_tweenProgress;
        }

        slot->_ffdDirty = true;
    }
}

void FFDTimelineState::fadeOut()
{
    _tweenFFD = TweenType::None;
}

void FFDTimelineState::update(int time)
{
    TweenTimelineState::update(time);

    if (_tweenFFD != TweenType::None)
    {
        const auto weight = this->_animationState->_weightResult;
        if (weight > 0.f)
        {
            const auto fadeProgress = this->_animationState->_fadeProgress;
            if (fadeProgress < 1.f)
            {
                const auto& currentFFD = reinterpret_cast<ExtensionFrameData*>(this->_currentFrame)->tweens;
                for (std::size_t i = 0, l = currentFFD.size(); i < l; ++i)
                {
                    (*_ffdVertices)[i] += (currentFFD[i] + _durationFFDFrame->tweens[i] * this->_tweenProgress - _ffdVertices->at(i)) * fadeProgress;
                }

                slot->_ffdDirty = true;
            }
        }
    }
}

NAMESPACE_DRAGONBONES_END