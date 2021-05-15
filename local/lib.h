#ifndef LIB
#define LIB

#ifdef __cplusplus
#include <cstdlib>
#include <cstdio>
#endif


#ifdef __cplusplus
extern "C"
{
#endif

#include "PainterEngine_Startup.h"
#include "PainterEngine_Application.h"
#include "core/PX_Memory.h"
#include "core/PX_MemoryPool.h"
#include "core/PX_Typedef.h"
px_int calc_Date_Stamp_by_int(px_int yy,px_int mm,px_int dd);
px_int calc_Date_Stamp(px_char s[],px_char inv[]);
px_int myhash(const px_char s[]);
px_void Convert_Date_Stamp_to_char(px_int stamp,px_char ret[]);
px_bool stringequal(px_char S1[],px_char s2[]);


#ifdef __cplusplus
}
#endif

#endif
