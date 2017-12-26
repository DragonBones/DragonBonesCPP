#include "BaseDemo.h"

class BoneOffset : BaseDemo
{
public:
    CREATE_FUNC(BoneOffset);

    static cocos2d::Scene* createScene()
    {
        auto scene = cocos2d::Scene::create();
        auto layer = BoneOffset::create();

        scene->addChild(layer);
        return scene;
    }

protected:
    virtual void _onStart()
    {
        const auto factory = dragonBones::CCFactory::getFactory();
        factory->loadDragonBonesData("bullet_01/bullet_01_ske.json");
        factory->loadTextureAtlasData("bullet_01/bullet_01_tex.json");

        for (size_t i = 0; i < 100; ++i)
        {
            const auto armatureDisplay = factory->buildArmatureDisplay("bullet_01");
            armatureDisplay->getEventDispatcher()->setEnabled(true);
            armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::COMPLETE, std::bind(&BoneOffset::_animationEventHandler, this, std::placeholders::_1));
            armatureDisplay->setPosition(0.0f, 0.0f);
            addChild(armatureDisplay);
            //
            _moveTo(armatureDisplay);
        }
    }

    void _animationEventHandler(cocos2d::EventCustom* event)
    {
        const auto eventObject = (dragonBones::EventObject*)event->getUserData();
        _moveTo((dragonBones::CCArmatureDisplay*)(eventObject->armature->getDisplay()));
    }

    void _moveTo(dragonBones::CCArmatureDisplay* armatureDisplay)
    {
        const auto fromX = armatureDisplay->getPositionX();
        const auto fromY = armatureDisplay->getPositionY();
        const auto toX = cocos2d::random(0.0f, getStageWidth()) - getStageWidth() * 0.5f;
        const auto toY = cocos2d::random(0.0f, getStageHeight()) - getStageHeight() * 0.5f;
        const auto dX = toX - fromX;
        const auto dY = toY - fromY;
        const auto rootSlot = armatureDisplay->getArmature()->getBone("root");
        const auto bulletSlot = armatureDisplay->getArmature()->getBone("bullet");
        // Modify root and bullet bone offset.
        rootSlot->offset.scaleX = sqrt(dX * dX + dY * dY) / 100.0f; // Bullet translate distance is 100 px.
        rootSlot->offset.rotation = atan2(dY, dX);
        rootSlot->offset.skew = cocos2d::random(0.0f, dragonBones::Transform::PI) - dragonBones::Transform::PI * 0.5f; // Random skew.
        bulletSlot->offset.scaleX = cocos2d::random(0.5f, 1.5f); // Random scale.
        bulletSlot->offset.scaleY = cocos2d::random(0.5f, 1.5f);
        // Update root and bullet bone.
        rootSlot->invalidUpdate();
        bulletSlot->invalidUpdate();
        //
        armatureDisplay->getAnimation()->timeScale = cocos2d::random(0.5f, 1.5f); // Random animation speed.
        armatureDisplay->getAnimation()->play("idle", 1);
    }
};