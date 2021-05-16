#include "PainterEngine_Application.h"
#include "PainterEngine_Event.h"
#include "DS.h"

PX_Application App;
px_int timecounter;

px_bool PX_ApplicationInitialize(PX_Application *pApp,px_int screen_width,px_int screen_height)
{
	timecounter=0;
	PX_ApplicationInitializeDefault(&pApp->runtime, screen_width, screen_height);
	PX_Runtime* pRuntime=&pApp->runtime;
	Data_Structure_load(pApp);

	do
	{
		if(!PX_FontModuleInitialize(&pRuntime->mp_resources,&pApp->fontmodule))return PX_FALSE; //初始化字模库
		if (!PX_LoadFontModuleFromFile(&pApp->fontmodule,"assets/font_Kesong_gbk_32.pxf")) return PX_FALSE; //加载字模
		if(!PX_UIInitialize(&pRuntime->mp_game,&pRuntime->mp_ui,&pApp->ui,&pApp->fontmodule))return PX_FALSE;//初始化UI库

		if(!PX_JsonInitialize(&pRuntime->mp_resources,&pApp->ui_json)) return PX_FALSE;//初始化JSON库
		if(!PX_LoadJsonFromFile(&pApp->ui_json,"assets/BootUpElement.json"))return PX_FALSE;//加载UI描述的JSON文件
		pApp->ui_root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->ui_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);//从JSON中加载UI数据

		if(!PX_JsonInitialize(&pRuntime->mp_resources,&pApp->info_json)) return PX_FALSE;//初始化JSON库
		if(!PX_LoadJsonFromFile(&pApp->info_json,"assets/InfoElement.json"))return PX_FALSE;//加载UI描述的JSON文件
		pApp->info_root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->info_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);//从JSON中加载UI数据

		if(!PX_JsonInitialize(&pRuntime->mp_resources,&pApp->check_fail_page_json)) return PX_FALSE;//初始化JSON库
		if(!PX_LoadJsonFromFile(&pApp->check_fail_page_json,"assets/CheckFailed.json"))return PX_FALSE;//加载UI描述的JSON文件
		pApp->check_fail_page_root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->check_fail_page_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);//从JSON中加载UI数据

		if(!PX_JsonInitialize(&pRuntime->mp_resources,&pApp->ensure_page_json)) return PX_FALSE;//初始化JSON库
		if(!PX_LoadJsonFromFile(&pApp->ensure_page_json,"assets/ReassurePageElement.json"))return PX_FALSE;//加载UI描述的JSON文件
		pApp->ensure_page_root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->ensure_page_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);//从JSON中加载UI数据

		if(!PX_JsonInitialize(&pRuntime->mp_resources,&pApp->book_successful_json)) return PX_FALSE;//初始化JSON库
		if(!PX_LoadJsonFromFile(&pApp->book_successful_json,"assets/BookSuccessfulElement.json"))return PX_FALSE;//加载UI描述的JSON文件
		pApp->book_successful_root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->book_successful_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);//从JSON中加载UI数据
	}while(0);

	PX_ObjectSetVisible(pApp->ui_root,1);
	PX_ObjectSetVisible(pApp->info_root,0);
	PX_ObjectSetVisible(pApp->check_fail_page_root,0);
	PX_ObjectSetVisible(pApp->ensure_page_root,0);
	PX_ObjectSetVisible(pApp->book_successful_root,0);

	Data_Structure_init(pApp);
	do
	{
		PX_ObjectRegisterEvent(PX_UIGetObjectByID(&pApp->ui,"start_time_yy"),PX_OBJECT_EVENT_VALUECHANGED,PX_ApplicationOnStartTimeSelectBarValueChanged,pApp);
		PX_ObjectRegisterEvent(PX_UIGetObjectByID(&pApp->ui,"start_time_mm"),PX_OBJECT_EVENT_VALUECHANGED,PX_ApplicationOnStartTimeSelectBarValueChanged,pApp);
		PX_ObjectRegisterEvent(PX_UIGetObjectByID(&pApp->ui,"end_time_yy"),PX_OBJECT_EVENT_VALUECHANGED,PX_ApplicationOnEndTimeSelectBarValueChanged,pApp);
		PX_ObjectRegisterEvent(PX_UIGetObjectByID(&pApp->ui,"end_time_mm"),PX_OBJECT_EVENT_VALUECHANGED,PX_ApplicationOnEndTimeSelectBarValueChanged,pApp);
		PX_ObjectRegisterEvent(PX_UIGetObjectByID(&pApp->ui,"search_button"),PX_OBJECT_EVENT_EXECUTE,PX_ApplicationOnSearchButtonClicked,pApp);
		PX_ObjectRegisterEvent(PX_UIGetObjectByID(&pApp->ui,"Date"),PX_OBJECT_EVENT_VALUECHANGED,PX_ApplicationOnSearchDateChanged,pApp);
		PX_ObjectRegisterEvent(PX_UIGetObjectByID(&pApp->ui,"Flight"),PX_OBJECT_EVENT_VALUECHANGED,PX_ApplicationOnSearchFlightNoChanged,pApp);
		PX_ObjectRegisterEvent(PX_UIGetObjectByID(&pApp->ui,"Book_button"),PX_OBJECT_EVENT_EXECUTE,PX_ApplicationOnBookButtonClicked,pApp);
		PX_ObjectRegisterEvent(PX_UIGetObjectByID(&pApp->ui,"EnsureButton"),PX_OBJECT_EVENT_EXECUTE,PX_ApplicationOnEnsureButtonClicked,pApp);

		PX_ObjectRegisterEvent(PX_UIGetObjectByID(&pApp->ui,"SwitchToInfoPage_button"),PX_OBJECT_EVENT_EXECUTE,PX_ApplicationInfoPageShowButtonClicked,pApp);
		PX_ObjectRegisterEvent(PX_UIGetObjectByID(&pApp->ui,"SwitchToBootUpPage_button"),PX_OBJECT_EVENT_EXECUTE,PX_ApplicationReturnButtonClicked,pApp);
		PX_ObjectRegisterEvent(PX_UIGetObjectByID(&pApp->ui,"SwitchToBootUpPage_button2"),PX_OBJECT_EVENT_EXECUTE,PX_ApplicationReturnButtonClicked,pApp);
		PX_ObjectRegisterEvent(PX_UIGetObjectByID(&pApp->ui,"SwitchToBootUpPage_button3"),PX_OBJECT_EVENT_EXECUTE,PX_ApplicationReturnButtonClicked,pApp);

		PX_ObjectRegisterEvent(PX_UIGetObjectByID(&pApp->ui,"SwitchToBootUpPage_button4"),PX_OBJECT_EVENT_EXECUTE,PX_ApplicationPageRefresh,pApp);
		PX_ObjectRegisterEvent(PX_UIGetObjectByID(&pApp->ui,"InfoGrepButton"),PX_OBJECT_EVENT_EXECUTE,PX_ApplicationOnInfoGrepButtonClicked,pApp);
		PX_ObjectRegisterEvent(PX_UIGetObjectByID(&pApp->ui,"DeleteAcceptButton"),PX_OBJECT_EVENT_EXECUTE,PX_ApplicationOnDeleteAcceptButtonClicked,pApp);
		PX_ObjectRegisterEvent(PX_UIGetObjectByID(&pApp->ui,"DeleteRequestingButton"),PX_OBJECT_EVENT_EXECUTE,PX_ApplicationOnDeleteRequestingButtonClicked,pApp);
	} while (0);

	return PX_TRUE;
}


px_void PX_ApplicationUpdate(PX_Application *pApp,px_dword elpased)
{
	PX_ObjectUpdate(pApp->ui_root,elpased);
	PX_ObjectUpdate(pApp->info_root,elpased);
	PX_ObjectUpdate(pApp->check_fail_page_root,elpased);
	PX_ObjectUpdate(pApp->ensure_page_root,elpased);
	PX_ObjectUpdate(pApp->book_successful_root,elpased);
	timecounter+=elpased;
	if(timecounter>120*1000)
	{
		timecounter=0;
		//Data_Write(pApp);
	}
}

px_void PX_ApplicationRender(PX_Application *pApp,px_dword elpased)
{
	px_surface *pRenderSurface=&pApp->runtime.RenderSurface;
	PX_Runtime* pRuntime=&pApp->runtime;
	PX_RuntimeRenderClear(&pApp->runtime,PX_OBJECT_UI_DEFAULT_BACKGROUNDCOLOR);
	//PX_GeoDrawBorder(pRenderSurface,100,100,300,300,8,PX_COLOR(220,220,220,220));
	//PX_GeoDrawBorder(pRenderSurface,200,200,500,500,8,PX_COLOR(128,128,128,128));
	PX_ObjectRender(pRenderSurface,pApp->ui_root,elpased);
	PX_ObjectRender(pRenderSurface,pApp->info_root,elpased);
	PX_ObjectRender(pRenderSurface,pApp->check_fail_page_root,elpased);
	PX_ObjectRender(pRenderSurface,pApp->ensure_page_root,elpased);
	PX_ObjectRender(pRenderSurface,pApp->book_successful_root,elpased);
}

px_void PX_ApplicationPostEvent(PX_Application *pApp,PX_Object_Event e)
{
	PX_Runtime* pRuntime=&pApp->runtime;
	if(e.Event==PX_OBJECT_EVENT_WINDOWRESIZE)
	{
		PX_UIUpdateObjectsPostions(&pApp->ui,pApp->ui_root,&pApp->ui_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);
		PX_UIUpdateObjectsPostions(&pApp->ui,pApp->info_root,&pApp->info_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);
	}

	PX_ApplicationEventDefault(&pApp->runtime, e);
	if(pApp->ui_root->Visible)
		PX_ObjectPostEvent(pApp->ui_root,e);
	if(pApp->info_root->Visible)
		PX_ObjectPostEvent(pApp->info_root,e);
	if(pApp->check_fail_page_root->Visible)
		PX_ObjectPostEvent(pApp->check_fail_page_root,e);
	if(pApp->ensure_page_root->Visible)
		PX_ObjectPostEvent(pApp->ensure_page_root,e);
	if(pApp->book_successful_root->Visible)
		PX_ObjectPostEvent(pApp->book_successful_root,e);

}
