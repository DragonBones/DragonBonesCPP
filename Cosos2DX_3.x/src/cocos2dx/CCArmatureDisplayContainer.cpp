#include "CCArmatureDisplayContainer.h"

NAMESPACE_DRAGONBONES_BEGIN

CCArmatureDisplayContainer* CCArmatureDisplayContainer::create()
{
    CCArmatureDisplayContainer* displayContainer = new CCArmatureDisplayContainer();

    if (displayContainer != nullptr)
    {
        displayContainer->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(displayContainer);
    }

    return displayContainer;
}

CCArmatureDisplayContainer::CCArmatureDisplayContainer() :
    _armature(nullptr)
{}
CCArmatureDisplayContainer::~CCArmatureDisplayContainer() {}

NAMESPACE_DRAGONBONES_END
