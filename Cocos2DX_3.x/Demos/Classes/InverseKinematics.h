#include "BaseDemo.h"
#include "DragHelper.h"

class InverseKinematics : BaseDemo
{
public:
    CREATE_FUNC(InverseKinematics);

    static cocos2d::Scene* createScene()
    {
        auto scene = cocos2d::Scene::create();
        auto layer = InverseKinematics::create();

        scene->addChild(layer);
        return scene;
    }

private:
    int _faceDir;
    float _aimRadian;
    float _offsetRotation;
    cocos2d::Point _target;
    dragonBones::CCArmatureDisplay* _armatureDisplay;
    dragonBones::CCArmatureDisplay* _floorBoard;
    dragonBones::Bone* _chestBone;
    dragonBones::Bone* _leftFootBone;
    dragonBones::Bone* _rightFootBone;
    dragonBones::Bone* _circleBone;
    dragonBones::Bone* _floorBoardBone;
    dragonBones::AnimationState* _aimState;


protected:
    virtual void _onStart()
    {
        _faceDir = 0;
        _aimRadian = 0.0f;
        _offsetRotation = 0.0f;
        //
        const auto factory = dragonBones::CCFactory::getFactory();
        factory->loadDragonBonesData("mecha_1406/mecha_1406_ske.json");
        factory->loadTextureAtlasData("mecha_1406/mecha_1406_tex.json");
        factory->loadDragonBonesData("floor_board/floor_board_ske.json");
        factory->loadTextureAtlasData("floor_board/floor_board_tex.json");
        //
        _armatureDisplay = factory->buildArmatureDisplay("mecha_1406");
        _floorBoard = factory->buildArmatureDisplay("floor_board");
        //
        _chestBone = _armatureDisplay->getArmature()->getBone("chest");
        _leftFootBone = _armatureDisplay->getArmature()->getBone("foot_l");
        _rightFootBone = _armatureDisplay->getArmature()->getBone("foot_r");
        _circleBone = _floorBoard->getArmature()->getBone("circle");
        _floorBoardBone = _floorBoard->getArmature()->getBone("floor_board");
        //
        _armatureDisplay->getAnimation()->play("idle");
        _aimState = _armatureDisplay->getAnimation()->fadeIn("aim", 0.1f, 1, 0, "aim_group");
        _aimState->resetToPose = false;
        _aimState->stop();
        //
        _floorBoard->getAnimation()->play("idle");
        _floorBoard->getArmature()->getSlot("player")->setDisplay(_armatureDisplay, dragonBones::DisplayType::Image);
        _floorBoard->setPosition(0.0f, -50.0f);
        addChild(_floorBoard);
        //
        const auto listener = cocos2d::EventListenerMouse::create();
        listener->onMouseMove = CC_CALLBACK_1(InverseKinematics::_mouseMovedHandler, this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        cocos2d::Director::getInstance()->getScheduler()->schedule(
            schedule_selector(InverseKinematics::_enterFrameHandler),
            this, 0.0f, false
        );
        //
        DragHelper::getInstance()->enableDrag((cocos2d::Node*)_floorBoard->getArmature()->getSlot("circle")->getDisplay());
        //
        createText("Touch to drag circle to modify IK bones.");
    }

private:
    void _mouseMovedHandler(cocos2d::EventMouse* event)
    {
        _target.x = event->getLocation().x - getPositionX();
        _target.y = -(event->getLocation().y - getPositionY());
    }

    void _enterFrameHandler(float passedTime)
    {
        _updateAim();
        _updateFoot();
    }

    void _updateAim()
    {
        const auto positionX = _floorBoard->getPositionX();
        const auto positionY = _floorBoard->getPositionY();
        const auto aimOffsetY = _chestBone->global.y * _floorBoard->getScaleY();

        _faceDir = _target.x > 0.0f ? 1 : -1;
        _armatureDisplay->getArmature()->setFlipX(_faceDir < 0);

        if (_faceDir > 0) {
            _aimRadian = -atan2(_target.y - positionY - aimOffsetY, _target.x - positionX);
        }
        else 
        {
            _aimRadian = dragonBones::Transform::PI + atan2(_target.y - positionY - aimOffsetY, _target.x - positionX);
            if (_aimRadian > dragonBones::Transform::PI) 
            {
                _aimRadian -= dragonBones::Transform::PI * 2.0f;
            }
        }

        // Calculate progress.
        const auto progress = abs((_aimRadian + dragonBones::Transform::PI / 2.0f) / dragonBones::Transform::PI);
        // Set currentTime.
        _aimState->setCurrentTime(progress * _aimState->getTotalTime());
    }

    void _updateFoot()
    {
        // Set floor board bone offset.
        const auto minRadian = -25.0f * dragonBones::Transform::DEG_RAD;
        const auto maxRadian = 25.0f * dragonBones::Transform::DEG_RAD;
        auto circleRadian = atan2(-_circleBone->global.y, _circleBone->global.x);

        if (_circleBone->global.x < 0.0f) 
        {
            circleRadian = dragonBones::Transform::normalizeRadian(circleRadian + dragonBones::Transform::PI);
        }

        _offsetRotation = std::min(std::max(circleRadian, minRadian), maxRadian);
        _floorBoardBone->offset.rotation = _offsetRotation;
        _floorBoardBone->invalidUpdate();
        // Set foot bone offset.
        const auto tan = std::tan(_offsetRotation);
        const auto sinR = 1.0f / sin(dragonBones::Transform::PI * 0.5f - _offsetRotation) - 1.0f;

        _leftFootBone->offset.y = tan * _leftFootBone->global.x + _leftFootBone->origin->y * sinR;
        _leftFootBone->offset.rotation = _offsetRotation * _faceDir;
        _leftFootBone->invalidUpdate();

        _rightFootBone->offset.y = tan * _rightFootBone->global.x + _rightFootBone->origin->y * sinR;
        _rightFootBone->offset.rotation = _offsetRotation * _faceDir;
        _rightFootBone->invalidUpdate();
    }
};