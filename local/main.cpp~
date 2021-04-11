#include <stdio>
#include <string>
#include <stdlib>
#include <algorithm>
#include <windows.h>
#include "PainterEngine_Startup.h"
#include "./PainterEngine_Application.h"
using namespace std;

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

PX_Application App;

void start_gui()
{
	PX_ApplicationInitialize(App,640,640);
//	PX_ApplicationRender(App,60);
	while(PX_loop())
	{
		PX_RuntimeRenderClear(&pApp->runtime,PX_OBJECT_UI_DEFAULT_BACKGROUNDCOLOR);
		PX_RuntimeRenderClear(&pApp->runtime,PX_COLOR(255,255,255,255));
	}
	return 0;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
//int main()
{
	start_gui();
	return 0;
}
