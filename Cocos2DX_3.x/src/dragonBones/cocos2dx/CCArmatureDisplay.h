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
     * @internal
     * @private
     */
    static CCArmatureDisplay* create();

protected:
    bool _debugDraw;
    Armature* _armature;
    cocos2d::EventDispatcher* _dispatcher;

public:
    CCArmatureDisplay() :
        _debugDraw(false),
        _armature(nullptr),
        _dispatcher(nullptr)
    {
        _dispatcher = new cocos2d::EventDispatcher();
        setEventDispatcher(_dispatcher);
        // _dispatcher->setEnabled(true);
    }
    virtual ~CCArmatureDisplay() {}

public:
    /**
     * @inheritDoc
     */
    virtual void dbInit(Armature* armature) override;
    /**
     * @inheritDoc
     */
    virtual void dbClear() override;
    /**
     * @inheritDoc
     */
    virtual void dbUpdate() override;
    /**
     * @inheritDoc
     */
    virtual void dispose(bool disposeProxy = true) override;
    /**
     * @inheritDoc
     */
    inline virtual bool hasDBEventListener(const std::string& type) const override
    {
        return _dispatcher->isEnabled();
    }
    /**
     * @inheritDoc
     */
    virtual void dispatchDBEvent(const std::string& type, EventObject* value) override;
    /**
     * @inheritDoc
     */
    virtual void addDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener) override;
    /**
     * @inheritDoc
     */
    virtual void removeDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener) override;
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
 * @internal
 * @private
 */
class DBCCSprite : public cocos2d::Sprite
{
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(DBCCSprite)

public:
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
