#ifndef DRAGONBONES_BASE_OBJECT_H
#define DRAGONBONES_BASE_OBJECT_H

#include "DragonBones.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
 * 基础对象。
 * @version DragonBones 4.5
 * @language zh_CN
 */
class BaseObject
{
private:
    static unsigned _hashCode;
    static unsigned _defaultMaxCount;
    static std::map<std::size_t, unsigned> _maxCountMap;
    static std::map<std::size_t, std::vector<BaseObject*>> _poolsMap;
    static void _returnObject(BaseObject *object);

public:
    /**
     * 设置每种对象池的最大缓存数量。
     * @param objectConstructor 对象类。
     * @param maxCount 最大缓存数量。 (设置为 0 则不缓存)
     * @version DragonBones 4.5
     * @language zh_CN
     */
    static void setMaxCount(std::size_t classTypeIndex, unsigned maxCount);
    /**
     * 清除对象池缓存的对象。
     * @param objectConstructor 对象类。 (不设置则清除所有缓存)
     * @version DragonBones 4.5
     * @language zh_CN
     */
    static void clearPool(std::size_t classTypeIndex = 0);
    /**
     * 从对象池中创建指定对象。
     * @version DragonBones 4.5
     * @language zh_CN
     */
    template<typename T>
    static T* borrowObject() 
    {
        const auto classTypeIndex = T::getTypeIndex();
        const auto iterator = _poolsMap.find(classTypeIndex);
        if (iterator != _poolsMap.end())
        {
            auto& pool = iterator->second;
            if (!pool.empty())
            {
                const auto object = static_cast<T*>(pool.back());
                pool.pop_back();
                object->_isInPool = false;
                return object;
            }
        }

        const auto object = new (std::nothrow) T();

        return object;
    }

public:
    /**
     * 对象的唯一标识。
     * @version DragonBones 4.5
     * @language zh_CN
     */
    const unsigned hashCode;

private:
    bool _isInPool;

public:
    virtual ~BaseObject() {}

protected:
    BaseObject() :
        hashCode(BaseObject::_hashCode++),
        _isInPool(false)
    {}

    virtual void _onClear() = 0;

public:
    /**
     * @private
     */
    virtual std::size_t getClassTypeIndex() const = 0;
    /**
     * 清除数据并返还对象池。
     * @version DragonBones 4.5
     * @language zh_CN
     */
    void returnToPool();
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_BASE_OBJECT_H
