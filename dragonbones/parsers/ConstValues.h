#ifndef CONST_VALUES_H
#define CONST_VALUES_H

#include "../DragonBones.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class ConstValues
{
public:
    static const std::string DRAGON_BONES;
    static const std::string ARMATURE;
    static const std::string SKIN;
    static const std::string BONE;
    static const std::string SLOT;
    static const std::string DISPLAY;
    static const std::string ANIMATION;
    static const std::string TIMELINE;
    static const std::string FRAME;
    static const std::string TRANSFORM;
    static const std::string COLOR_TRANSFORM;
    static const std::string RECTANGLE;
    static const std::string ELLIPSE;
    static const std::string TEXT;
    static const std::string COLOR;
    static const std::string SCALING_GRID;
    
    static const std::string TEXTURE_ATLAS;
    static const std::string SUB_TEXTURE;
    
    static const std::string A_ROTATED;
    static const std::string A_FRAME_X;
    static const std::string A_FRAME_Y;
    static const std::string A_FRAME_WIDTH;
    static const std::string A_FRAME_HEIGHT;
    
    static const std::string A_VERSION;
    static const std::string A_IMAGE_PATH;
    static const std::string A_IMAGE_FORMAT;
    static const std::string A_FRAME_RATE;
    static const std::string A_NAME;
    static const std::string A_PARENT;
    static const std::string A_LENGTH;
    static const std::string A_TYPE;
    static const std::string A_FADE_IN_TIME;
    static const std::string A_DURATION;
    static const std::string A_SCALE;
    static const std::string A_OFFSET;
    static const std::string A_LOOP;
    static const std::string A_EVENT;
    static const std::string A_EVENT_PARAMETERS;
    static const std::string A_SOUND;
    static const std::string A_ACTION;
    static const std::string A_HIDE;
    static const std::string A_AUTO_TWEEN;
    static const std::string A_TWEEN_EASING;
    static const std::string A_TWEEN_ROTATE;
    static const std::string A_TWEEN_SCALE;
    static const std::string A_DISPLAY_INDEX;
    static const std::string A_Z_ORDER;
    static const std::string A_BLENDMODE;
    static const std::string A_WIDTH;
    static const std::string A_HEIGHT;
    static const std::string A_INHERIT_ROTATION;
    static const std::string A_INHERIT_SCALE;
    static const std::string A_X;
    static const std::string A_Y;
    static const std::string A_SKEW_X;
    static const std::string A_SKEW_Y;
    static const std::string A_SCALE_X;
    static const std::string A_SCALE_Y;
    static const std::string A_PIVOT_X;
    static const std::string A_PIVOT_Y;
    static const std::string A_ALPHA_OFFSET;
    static const std::string A_RED_OFFSET;
    static const std::string A_GREEN_OFFSET;
    static const std::string A_BLUE_OFFSET;
    static const std::string A_ALPHA_MULTIPLIER;
    static const std::string A_RED_MULTIPLIER;
    static const std::string A_GREEN_MULTIPLIER;
    static const std::string A_BLUE_MULTIPLIER;

    static const std::string A_ALPHA;
    static const std::string A_RED;
    static const std::string A_GREEN;
    static const std::string A_BLUE;

    static const std::string A_LEFT;
    static const std::string A_RIGHT;
    static const std::string A_TOP;
    static const std::string A_BOTTOM;

    static const std::string A_BOLD;
    static const std::string A_ITALIC;
    static const std::string A_SIZE;
    static const std::string A_FACE;
    static const std::string A_ALIGN_H;
    static const std::string A_ALIGN_V;
    static const std::string A_LINE_TYPE;
    static const std::string A_TEXT_TYPE;
    static const std::string A_TEXT;
    static const std::string A_HTML_TEXT;
    static const std::string A_LETTER_SPACING;
    static const std::string A_LINE_SPACING;
    static const std::string A_MAX_CHARACTERS;
    
    // add scale offset to frame
    static const std::string A_SCALE_X_OFFSET;
    static const std::string A_SCALE_Y_OFFSET;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(ConstValues);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // CONST_VALUES_H