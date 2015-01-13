#include "DBCCArmature.h"
#include "DBCCEventDispatcher.h"

NAME_SPACE_DRAGON_BONES_BEGIN
cocos2d::Node* DBCCArmature::getCCDisplay() const
{
    return static_cast<cocos2d::Node*>(_display);
}

cocos2d::EventDispatcher* DBCCArmature::getCCEventDispatcher() const
{
    return static_cast<DBCCEventDispatcher*>(_eventDispatcher)->eventDispatcher;
}

DBCCArmature::DBCCArmature(ArmatureData *armatureData, Animation *animation, IEventDispatcher *eventDispatcher, cocos2d::Node *display)
    : Armature(armatureData, animation, eventDispatcher, display)
{
}
DBCCArmature::~DBCCArmature()
{
    dispose();
}
void DBCCArmature::dispose()
{
    _delayDispose = true;
    if(!_animation || _lockDispose)
    {
        return;
    }

    if (_display)
    {
        getCCDisplay()->cleanup();
        getCCDisplay()->release();
    }
    
    Armature::dispose();
}

Rectangle DBCCArmature::getBoundingBox()
{
    auto r = getCCBoundingBox();
    return Rectangle(r.origin.x, r.origin.y, r.size.width, r.size.height);
}

cocos2d::Rect DBCCArmature::getCCBoundingBox()
{
    float minx = 0, miny = 0, maxx = 0, maxy = 0;
    bool first = true;
    for (const auto slot : _slotList)
    {
        if (!slot->getVisible() || !slot->isShowDisplay()) { continue; }
        auto r = slot->getBoundingBox();
        if (first)
        {
            first = false;
            minx = r.x;
            miny = r.y;
            maxx = r.x + r.width;
            maxy = r.y + r.height;
        }
        else
        {
            minx = r.x < minx ? r.x : minx;
            miny = r.y < miny ? r.y : miny;
            maxx = r.x + r.width > maxx ? r.x + r.width : maxx;
            maxy = r.y + r.height > maxy ? r.y + r.height : maxy;
        }
    }
    cocos2d::Rect rect(minx, miny, maxx - minx, maxy - miny);
    return cocos2d::RectApplyTransform(rect, getCCDisplay()->getNodeToParentTransform());
}

DBCCSlot* DBCCArmature::getCCSlot(const std::string &slotName) const
{
    Slot *slot = getSlot(slotName);
    return slot ? static_cast<DBCCSlot*>(slot) : nullptr;
}
NAME_SPACE_DRAGON_BONES_END