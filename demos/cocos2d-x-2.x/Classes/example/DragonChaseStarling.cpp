#include "example/DragonChaseStarling.h"

DragonChaseStarling::DragonChaseStarling()
: _db(NULL)
, _starlingBird(NULL)
, _head(NULL)
, _armR(NULL)
, (NULL)
{
    _db = CCDragonBones::create("dragon/skeleton.xml", "dragon/texture.xml", "Dragon", "Dragon", "");
    this->addChild(_db);
    _db->setPosition(VisibleRect::bottom(0, _footY));

    _starlingBird = CCSprite::create("starling.png");
    this->addChild(_starlingBird);

    _head = _db->getArmature()->getBone("head");
    _armR = _db->getArmature()->getBone("armUpperR");
    _armL = _db->getArmature()->getBone("armUpperL");
}


std::string DragonChaseStarling::title()
{
    return "Dragon Chase Starling";
}