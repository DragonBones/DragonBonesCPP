#ifndef DRAGONBONES_CC_ARMATURE_DISPLAY_CONTAINER_H
#define DRAGONBONES_CC_ARMATURE_DISPLAY_CONTAINER_H

#include "dragonBones/DragonBonesHeaders.h"
#include "cocos2d.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
* @inheritDoc
*/
class CCArmatureDisplay : public cocos2d::Node, public virtual IArmatureProxy
{
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(CCArmatureDisplay)

public:
    /**
    * @private
    */
    static CCArmatureDisplay* create();

protected:
    Armature* _armature;
    cocos2d::EventDispatcher* _dispatcher;

protected:
    CCArmatureDisplay() :
        _armature(nullptr),
        _dispatcher(nullptr)
    {
        _dispatcher = new cocos2d::EventDispatcher();
        this->setEventDispatcher(_dispatcher);
        // _dispatcher->setEnabled(true); // 默认不开启，需要手动开启（浪费性能）
    }
    virtual ~CCArmatureDisplay() {}

public:
    /**
    * @inheritDoc
    */
    virtual void init(Armature* armature) override;
    /**
    * @inheritDoc
    */
    virtual void clear() override;
    /**
    * @inheritDoc
    */
    virtual void dispose(bool disposeProxy = true) override;
    /**
    * @inheritDoc
    */
    virtual void debugUpdate(bool isEnabled) override;
    /**
    * @inheritDoc
    */
    virtual void _dispatchEvent(const std::string& type, EventObject* value) override;
    /**
    * @inheritDoc
    */
    virtual void addEvent(const std::string& type, const std::function<void(EventObject*)>& listener) override;
    /**
    * @inheritDoc
    */
    virtual void removeEvent(const std::string& type, const std::function<void(EventObject*)>& listener) override;
    /**
    * @inheritDoc
    */
    inline virtual bool hasEvent(const std::string& type) const override
    {
        return _dispatcher->isEnabled();
    }
    /**
    * @inheritDoc
    */
    inline virtual Armature* getArmature() const override
    {
        return _armature;
    }
    /**
    * @inheritDoc
    */
    inline virtual Animation* getAnimation() const override
    {
        return _armature->getAnimation();
    }
};
/**
* @private
*/
class DBCCSprite : public cocos2d::Sprite
{
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(DBCCSprite)

public:
    /**
    * @private
    */
    static DBCCSprite* create();

protected:
    DBCCSprite() {}
    virtual ~DBCCSprite() {}
    /**
    * Modify for polyInfo rect
    */
    bool _checkVisibility(const cocos2d::Mat4& transform, const cocos2d::Size& size, const cocos2d::Rect& rect);

public:
    /**
     * Modify for polyInfo rect
     */
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
    /**
     * Modify for cocos2dx 3.7, 3.8, 3.9
     */
    cocos2d::PolygonInfo& getPolygonInfoModify();
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_CC_ARMATURE_DISPLAY_CONTAINER_H
