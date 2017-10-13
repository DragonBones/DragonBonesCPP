#ifndef DRAGONBONES_IARMATURE_PROXY_H
#define DRAGONBONES_IARMATURE_PROXY_H

#include "../core/DragonBones.h"
#include "../events/IEventDispatcher.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* @language zh_CN
* 骨架代理接口。
* @version DragonBones 5.0
*/
class IArmatureProxy : public IEventDispatcher
{
    ABSTRACT_CLASS(IArmatureProxy);

public:
    /**
    * @private
    */
    virtual void dbInit(Armature* armature) = 0;
    /**
    * @private
    */
    virtual void dbClear() = 0;
    /**
    * @private
    */
    virtual void dbUpdate() = 0;
    /**
     * @language zh_CN
     * 释放代理和骨架。 (骨架会回收到对象池)
     * @version DragonBones 4.5
     */
    virtual void dispose(bool disposeProxy) = 0;
    /**
     * @language zh_CN
     * 获取骨架。
     * @see dragonBones.Armature
     * @version DragonBones 4.5
     */
    virtual Armature* getArmature() const = 0;
    /**
     * @language zh_CN
     * 获取动画控制器。
     * @see dragonBones.Animation
     * @version DragonBones 4.5
     */
    virtual Animation* getAnimation() const = 0;
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_IARMATURE_PROXY_H
