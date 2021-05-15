#ifndef DS_H
#define DS_H

#ifdef __cplusplus
#include <map>
#include <vector>
#include <cstdlib>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include "PainterEngine_Startup.h"
#include "PainterEngine_Application.h"
#include "core/PX_MemoryPool.h"

px_bool Data_Structure_load(PX_Application *pApp);
px_bool Data_Structure_init(PX_Application *pApp);
px_bool Data_Structure_query(px_list *result,px_int from,px_int to,px_char Departure[],px_char Arrival[]);
px_bool Data_Write(PX_Application *pApp);


#ifdef  __cplusplus
}
#endif

extern PX_Json JsonData;
extern px_int Start_Time_int,End_Time_int;

#endif
