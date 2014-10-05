#ifndef DRAGON_BONES_HEADERS_H
#define DRAGON_BONES_HEADERS_H

#include "DragonBones.h"

// geoms
#include "geoms/ColorTransform.h"
#include "geoms/Matrix.h"
#include "geoms/Point.h"
#include "geoms/Rectangle.h"
#include "geoms/Transform.h"

// objects
#include "objects/IAreaData.h"
#include "objects/RectangleData.h"
#include "objects/EllipseData.h"
#include "objects/Frame.h"
#include "objects/TransformFrame.h"
#include "objects/Timeline.h"
#include "objects/TransformTimeline.h"
#include "objects/AnimationData.h"
#include "objects/DisplayData.h"
#include "objects/SlotData.h"
#include "objects/SkinData.h"
#include "objects/BoneData.h"
#include "objects/ArmatureData.h"
#include "objects/DragonBonesData.h"

// textures
#include "textures/TextureData.h"
#include "textures/TextureAtlasData.h"
#include "textures/ITextureAtlas.h"

// events
#include "events/EventData.h"
#include "events/IEventDispatcher.h"

// animation
#include "animation/IAnimatable.h"
#include "animation/TimelineState.h"
#include "animation/AnimationState.h"
#include "animation/Animation.h"
#include "animation/WorldClock.h"

// core
#include "core/Object.h"
#include "core/Armature.h"
#include "core/Bone.h"
#include "core/Slot.h"

// parsers
#include "parsers/BaseDataParser.h"
#include "parsers/XMLDataParser.h"

// factories
#include "factories/BaseFactory.h"

#endif  // DRAGON_BONES_HEADERS_H
