#ifndef __OBJECTS_ANIMATION_DATA_H__
#define __OBJECTS_ANIMATION_DATA_H__

#include "../DragonBones.h"
#include "TransformTimeline.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class AnimationData : public Timeline
{
public:
    bool autoTween;
    int frameRate;
    int playTimes;
    float fadeTime;
    // use frame tweenEase, NaN
    // overwrite frame tweenEase, [-1, 0):ease in, 0:line easing, (0, 1]:ease out, (1, 2]:ease in out
    float tweenEasing;
    
    std::string name;
    std::vector<TransformTimeline*> timelineList;
    std::vector<std::string> hideTimelineList;
    
public:
    AnimationData()
    {
        autoTween = false;
        frameRate = 30;
        playTimes = 1;
        fadeTime = 0.f;
        tweenEasing = USE_FRAME_TWEEN_EASING;
    }
    AnimationData(const AnimationData &copyData)
    {
        operator=(copyData);
    }
    AnimationData& operator=(const AnimationData &copyData)
    {
        Timeline::operator=(copyData);
        _dispose();
        autoTween = copyData.autoTween;
        frameRate = copyData.frameRate;
        playTimes = copyData.playTimes;
        fadeTime = copyData.fadeTime;
        tweenEasing = copyData.tweenEasing;
        name = copyData.name;
        timelineList.reserve(copyData.timelineList.size());
        
        for (size_t i = 0, l = timelineList.size(); i < l; ++i)
        {
            timelineList.push_back(new TransformTimeline());
            *(timelineList[i]) = *(copyData.timelineList[i]);
        }
        
        // copy
        hideTimelineList = copyData.hideTimelineList;
        return *this;
    }
    virtual ~AnimationData()
    {
        dispose();
    }
    virtual void dispose()
    {
        Timeline::dispose();
        _dispose();
    }
    
    TransformTimeline *getTimeline(const std::string &timelineName) const
    {
        for (size_t i = 0, l = timelineList.size(); i < l; ++i)
        {
            if (timelineList[i]->name == timelineName)
            {
                return timelineList[i];
            }
        }
        
        return nullptr;
    }
    
private:
    void _dispose()
    {
        for (size_t i = 0, l = timelineList.size(); i < l; ++i)
        {
            timelineList[i]->dispose();
            delete timelineList[i];
        }
        
        timelineList.clear();
        hideTimelineList.clear();
    }
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __OBJECTS_ANIMATION_DATA_H__
