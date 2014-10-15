#ifndef ANIMATION_I_ANIMATABLE_H
#define ANIMATION_I_ANIMATABLE_H

NAME_SPACE_DRAGON_BONES_BEGIN
class IAnimatable
{
public:
    IAnimatable() {};
    virtual ~IAnimatable() {};
    virtual void advanceTime(float passedTime) = 0;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(IAnimatable);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // ANIMATION_I_ANIMATABLE_H
