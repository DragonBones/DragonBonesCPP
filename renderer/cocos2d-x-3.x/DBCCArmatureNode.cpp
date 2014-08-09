#include "DBCCArmatureNode.h"
#include "DBCCEventDispatcher.h"

NAME_SPACE_DRAGON_BONES_BEGIN

DBCCArmatureNode* DBCCArmatureNode::create(DBCCArmature* armature)
{
	DBCCArmatureNode * ret = new DBCCArmatureNode();
	if (ret && ret->initWithDBCCArmature(armature))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool DBCCArmatureNode::initWithDBCCArmature(DBCCArmature* armature)
{
	if (armature != nullptr)
	{
		_armature = armature;
		addChild(armature->getCCDisplay());
		return true;
	}

	return false;
}


DBCCArmatureNode::DBCCArmatureNode()
{
}
DBCCArmatureNode::~DBCCArmatureNode()
{
    if (_armature)
    {
		_armature->dispose();
		_armature = nullptr;
    }
}

cocos2d::Rect DBCCArmatureNode::getBoundingBox() const
{
	float minx, miny, maxx, maxy = 0;

	bool first = true;

	cocos2d::Rect boundingBox;

	for (const auto slot : _armature->getSlots())
	{
		if (! slot->getVisible()){ continue; }
		auto node = static_cast<cocos2d::Node *>(slot->getDisplay());
		auto r = node->getBoundingBox();
		if(first)
		{
			minx = r.getMinX();
			miny = r.getMinY();
			maxx = r.getMaxX();
			maxy = r.getMaxY();

			first = false;
		}
		else
		{
			minx = r.getMinX() < boundingBox.getMinX() ? r.getMinX() : boundingBox.getMinX();
			miny = r.getMinY() < boundingBox.getMinY() ? r.getMinY() : boundingBox.getMinY();
			maxx = r.getMaxX() > boundingBox.getMaxX() ? r.getMaxX() : boundingBox.getMaxX();
			maxy = r.getMaxY() > boundingBox.getMaxY() ? r.getMaxY() : boundingBox.getMaxY();
		}
		boundingBox.setRect(minx, miny, maxx - minx, maxy - miny);
	}

	auto display = _armature->getCCDisplay();
	return cocos2d::RectApplyTransform(boundingBox, display->getNodeToParentTransform());
}

NAME_SPACE_DRAGON_BONES_END