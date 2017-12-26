#ifndef DRAG_HELPER_H
#define DRAG_HELPER_H

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"

class DragHelper
{
private:
    static DragHelper* _instance;

public:
    static DragHelper* getInstance();

private:
    bool _isTouched;
    cocos2d::Vec3 _helpPoint;
    cocos2d::Vec3 _dragOffset;
    cocos2d::Node* _dragDisplayObject;
    
public:
    DragHelper():
        _isTouched(false),
        _dragDisplayObject(nullptr)
    {
    }

    void enableDrag(cocos2d::Node* displayObject)
    {
        const auto listener = cocos2d::EventListenerMouse::create();
        listener->onMouseDown = CC_CALLBACK_1(DragHelper::_mouseDownHandler, this);
        listener->onMouseUp = CC_CALLBACK_1(DragHelper::_mouseUpHandler, this);
        listener->onMouseMove = CC_CALLBACK_1(DragHelper::_mouseMovedHandler, this);
        displayObject->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, displayObject);
    }

    void disableDrag(cocos2d::Node* displayObject)
    {

    }

private:
    void _mouseDownHandler(cocos2d::EventMouse* event)
    {
        if (_dragDisplayObject) 
        {
            return;
        }

        const auto node = event->getCurrentTarget();


        _dragDisplayObject = node;

        const auto armatureDisplay = (dragonBones::CCArmatureDisplay*)(_dragDisplayObject->getParent());
        const auto bone = armatureDisplay->getArmature()->getBoneByDisplay(_dragDisplayObject);

        if (bone) 
        {
            _helpPoint.set(event->getLocation().x, event->getLocation().y, 0.0f);
            const auto& transform = armatureDisplay->getWorldToNodeTransform();
            transform.transformPoint(&_helpPoint);

            if (bone->offsetMode != dragonBones::OffsetMode::Override)
            {
                bone->offsetMode = dragonBones::OffsetMode::Override;
                bone->offset.x = bone->global.x;
                bone->offset.y = bone->global.y;
            }

            _dragOffset.x = bone->offset.x - _helpPoint.x;
            _dragOffset.y = bone->offset.y - _helpPoint.y;

            _isTouched = true;
        }
    }

    void _mouseUpHandler(cocos2d::EventMouse* event)
    {
        _dragDisplayObject = nullptr;
    }

    void _mouseMovedHandler(cocos2d::EventMouse* event)
    {
        if (!_dragDisplayObject)
        {
            return;
        }

        const auto armatureDisplay = (dragonBones::CCArmatureDisplay*)(_dragDisplayObject->getParent());
        const auto bone = armatureDisplay->getArmature()->getBoneByDisplay(_dragDisplayObject);

        if (bone) 
        {
            _helpPoint.set(event->getLocation().x, event->getLocation().y, 0.0f);
            const auto& transform = armatureDisplay->getWorldToNodeTransform();
            transform.transformPoint(&_helpPoint);

            bone->offset.x = _helpPoint.x + _dragOffset.x;
            bone->offset.y = _helpPoint.y + _dragOffset.y;
            bone->invalidUpdate();
        }
    }
};

#endif // DRAG_HELPER_H
