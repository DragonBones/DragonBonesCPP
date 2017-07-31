#ifndef DRAGONBONES_HEADERS_H
#define DRAGONBONES_HEADERS_H

// core
#include "core/DragonBones.h"
#include "core/BaseObject.h"

// geom
#include "geom/Matrix.h"
#include "geom/Transform.h"
#include "geom/ColorTransform.h"
#include "geom/Point.h"
#include "geom/Rectangle.h"

// model
#include "model/TextureAtlasData.h"
#include "model/UserData.h"
#include "model/DragonBonesData.h"
#include "model/ArmatureData.h"
#include "model/ConstraintData.h"
#include "model/DisplayData.h"
#include "model/BoundingBoxData.h"
#include "model/AnimationData.h"
#include "model/AnimationConfig.h"

// armature
#include "armature/IArmatureProxy.h"
#include "armature/Armature.h"
#include "armature/TransformObject.h"
#include "armature/Bone.h"
#include "armature/Slot.h"
#include "armature/Constraint.h"

// animation
#include "animation/IAnimatable.h"
#include "animation/WorldClock.h"
#include "animation/Animation.h"
#include "animation/AnimationState.h"
#include "animation/BaseTimelineState.h"
#include "animation/TimelineState.h"

// event
#include "events/EventObject.h"
#include "events/IEventDispatcher.h"

#ifndef EGRET_WASM
// parser
#include "parsers/DataParser.h"
#include "parsers/JSONDataParser.h"
// factory
#include "factories/BaseFactory.h"
#endif //EGRET_WASM


#endif // DRAGONBONES_HEADERS_H
