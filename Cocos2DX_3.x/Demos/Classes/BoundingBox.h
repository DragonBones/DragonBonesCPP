#include "BaseDemo.h"
#include "DragHelper.h"

class BoundingBox : BaseDemo
{
public:
    CREATE_FUNC(BoundingBox);

    static cocos2d::Scene* createScene()
    {
        auto scene = cocos2d::Scene::create();
        auto layer = BoundingBox::create();

        scene->addChild(layer);
        return scene;
    }

private:
    dragonBones::Point _helpPointA;
    dragonBones::Point _helpPointB;
    dragonBones::Point _helpPointC;
    dragonBones::CCArmatureDisplay* _armatureDisplay;
    dragonBones::CCArmatureDisplay* _boundingBoxTester;
    dragonBones::CCArmatureDisplay* _targetA;
    dragonBones::CCArmatureDisplay* _targetB;
    dragonBones::Bone* _line;
    dragonBones::Bone* _pointA;
    dragonBones::Bone* _pointB;


protected:
    virtual void _onStart()
    {
        const auto factory = dragonBones::CCFactory::getFactory();
        factory->loadDragonBonesData("mecha_2903/mecha_2903_ske.json");
        factory->loadTextureAtlasData("mecha_2903/mecha_2903_tex.json");
        factory->loadDragonBonesData("bounding_box_tester/bounding_box_tester_ske.json");
        factory->loadTextureAtlasData("bounding_box_tester/bounding_box_tester_tex.json");
        //
        _armatureDisplay = factory->buildArmatureDisplay("mecha_2903d");
        _boundingBoxTester = factory->buildArmatureDisplay("tester");
        _targetA = (dragonBones::CCArmatureDisplay*)_boundingBoxTester->getArmature()->getSlot("target_a")->getDisplay();
        _targetB = (dragonBones::CCArmatureDisplay*)_boundingBoxTester->getArmature()->getSlot("target_b")->getDisplay();
        _line = _boundingBoxTester->getArmature()->getBone("line");
        _pointA = _boundingBoxTester->getArmature()->getBone("point_a");
        _pointB = _boundingBoxTester->getArmature()->getBone("point_b");
        //
        _armatureDisplay->debugDraw = true;
        _armatureDisplay->setPosition(0.0f, -100.0f);
        _boundingBoxTester->setPosition(0.0f, -200.0f);
        _targetA->getArmature()->inheritAnimation = false;
        _targetB->getArmature()->inheritAnimation = false;
        _line->offsetMode = dragonBones::OffsetMode::Override;
        _pointA->offsetMode = dragonBones::OffsetMode::Override;
        _pointB->offsetMode = dragonBones::OffsetMode::Override;
        _armatureDisplay->getAnimation()->play("walk");
        _boundingBoxTester->getAnimation()->play("0");
        //
        addChild(_armatureDisplay);
        addChild(_boundingBoxTester);
        cocos2d::Director::getInstance()->getScheduler()->schedule(
            schedule_selector(BoundingBox::_enterFrameHandler),
            this, 0.0f, false
        );
        //
        _targetA->getEventDispatcher()->setEnabled(true);
        _targetB->getEventDispatcher()->setEnabled(true);
        DragHelper::getInstance()->enableDrag(_targetA);
        DragHelper::getInstance()->enableDrag(_targetB);
        //
        createText("Touch to drag bounding box tester.");
    }

private:
    void _enterFrameHandler(float passedTime)
    {
        const auto& boundingBoxTesterTLTG = _boundingBoxTester->getNodeToWorldTransform();
        const auto& boundingBoxTesterTGTL = _boundingBoxTester->getWorldToNodeTransform();
        const auto& armatureDisplayTLTG = _armatureDisplay->getNodeToWorldTransform();
        const auto& armatureDisplayTGTL = _armatureDisplay->getWorldToNodeTransform();

        auto helpPointA = _targetA->getPosition3D();
        auto helpPointB = _targetB->getPosition3D();
        boundingBoxTesterTLTG.transformPoint(&helpPointA);
        boundingBoxTesterTLTG.transformPoint(&helpPointB);
        armatureDisplayTGTL.transformPoint(&helpPointA);
        armatureDisplayTGTL.transformPoint(&helpPointB);

        const auto containsSlotA = _armatureDisplay->getArmature()->containsPoint(helpPointA.x, helpPointA.y);
        const auto containsSlotB = _armatureDisplay->getArmature()->containsPoint(helpPointB.x, helpPointB.y);
        const auto intersectsSlots = _armatureDisplay->getArmature()->intersectsSegment(helpPointA.x, helpPointA.y, helpPointB.x, helpPointB.y, &_helpPointA, &_helpPointB, &_helpPointC);
        helpPointA.set(_helpPointA.x, _helpPointA.y, 0.0f);
        helpPointB.set(_helpPointB.x, _helpPointB.y, 0.0f);

        {
            const auto& animationName = containsSlotA ? "1" : "0";
            if (_targetA->getAnimation()->getLastAnimationName() != animationName) 
            {
                _targetA->getAnimation()->fadeIn(animationName, 0.2f)->resetToPose = false;
            }
        }

        {
            const auto& animationName = containsSlotB ? "1" : "0";
            if (_targetB->getAnimation()->getLastAnimationName() != animationName)
            {
                _targetB->getAnimation()->fadeIn(animationName, 0.2f)->resetToPose = false;
            }
        }

        {
            const auto targetA = _targetA->getArmature()->getParent()->getParent();
            const auto targetB = _targetB->getArmature()->getParent()->getParent();
            const auto dX = targetB->global.x - targetA->global.x;
            const auto dY = targetB->global.y - targetA->global.y;
            _line->offset.x = targetA->global.x;
            _line->offset.y = targetA->global.y;
            _line->offset.scaleX = sqrt(dX * dX + dY * dY) / 100.0f;
            _line->offset.rotation = atan2(dY, dX);
            _line->invalidUpdate();

            const auto& animationName = intersectsSlots ? "1" : "0";
            if (_boundingBoxTester->getAnimation()->getLastAnimationName() != animationName)
            {
                _boundingBoxTester->getAnimation()->fadeIn(animationName, 0.2f)->resetToPose = false;
            }

            if (intersectsSlots) 
            {
                armatureDisplayTLTG.transformPoint(&helpPointA);
                armatureDisplayTLTG.transformPoint(&helpPointB);
                boundingBoxTesterTGTL.transformPoint(&helpPointA);
                boundingBoxTesterTGTL.transformPoint(&helpPointB);

                _pointA->setVisible(true);
                _pointB->setVisible(true);
                _pointA->offset.x = helpPointA.x;
                _pointA->offset.y = helpPointA.y;
                _pointB->offset.x = helpPointB.x;
                _pointB->offset.y = helpPointB.y;
                _pointA->offset.rotation = _helpPointC.x;
                _pointB->offset.rotation = _helpPointC.y;
                _pointA->invalidUpdate();
                _pointB->invalidUpdate();
            }
            else 
            {
                _pointA->setVisible(false);
                _pointB->setVisible(false);
            }
        }
    }
};