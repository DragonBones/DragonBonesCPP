#ifndef DBCC_UTILS_H
#define DBCC_UTILS_H

#include "DBCCArmature.h"

class GLProgramState;

NAME_SPACE_DRAGON_BONES_BEGIN

class DBCCUtils
{
public:
    static void setAllBlendFunc(DBCCArmature *armature, int src, int dest);
    static void setAllGLParamState(DBCCArmature *armature, cocos2d::GLProgramState *state);

private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCUtils);
};
NAME_SPACE_DRAGON_BONES_END
#endif