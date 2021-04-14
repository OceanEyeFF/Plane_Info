#include "PainterEngine_Application.h"

PX_Application App;

px_bool PX_ApplicationInitialize(PX_Application *pApp,px_int screen_width,px_int screen_height)
{
	PX_ApplicationInitializeDefault(&pApp->runtime, screen_width, screen_height);
	PX_Runtime* pRuntime=&pApp->runtime;
	if(!PX_FontModuleInitialize(&pRuntime->mp_resources,&pApp->fontmodule))return PX_FALSE; //初始化字模库
	if (!PX_LoadFontModuleFromFile(&pApp->fontmodule,"assets/font1_KeSong.pxf")) return PX_FALSE; //加载字模
	if(!PX_JsonInitialize(&pRuntime->mp_resources,&pApp->ui_json)) return PX_FALSE;//初始化JSON库
	if(!PX_LoadJsonFromFile(&pApp->ui_json,"assets/BootUpElement.json"))return PX_FALSE;//加载UI描述的JSON文件
	if(!PX_UIInitialize(&pRuntime->mp_game,&pRuntime->mp_ui,&pApp->ui,&pApp->fontmodule))return PX_FALSE;//初始化UI库
	pApp->ui_root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->ui_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);//从JSON中加载UI数据

	return PX_TRUE;
}

px_void PX_ApplicationUpdate(PX_Application *pApp,px_dword elpased)
{
	PX_ObjectUpdate(pApp->ui_root,elpased);
}

px_void PX_ApplicationRender(PX_Application *pApp,px_dword elpased)
{
	px_surface *pRenderSurface=&pApp->runtime.RenderSurface;
	PX_Runtime* pRuntime=&pApp->runtime;
	PX_RuntimeRenderClear(&pApp->runtime,PX_OBJECT_UI_DEFAULT_BACKGROUNDCOLOR);
	//PX_GeoDrawBorder(pRenderSurface,100,100,300,300,8,PX_COLOR(220,220,220,220));
	//PX_GeoDrawBorder(pRenderSurface,200,200,500,500,8,PX_COLOR(128,128,128,128));
	PX_ObjectRender(pRenderSurface,pApp->ui_root,elpased);
}

px_void PX_ApplicationPostEvent(PX_Application *pApp,PX_Object_Event e)
{
	PX_Runtime* pRuntime=&pApp->runtime;
	px_surface *pRenderSurface=&pApp->runtime.RenderSurface;
	PX_ApplicationEventDefault(&pApp->runtime, e);
	PX_ObjectPostEvent(pApp->ui_root,e);

	if(e.Event==PX_OBJECT_EVENT_WINDOWRESIZE)
	{
		PX_UIUpdateObjectsPostions(&pApp->ui,pApp->ui_root,&pApp->ui_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);
	}

}

