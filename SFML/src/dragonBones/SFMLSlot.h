/*
*********************************************************************
* File          : SFMLSlot.h
* Project		: DragonBonesSFML
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
* License   	: MIT License
*********************************************************************
*/

#pragma once

#include <memory>

#include <dragonBones\DragonBonesHeaders.h>

#include "SFMLDisplay.h"

DRAGONBONES_NAMESPACE_BEGIN

class SFMLSlot : public Slot
{
	BIND_CLASS_TYPE_A(SFMLSlot);

private:
	float _textureScale;
	std::unique_ptr<SFMLDisplay> _renderDisplay;

public:
	virtual void _updateVisible() override;
	virtual void _updateBlendMode() override;
	virtual void _updateColor() override;

protected:
	virtual void _initDisplay(void* value) override;
	virtual void _disposeDisplay(void* value) override;
	virtual void _onUpdateDisplay() override;
	virtual void _addDisplay() override;
	virtual void _replaceDisplay(void* value, bool isArmatureDisplay) override;
	virtual void _removeDisplay() override;
	virtual void _updateZOrder() override;

	virtual void _updateFrame() override;
	virtual void _updateMesh() override;
	virtual void _updateTransform() override;
	virtual void _identityTransform() override;

	virtual void _onClear() override;
};

DRAGONBONES_NAMESPACE_END
