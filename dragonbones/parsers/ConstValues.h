#ifndef __CONST_VALUES_H__
#define __CONST_VALUES_H__

#include "../DragonBones.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class ConstValues
{
public:
    static const String DRAGON_BONES;
    static const String ARMATURE;
    static const String SKIN;
    static const String BONE;
    static const String SLOT;
    static const String DISPLAY;
    static const String ANIMATION;
    static const String TIMELINE;
    static const String FRAME;
    static const String TRANSFORM;
    static const String COLOR_TRANSFORM;
    static const String RECTANGLE;
    static const String ELLIPSE;
    
    static const String TEXTURE_ATLAS;
    static const String SUB_TEXTURE;
    
    static const String A_ROTATED;
    static const String A_FRAME_X;
    static const String A_FRAME_Y;
    static const String A_FRAME_WIDTH;
    static const String A_FRAME_HEIGHT;
    
    static const String A_VERSION;
    static const String A_IMAGE_PATH;
    static const String A_FRAME_RATE;
    static const String A_NAME;
    static const String A_PARENT;
    static const String A_LENGTH;
    static const String A_TYPE;
    static const String A_FADE_IN_TIME;
    static const String A_DURATION;
    static const String A_SCALE;
    static const String A_OFFSET;
    static const String A_LOOP;
    static const String A_EVENT;
    static const String A_EVENT_PARAMETERS;
    static const String A_SOUND;
    static const String A_ACTION;
    static const String A_HIDE;
    static const String A_AUTO_TWEEN;
    static const String A_TWEEN_EASING;
    static const String A_TWEEN_ROTATE;
    static const String A_TWEEN_SCALE;
    static const String A_DISPLAY_INDEX;
    static const String A_Z_ORDER;
    static const String A_BLENDMODE;
    static const String A_WIDTH;
    static const String A_HEIGHT;
    static const String A_INHERIT_ROTATION;
    static const String A_INHERIT_SCALE;
    static const String A_X;
    static const String A_Y;
    static const String A_SKEW_X;
    static const String A_SKEW_Y;
    static const String A_SCALE_X;
    static const String A_SCALE_Y;
    static const String A_PIVOT_X;
    static const String A_PIVOT_Y;
    static const String A_ALPHA_OFFSET;
    static const String A_RED_OFFSET;
    static const String A_GREEN_OFFSET;
    static const String A_BLUE_OFFSET;
    static const String A_ALPHA_MULTIPLIER;
    static const String A_RED_MULTIPLIER;
    static const String A_GREEN_MULTIPLIER;
    static const String A_BLUE_MULTIPLIER;
    
    // add scale offset to frame
    static const String A_SCALE_X_OFFSET;
    static const String A_SCALE_Y_OFFSET;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(ConstValues);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __CONST_VALUES_H__