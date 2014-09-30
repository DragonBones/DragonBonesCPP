#include "DemoKnight.h"

DemoKnight::DemoKnight()
: _armature(nullptr)
, _armArmature(nullptr)
{

}

DemoKnight::~DemoKnight()
{
    _armature->dispose();
    _armArmature->dispose();
    CC_SAFE_DELETE(_armature);
    CC_SAFE_DELETE(_armArmature);
}