#ifndef PAINTERENGINE_EVENT_H
#define PAINTERENGINE_EVENT_H

#include "PainterEngine_Startup.h"
#include "PainterEngine_Application.h"
#include "lib.h"
#include "DS.h"

px_void PX_ApplicationOnStartTimeSelectBarValueChanged(PX_Object *pObject,PX_Object_Event e,px_void *ptr);
px_void PX_ApplicationOnEndTimeSelectBarValueChanged(PX_Object *pObject,PX_Object_Event e,px_void *ptr);


px_void PX_ApplicationOnSearchButtonClicked(PX_Object *pObject,PX_Object_Event e,px_void *ptr);
#endif
