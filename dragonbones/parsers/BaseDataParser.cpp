#include "BaseDataParser.h"

#include <stdio.h>
#include <cstring>

NAME_SPACE_DRAGON_BONES_BEGIN
TextData::AlignHType BaseDataParser::getAlignHType(const char *alignHString)
{
    if (alignHString)
    {
        if (std::strcmp(alignHString, "left") == 0)
        {
            return TextData::AlignHType::LEFT;
        }
        else if (std::strcmp(alignHString, "center") == 0)
        {
            return TextData::AlignHType::CENTER;
        }
        else if (std::strcmp(alignHString, "right") == 0)
        {
            return TextData::AlignHType::RIGHT;
        }
        else if (std::strcmp(alignHString, "justify") == 0)
        {
            return TextData::AlignHType::JUSTIFY;
        }
    }

    return TextData::AlignHType::LEFT;
}

TextData::AlignVType BaseDataParser::getAlignVType(const char *alignVString)
{
    if (alignVString)
    {
        if (std::strcmp(alignVString, "top") == 0)
        {
            return TextData::AlignVType::TOP;
        }
        else if (std::strcmp(alignVString, "center") == 0)
        {
            return TextData::AlignVType::CENTER;
        }
        else if (std::strcmp(alignVString, "right") == 0)
        {
            return TextData::AlignVType::BOTTOM;
        }
    }

    return TextData::AlignVType::TOP;
}

TextData::TextType BaseDataParser::getTextType(const char *textTypeString)
{
    if (textTypeString)
    {
        if (std::strcmp(textTypeString, "static") == 0)
        {
            return TextData::TextType::STATIC;
        }
        else if (std::strcmp(textTypeString, "dynamic") == 0)
        {
            return TextData::TextType::DYNAMIC;
        }
        else if (std::strcmp(textTypeString, "input") == 0)
        {
            return TextData::TextType::INPUT;
        }
    }

    return TextData::TextType::STATIC;
}

TextData::LineType BaseDataParser::getLineType(const char *lineTypeString)
{
    if (lineTypeString)
    {
        if (std::strcmp(lineTypeString, "single line") == 0)
        {
            return TextData::LineType::SINGLE_LINE;
        }
        else if (std::strcmp(lineTypeString, "multiline") == 0)
        {
            return TextData::LineType::MULTILINE;
        }
        else if (std::strcmp(lineTypeString, "multiline no wrap") == 0)
        {
            return TextData::LineType::MULTILINE_NO_WRAP;
        }
        else if (std::strcmp(lineTypeString, "password") == 0)
        {
            return TextData::LineType::PASSWORD;
        }
    }

    return TextData::LineType::SINGLE_LINE;
}

void BaseDataParser::transformArmatureData(ArmatureData *armatureData)
{
    for (size_t i = armatureData->boneDataList.size(); i--;)
    {
        BoneData *boneData = armatureData->boneDataList[i];
        
        if (boneData && !boneData->parent.empty())
        {
            const BoneData *parentBoneData = armatureData->getBoneData(boneData->parent);
            
            if (parentBoneData)
            {
                boneData->transform = boneData->global;
                boneData->transform.transformWith(parentBoneData->global);
            }
        }
    }
}

void BaseDataParser::transformArmatureDataAnimations(ArmatureData *armatureData)
{
    for (size_t i = 0, l = armatureData->animationDataList.size(); i < l; ++i)
    {
        transformAnimationData(armatureData->animationDataList[i], armatureData);
    }
}

void BaseDataParser::transformAnimationData(AnimationData *animationData, const ArmatureData *armatureData)
{
    SkinData *skinData = armatureData->getSkinData("");
    
    for (size_t i = 0, l = armatureData->boneDataList.size(); i < l; ++i)
    {
        const BoneData *boneData = armatureData->boneDataList[i];
        TransformTimeline *timeline = animationData->getTimeline(boneData->name);
        
        if (!timeline)
        {
            continue;
        }
        
        SlotData *slotData = nullptr;
        
        if (skinData)
        {
            for (size_t i = 0, l = skinData->slotDataList.size(); i < l; ++i)
            {
                slotData = skinData->slotDataList[i];
                
                if (slotData->parent == boneData->name)
                {
                    break;
                }
            }
        }
        
        Transform *originTransform = nullptr;
        Point *originPivot = nullptr;
        TransformFrame *prevFrame = nullptr;
        
        for (size_t i = 0, l = timeline->frameList.size(); i < l; ++i)
        {
            TransformFrame *frame = static_cast<TransformFrame*>(timeline->frameList[i]);
            setFrameTransform(animationData, armatureData, boneData, frame);
            frame->transform.x -= boneData->transform.x;
            frame->transform.y -= boneData->transform.y;
            frame->transform.skewX -= boneData->transform.skewX;
            frame->transform.skewY -= boneData->transform.skewY;
            frame->transform.scaleX -= boneData->transform.scaleX;
            frame->transform.scaleY -= boneData->transform.scaleY;
            
            if (!timeline->transformed && slotData)
            {
                frame->zOrder -= slotData->zOrder;
            }
            
            if (!originTransform)
            {
                // copy
                timeline->originTransform = frame->transform;
                originTransform = &timeline->originTransform;
                originTransform->skewX = formatRadian(originTransform->skewX);
                originTransform->skewY = formatRadian(originTransform->skewY);
                // copy
                timeline->originPivot = frame->pivot;
                originPivot = &timeline->originPivot;
            }
            
            frame->transform.x -= originTransform->x;
            frame->transform.y -= originTransform->y;
            frame->transform.skewX = formatRadian(frame->transform.skewX - originTransform->skewX);
            frame->transform.skewY = formatRadian(frame->transform.skewY - originTransform->skewY);
            frame->transform.scaleX -= originTransform->scaleX;
            frame->transform.scaleY -= originTransform->scaleY;
            
            if (!timeline->transformed)
            {
                frame->pivot.x -= originPivot->x;
                frame->pivot.y -= originPivot->y;
            }
            
            if (prevFrame)
            {
                const float dLX = frame->transform.skewX - prevFrame->transform.skewX;
                
                if (prevFrame->tweenRotate)
                {
                    if (prevFrame->tweenRotate > 0)
                    {
                        if (dLX < 0)
                        {
                            frame->transform.skewX += PI * 2;
                            frame->transform.skewY += PI * 2;
                        }
                        
                        if (prevFrame->tweenRotate > 1)
                        {
                            frame->transform.skewX += PI * 2 * (prevFrame->tweenRotate - 1);
                            frame->transform.skewY += PI * 2 * (prevFrame->tweenRotate - 1);
                        }
                    }
                    else
                    {
                        if (dLX > 0)
                        {
                            frame->transform.skewX -= PI * 2;
                            frame->transform.skewY -= PI * 2;
                        }
                        
                        if (prevFrame->tweenRotate < 1)
                        {
                            frame->transform.skewX += PI * 2 * (prevFrame->tweenRotate + 1);
                            frame->transform.skewY += PI * 2 * (prevFrame->tweenRotate + 1);
                        }
                    }
                }
                else
                {
                    frame->transform.skewX = prevFrame->transform.skewX + formatRadian(frame->transform.skewX - prevFrame->transform.skewX);
                    frame->transform.skewY = prevFrame->transform.skewY + formatRadian(frame->transform.skewY - prevFrame->transform.skewY);
                }
            }
            
            prevFrame = frame;
        }
        
        timeline->transformed = true;
    }
}

void BaseDataParser::addHideTimeline(AnimationData *animationData, const ArmatureData *armatureData)
{
    for (size_t i = 0, l = armatureData->boneDataList.size(); i < l; ++i)
    {
        const BoneData *boneData = armatureData->boneDataList[i];
        
        if (!animationData->getTimeline(boneData->name))
        {
            auto iterator = std::find(animationData->hideTimelineList.cbegin(), animationData->hideTimelineList.cend(), boneData->name);
            
            if (iterator != animationData->hideTimelineList.cend())
            {
                continue;
            }
            
            animationData->hideTimelineList.push_back(boneData->name);
        }
    }
}

void BaseDataParser::setFrameTransform(AnimationData *animationData, const ArmatureData *armatureData, const BoneData *boneData, TransformFrame *frame)
{
    frame->transform = frame->global;
    BoneData *parentData = armatureData->getBoneData(boneData->parent);
    
    if (parentData)
    {
        TransformTimeline *parentTimeline = animationData->getTimeline(parentData->name);
        
        if (parentTimeline)
        {
            std::vector<TransformTimeline*> parentTimelineList;
            std::vector<BoneData*> parentDataList;
            
            while (parentTimeline)
            {
                parentTimelineList.push_back(parentTimeline);
                parentDataList.push_back(parentData);
                parentData = armatureData->getBoneData(parentData->parent);
                
                if (parentData)
                {
                    parentTimeline = animationData->getTimeline(parentData->name);
                }
                else
                {
                    parentTimeline = nullptr;
                }
            }
            
            Matrix helpMatrix;
            Transform currentTransform;
            Transform *globalTransform = nullptr;
            
            for (size_t i = parentTimelineList.size(); i--;)
            {
                parentTimeline = parentTimelineList[i];
                parentData = parentDataList[i];
                getTimelineTransform(parentTimeline, frame->position, &currentTransform, !globalTransform);
                
                if (globalTransform)
                {
                    //if(inheritRotation)
                    //{
                    globalTransform->skewX += currentTransform.skewX + parentTimeline->originTransform.skewX + parentData->transform.skewX;
                    globalTransform->skewY += currentTransform.skewY + parentTimeline->originTransform.skewY + parentData->transform.skewY;
                    //}
                    //if(inheritScale)
                    //{
                    //  globalTransform.scaleX *= currentTransform.scaleX + parentTimeline.originTransform.scaleX;
                    //  globalTransform.scaleY *= currentTransform.scaleY + parentTimeline.originTransform.scaleY;
                    //}
                    //else
                    //{
                    globalTransform->scaleX = currentTransform.scaleX + parentTimeline->originTransform.scaleX + parentData->transform.scaleX;
                    globalTransform->scaleY = currentTransform.scaleY + parentTimeline->originTransform.scaleY + parentData->transform.scaleY;
                    //}
                    const float x = currentTransform.x + parentTimeline->originTransform.x + parentData->transform.x;
                    const float y = currentTransform.y + parentTimeline->originTransform.y + parentData->transform.y;
                    globalTransform->x = helpMatrix.a * x + helpMatrix.c * y + helpMatrix.tx;
                    globalTransform->y = helpMatrix.d * y + helpMatrix.b * x + helpMatrix.ty;
                }
                else
                {
                    globalTransform = new Transform();
                    *globalTransform = currentTransform;
                }
                
                globalTransform->toMatrix(helpMatrix, true);
            }
            
            frame->transform.transformWith(*globalTransform);
            
            if (globalTransform)
            {
                delete globalTransform;
                globalTransform = nullptr;
            }
        }
    }
}

void BaseDataParser::getTimelineTransform(const TransformTimeline *timeline, int position, Transform *retult, bool isGlobal)
{
    for (size_t i = 0, l = timeline->frameList.size(); i < l; ++i)
    {
        const TransformFrame *currentFrame = static_cast<const TransformFrame*>(timeline->frameList[i]);
        
        if (currentFrame->position <= position && currentFrame->position + currentFrame->duration > position)
        {
            if (i == timeline->frameList.size() - 1 || position == currentFrame->position)
            {
                //copy
                *retult = isGlobal ? currentFrame->global : currentFrame->transform;
            }
            else
            {
                float progress = (position - currentFrame->position) / (float)(currentFrame->duration);
                const float tweenEasing = currentFrame->tweenEasing;
                
                if (tweenEasing && tweenEasing != NO_TWEEN_EASING && tweenEasing != AUTO_TWEEN_EASING)
                {
                    progress = getEaseValue(progress, tweenEasing);
                }
                
                const TransformFrame *nextFrame = static_cast<const TransformFrame*>(timeline->frameList[i + 1]);
                const Transform *currentTransform = isGlobal ? &currentFrame->global : &currentFrame->transform;
                const Transform *nextTransform = isGlobal ? &nextFrame->global : &nextFrame->transform;
                retult->x = currentTransform->x + (nextTransform->x - currentTransform->x) * progress;
                retult->y = currentTransform->y + (nextTransform->y - currentTransform->y) * progress;
                retult->skewX = formatRadian(currentTransform->skewX + (nextTransform->skewX - currentTransform->skewX) * progress);
                retult->skewY = formatRadian(currentTransform->skewY + (nextTransform->skewY - currentTransform->skewY) * progress);
                retult->scaleX = currentTransform->scaleX + (nextTransform->scaleX - currentTransform->scaleX) * progress;
                retult->scaleY = currentTransform->scaleY + (nextTransform->scaleY - currentTransform->scaleY) * progress;
            }
            
            break;
        }
    }
}

BaseDataParser::BaseDataParser() {}
BaseDataParser::~BaseDataParser() {}
NAME_SPACE_DRAGON_BONES_END