#include <stdio>
#include <string>
#include <stdlib>
#include <algorithm>
#include "PainterEngine_Startup.h"
#include "./PainterEngine_Application.h"

using namespace std;

PX_Application App;

void start_gui()
{
	PX_ApplicationInitialize(App,640,640);
	return 0;
}

int main()
{
	start_gui();
	return 0;
}
