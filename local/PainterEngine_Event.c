#include "PainterEngine_Event.h"

px_void PX_ApplicationOnStartTimeSelectBarValueChanged(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	const px_int tot_day[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	px_char *Start_Time_Info[3];
	px_int Start_Time_Int[3];
	PX_Application *pApp=(PX_Application *)ptr;
	Start_Time_Info[0]=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"start_time_yy"));
	Start_Time_Info[1]=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"start_time_mm"));
	Start_Time_Info[2]=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"start_time_dd"));
	Start_Time_Int[0]=PX_atoi(Start_Time_Info[0]);
	Start_Time_Int[1]=PX_atoi(Start_Time_Info[1]);
	Start_Time_Int[2]=PX_atoi(Start_Time_Info[2]);
	px_int expect_totday=tot_day[Start_Time_Int[1]]+((Start_Time_Int[1]==2&&(Start_Time_Int[0]&3)==0&&(Start_Time_Int[0]%100)!=0)||Start_Time_Int[0]%400==0);
	PX_Object *Startdd_Object=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"start_time_dd");
	PX_Object_SelectBar *Startdd_Selectbar=PX_Object_GetSelectBar(Startdd_Object);
	px_int cur_totday=Startdd_Selectbar->Items.size;
	char tmp[3];
	while(cur_totday!=expect_totday)
	{
		if(cur_totday>expect_totday)
			PX_Object_SelectBarRemoveItem(Startdd_Object,--cur_totday);
		else
		{
			PX_itoa(++cur_totday,tmp,3,10);
			PX_Object_SelectBarAddItem(Startdd_Object,tmp);
		}
	}
}

px_void PX_ApplicationOnEndTimeSelectBarValueChanged(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	const px_int tot_day[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	px_char *End_Time_Info[3];
	px_int End_Time_Int[3];
	PX_Application *pApp=(PX_Application *)ptr;
	End_Time_Info[0]=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"end_time_yy"));
	End_Time_Info[1]=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"end_time_mm"));
	End_Time_Info[2]=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"end_time_dd"));
	End_Time_Int[0]=PX_atoi(End_Time_Info[0]);
	End_Time_Int[1]=PX_atoi(End_Time_Info[1]);
	End_Time_Int[2]=PX_atoi(End_Time_Info[2]);
	px_int expect_totday=tot_day[End_Time_Int[1]]+((End_Time_Int[1]==2&&(End_Time_Int[0]&3)==0&&(End_Time_Int[0]%100)!=0)||End_Time_Int[0]%400==0);
	PX_Object *Enddd_Object=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"end_time_dd");
	PX_Object_SelectBar *Enddd_Selectbar=PX_Object_GetSelectBar(Enddd_Object);
	px_int cur_totday=Enddd_Selectbar->Items.size;
	char tmp[3];
	while(cur_totday!=expect_totday)
	{
		if(cur_totday>expect_totday)
			PX_Object_SelectBarRemoveItem(Enddd_Object,--cur_totday);
		else
		{
			PX_itoa(++cur_totday,tmp,3,10);
			PX_Object_SelectBarAddItem(Enddd_Object,tmp);
		}
	}
}

px_list query_saved;

px_void PX_ApplicationOnSearchButtonClicked(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	PX_Application *pApp=(PX_Application *)ptr;
	PX_Runtime *pRuntime=&pApp->runtime;
	px_char *Start_Time_Info[3],*End_Time_Info[3];
	px_int End_Time_Int[3],Start_Time_Int[3];
	px_int Start_Time_Stamp,End_Time_Stamp;
	do
	{
		Start_Time_Info[0]=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"start_time_yy"));
		Start_Time_Info[1]=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"start_time_mm"));
		Start_Time_Info[2]=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"start_time_dd"));
		Start_Time_Int[0]=PX_atoi(Start_Time_Info[0]);
		Start_Time_Int[1]=PX_atoi(Start_Time_Info[1]);
		Start_Time_Int[2]=PX_atoi(Start_Time_Info[2]);

		End_Time_Info[0]=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"end_time_yy"));
		End_Time_Info[1]=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"end_time_mm"));
		End_Time_Info[2]=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"end_time_dd"));
		End_Time_Int[0]=PX_atoi(End_Time_Info[0]);
		End_Time_Int[1]=PX_atoi(End_Time_Info[1]);
		End_Time_Int[2]=PX_atoi(End_Time_Info[2]);

		Start_Time_Stamp=calc_Date_Stamp_by_int(Start_Time_Int[0],Start_Time_Int[1],Start_Time_Int[2]);
		End_Time_Stamp=calc_Date_Stamp_by_int(End_Time_Int[0],End_Time_Int[1],End_Time_Int[2]);
	}while(0);

	if(Start_Time_Stamp>End_Time_Stamp) return;

	px_char *Departure,*Arrival;
	do
	{
		Departure=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"departure"));
		Arrival=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"arrival"));
	}while(0);

	do
	{
		px_int DateTimeStamp;
		px_char DateTimeString[20];
		PX_Object *Date_Object=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Date");
		PX_Object_SelectBar *Date_Selectbar=PX_Object_GetSelectBar(Date_Object);
		PX_VectorPop(&Date_Selectbar->Items);
		for(DateTimeStamp=Start_Time_Stamp;DateTimeStamp<=End_Time_Stamp;++DateTimeStamp)
		{
			Convert_Date_Stamp_to_char(DateTimeStamp,DateTimeString);
			PX_Object_SelectBarAddItem(Date_Object,DateTimeString);
		}

	}
	while(0);//清空selectbar

	PX_ListClear(&query_saved);
	query_saved.mp=PX_NULL;
	/*
	long long rec[5]={
		(long long)&query_saved,
		(long long)Start_Time_Stamp,
		(long long)End_Time_Stamp,
		(long long)Departure,
		(long long)Arrival
	};

	HANDLE hThread = (HANDLE)_beginthread(ThreadProcDSQ, 1, rec);*/
	PX_ListInitialize(&pRuntime->mp_resources,&query_saved);
	Data_Structure_query(&query_saved,
			Start_Time_Stamp,
			End_Time_Stamp,
			Departure,
			Arrival
			);//查询语句


	px_list_node *node=query_saved.head;
	PX_Json_Value *cur;
	while(node)
	{
		cur=(PX_Json_Value*)node->pdata;
		node=node->pnext;
	}
}

px_void PX_ApplicationOnSearchDateChanged(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
}
