#ifndef PAINTERENGINE_APPLICATION_H
#define PAINTERENGINE_APPLICATION_H

#include "PainterEngine_Startup.h"

#include <process.h>
#include <Windows.h>

typedef struct
{
	PX_FontModule fontmodule;
	PX_Runtime runtime;
	PX_Json ui_json,info_json;
	PX_UI ui,info;
	PX_Object *ui_root,*info_root;
}PX_Application;

extern PX_Application App;

px_bool PX_ApplicationInitialize(PX_Application *App,px_int screen_Width,px_int screen_Height);
px_void PX_ApplicationUpdate(PX_Application *App,px_dword elpased);
px_void PX_ApplicationRender(PX_Application *App,px_dword elpased);
px_void PX_ApplicationPostEvent(PX_Application *App,PX_Object_Event e);

#endif
