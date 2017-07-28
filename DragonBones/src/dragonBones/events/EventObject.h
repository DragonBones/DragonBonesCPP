#ifndef DRAGONBONES_EVENT_OBJECT_H
#define DRAGONBONES_EVENT_OBJECT_H

#include "../core/BaseObject.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* 事件数据。
* @version DragonBones 4.5
* @language zh_CN
*/
class EventObject : public BaseObject
{
    BIND_CLASS_TYPE_A(EventObject);

public:
    /**
    * 动画开始。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    static const char* START;
    /**
    * 动画循环播放一次完成。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    static const char* LOOP_COMPLETE;
    /**
    * 动画播放完成。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    static const char* COMPLETE;
    /**
    * 动画淡入开始。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    static const char* FADE_IN;
    /**
    * 动画淡入完成。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    static const char* FADE_IN_COMPLETE;
    /**
    * 动画淡出开始。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    static const char* FADE_OUT;
    /**
    * 动画淡出完成。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    static const char* FADE_OUT_COMPLETE;
    /**
    * 动画帧事件。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    static const char* FRAME_EVENT;
    /**
    * 动画声音事件。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    static const char* SOUND_EVENT;

public:
    /**
    * @private
    */
    float time;
    /**
    * 事件类型。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    std::string type;
    /**
    * 事件名称。 (帧标签的名称或声音的名称)
    * @version DragonBones 4.5
    * @language zh_CN
    */
    std::string name;
    /**
    * 发出事件的骨架。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    Armature* armature;
    /**
    * 发出事件的骨骼。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    Bone* bone;
    /**
    * 发出事件的插槽。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    Slot* slot;
    /**
    * 发出事件的动画状态。
    * @version DragonBones 4.5
    * @language zh_CN
    */
    AnimationState* animationState;
    /**
    * 自定义数据
    * @see dragonBones.CustomData
    * @version DragonBones 5.0
    * @language zh_CN
    */
    UserData* data;

    void copyFrom(const EventObject& value);

protected:
    virtual void _onClear() override;

public: // For WebAssembly.
    Armature* getArmature() const { return armature; }
    Bone* getBone() const { return bone; }
    Slot* getSlot() const { return slot; }
    AnimationState* getAnimationState() const { return animationState; }
    UserData* getUserData() const { return data; }
};
DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_EVENT_OBJECT_H
