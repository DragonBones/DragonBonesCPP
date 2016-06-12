#include "CCArmatureDisplayContainer.h"

DRAGONBONES_NAMESPACE_BEGIN

CCArmatureDisplayContainer* CCArmatureDisplayContainer::create()
{
    CCArmatureDisplayContainer* displayContainer = new (std::nothrow) CCArmatureDisplayContainer();

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
    _armature(nullptr),
	_dispatcher()
{
}
CCArmatureDisplayContainer::~CCArmatureDisplayContainer() {}

void CCArmatureDisplayContainer::_onClear()
{
    this->release();

    _armature = nullptr;

	_dispatcher.removeAllEventListeners();
}

void CCArmatureDisplayContainer::_dispatchEvent(EventObject* value)
{
	_dispatcher.dispatchCustomEvent(value->type, value);
}

DRAGONBONES_NAMESPACE_END
