

#ifndef DBCC_MACRO_H
#define DBCC_MACRO_H

#include "base/ccMacros.h"

#ifndef DRAGON_BONES_ENABLE_LUA
#define DRAGON_BONES_ENABLE_LUA 0
#endif // !DRAGON_BONES_ENABLE_LUA

#undef DBASSERT
#define DBASSERT CCASSERT


#endif // DBCC_MACRO_H
