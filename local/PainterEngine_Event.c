#include "PainterEngine_Event.h"

px_list query_saved;
px_list_node *rec[1000];
PX_Json tmpJson;

px_void goto_bootup_page(PX_Application *pApp)
{
	PX_ObjectSetVisible(pApp->ui_root,1);
	PX_ObjectSetVisible(pApp->info_root,0);
	PX_ObjectSetVisible(pApp->check_fail_page_root,0);
	PX_ObjectSetVisible(pApp->ensure_page_root,0);
	PX_ObjectSetVisible(pApp->book_successful_root,0);
}

px_void goto_info_page(PX_Application *pApp)
{
	PX_ObjectSetVisible(pApp->ui_root,0);
	PX_ObjectSetVisible(pApp->info_root,1);
	PX_ObjectSetVisible(pApp->check_fail_page_root,0);
	PX_ObjectSetVisible(pApp->ensure_page_root,0);
	PX_ObjectSetVisible(pApp->book_successful_root,0);
}

px_void goto_check_fail_page(PX_Application *pApp)
{
	PX_ObjectSetVisible(pApp->ui_root,0);
	PX_ObjectSetVisible(pApp->info_root,0);
	PX_ObjectSetVisible(pApp->check_fail_page_root,1);
	PX_ObjectSetVisible(pApp->ensure_page_root,0);
	PX_ObjectSetVisible(pApp->book_successful_root,0);
}

px_void goto_ensure_page(PX_Application *pApp)
{
	PX_ObjectSetVisible(pApp->ui_root,0);
	PX_ObjectSetVisible(pApp->info_root,0);
	PX_ObjectSetVisible(pApp->check_fail_page_root,0);
	PX_ObjectSetVisible(pApp->ensure_page_root,1);
	PX_ObjectSetVisible(pApp->book_successful_root,0);
}

px_void goto_successful_page(PX_Application *pApp)
{
	PX_ObjectSetVisible(pApp->ui_root,0);
	PX_ObjectSetVisible(pApp->info_root,0);
	PX_ObjectSetVisible(pApp->check_fail_page_root,0);
	PX_ObjectSetVisible(pApp->ensure_page_root,0);
	PX_ObjectSetVisible(pApp->book_successful_root,1);
}

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

px_void PX_ApplicationInfoPageShowButtonClicked(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	PX_Application *pApp=(PX_Application *)ptr;
	PX_Runtime *pRuntime=&pApp->runtime;
	goto_info_page(pApp);
}

px_void PX_ApplicationReturnButtonClicked(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	PX_Application *pApp=(PX_Application *)ptr;
	PX_Runtime *pRuntime=&pApp->runtime;
	goto_bootup_page(pApp);
}

px_void update_SearchFlightSeatInfo_by_FlightNO(PX_Application *pApp,px_int Start_Time_Stamp)
{
	PX_Runtime *pRuntime=&pApp->runtime;
	PX_Object *SearchDateObject=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Date");
	PX_Object *SearchFlightNoObject=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Flight");

	PX_Object_SelectBar *SearchFlightNoSelectBar=PX_Object_GetSelectBar(SearchFlightNoObject);
	PX_Object_SelectBar *SearchDateSelectBar=PX_Object_GetSelectBar(SearchDateObject);

	px_char *TimeString=PX_Object_SelectBarGetCurrentText(SearchDateObject);
	px_int Current_Time_Stamp=calc_Date_Stamp(TimeString,"/");

	px_list_node *pNode;
	PX_Json_Value *pData;
	{
		for(pNode=rec[Current_Time_Stamp-Start_Time_Stamp];;pNode=pNode->pnext)
		{
			if(!pNode) break;
			pData=(PX_Json_Value*)pNode->pdata;
			if(PX_JsonGetObjectValue(pData,"Date_Stamp")->_number!=Current_Time_Stamp)
				break;
			if(stringequal(PX_JsonGetObjectValue(pData,"Flight No")->_string.buffer,(px_char*)PX_Object_SelectBarGetCurrentText(SearchFlightNoObject))) break;
		}
	}while(0);

	PX_Object *SearchFlightSeatObject[3];
	{
		SearchFlightSeatObject[0]=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"RightBorderText1");
		SearchFlightSeatObject[1]=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"RightBorderText2");
		SearchFlightSeatObject[2]=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"RightBorderText3");
	}while(0);

	px_int ExpectSeat[3];
	{
		px_int i;
		for(i=0;i<3;++i) ExpectSeat[i]=PX_JsonGetArrayValue(PX_JsonGetObjectValue(pData,"Seats"),i)->_number-PX_JsonGetArrayValue(PX_JsonGetObjectValue(pData,"OccupiedSeats"),i)->_number;
	}while(0);

	px_int i;
	px_char tmpstring[10];
	for(i=0;i<3;++i)
	{
		PX_itoa(ExpectSeat[i],tmpstring,10,10);
		PX_Object_LabelSetText(SearchFlightSeatObject[i],tmpstring);
	}
}

px_void update_SearchFlightNo_by_DateInfo(PX_Application* pApp,px_int Start_Time_Stamp)
{
	PX_Runtime *pRuntime=&pApp->runtime;
	PX_Object *SearchFlightNoObject=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Flight");
	PX_Object *SearchDateObject=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Date");

	PX_Object_SelectBar *SearchFlightNoSelectBar=PX_Object_GetSelectBar(SearchFlightNoObject);
	PX_Object_SelectBar *SearchDateSelectBar=PX_Object_GetSelectBar(SearchDateObject);

	while(PX_VectorPop(&SearchFlightNoSelectBar->Items));

	px_char *TimeString=PX_Object_SelectBarGetCurrentText(SearchDateObject);
	px_int Current_Time_Stamp=calc_Date_Stamp(TimeString,"/");

	px_list_node *pNodeList;
	PX_Json_Value *pData;

	if(rec[Current_Time_Stamp-Start_Time_Stamp]==PX_NULL) return;
	do
	{
		for(pNodeList=rec[Current_Time_Stamp-Start_Time_Stamp];;pNodeList=pNodeList->pnext)
		{
			if(!pNodeList) break;
			pData=(PX_Json_Value*)pNodeList->pdata;
			if(PX_JsonGetObjectValue(pData,"Date_Stamp")->_number!=Current_Time_Stamp)
				break;
			PX_Object_SelectBarAddItem(SearchFlightNoObject,PX_JsonGetObjectValue(pData,"Flight No")->_string.buffer);
		}
	}
	while(0);//重置selectbar

	update_SearchFlightSeatInfo_by_FlightNO(pApp,Start_Time_Stamp);
}

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
	}
	while(0);// 获取开始时间结束时间

	if(Start_Time_Stamp>End_Time_Stamp) return;

	px_char *Departure,*Arrival;
	do
	{
		Departure=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"departure"));
		Arrival=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"arrival"));
	}
	while(0);//获取始发地，目的地

	if(stringequal(Departure,Arrival)) return;

	do
	{
		px_int DateTimeStamp;
		px_char DateTimeString[20];
		PX_Object *Date_Object=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Date");
		PX_Object_SelectBar *Date_Selectbar=PX_Object_GetSelectBar(Date_Object);
		while(PX_VectorPop(&Date_Selectbar->Items));
		for(DateTimeStamp=Start_Time_Stamp;DateTimeStamp<=End_Time_Stamp;++DateTimeStamp)
		{
			Convert_Date_Stamp_to_char(DateTimeStamp,DateTimeString);
			PX_Object_SelectBarAddItem(Date_Object,DateTimeString);
		}

	}
	while(0);//重置selectbar

	PX_ListClear(&query_saved);
	query_saved.mp=PX_NULL;

	PX_ListInitialize(&pRuntime->mp_resources,&query_saved);
	Data_Structure_query(&query_saved,Start_Time_Stamp,End_Time_Stamp,Departure,Arrival);//查询语句


	PX_memset(rec,PX_NULL,sizeof(rec));
	px_list_node *node=query_saved.head;
	PX_Json_Value *tmp,*pExistValue;
	px_int date,cur;
	cur=Start_Time_Stamp;
	while(node)
	{
		tmp=(PX_Json_Value*)node->pdata;
		date=PX_JsonGetObjectValue(tmp,"Date_Stamp")->_number;

		while(date>cur) ++cur;

		if(!rec[cur]) rec[cur-Start_Time_Stamp]=node;

		node=node->pnext;
	}

	update_SearchFlightNo_by_DateInfo(pApp,Start_Time_Stamp);
}

px_void PX_ApplicationOnSearchDateChanged(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
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
	}
	while(0);// 获取开始时间结束时间
	update_SearchFlightNo_by_DateInfo(pApp,Start_Time_Stamp);
}

px_void PX_ApplicationOnSearchFlightNoChanged(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
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
	}
	while(0);// 获取开始时间结束时间

	update_SearchFlightSeatInfo_by_FlightNO(pApp,Start_Time_Stamp);
}

px_void PX_ApplicationOnBookButtonClicked(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	PX_Application *pApp=(PX_Application *)ptr;
	PX_Runtime *pRuntime=&pApp->runtime;

	PX_Object *SearchDateObject=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Date");
	PX_Object *SearchFlightNoObject=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Flight");

	px_char *FlightString=PX_Object_SelectBarGetCurrentText(SearchFlightNoObject);
	if(!PX_strlen(FlightString)) goto __Error;
	//判断是否有选定航班

	px_char *NameString,*PhoneNoString;
	{
		PX_Object *PhoneNoObject,*NameObject;
		NameObject=PX_UIGetObjectByID(&pApp->ui,"MidBottomEditText1");
		PhoneNoObject=PX_UIGetObjectByID(&pApp->ui,"MidBottomEditText2");

		NameString=PX_Object_EditGetText(NameObject);
		PhoneNoString=PX_Object_EditGetText(PhoneNoObject);

		if(!PX_strlen(NameString) || !PX_strlen(PhoneNoString))
			goto __Error;
	}while(0);
	// 判断是否输入姓名和电话号码

	px_char *SeatTypeinString[3];
	px_int SeatTypein[3];
	do
	{

		PX_Object *SearchFlightSeatCapObject[3];
		{
			SearchFlightSeatCapObject[0]=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"RightBorderText1");
			SearchFlightSeatCapObject[1]=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"RightBorderText2");
			SearchFlightSeatCapObject[2]=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"RightBorderText3");
		}while(0);

		px_int SeatCapacity[3];
		{
			px_int i;
			for(i=0;i<3;++i) SeatCapacity[i]=PX_atoi(PX_Object_LabelGetText(SearchFlightSeatCapObject[i]));
		}while(0);

		PX_Object *SearchFlightSeatTypeinObject[3];
		{
			SearchFlightSeatTypeinObject[0]=PX_UIGetObjectByID(&pApp->ui,"RightEditText1");
			SearchFlightSeatTypeinObject[1]=PX_UIGetObjectByID(&pApp->ui,"RightEditText2");
			SearchFlightSeatTypeinObject[2]=PX_UIGetObjectByID(&pApp->ui,"RightEditText3");
		}while(0);

		{
			px_int i;
			for(i=0;i<3;++i)
			{
				SeatTypeinString[i]=PX_Object_EditGetText(SearchFlightSeatTypeinObject[i]);
				SeatTypein[i]=PX_atoi(SeatTypeinString[i]);
			}
		}while(0);

		px_int i;
		for(i=0;i<3;++i)
			if(SeatTypein[i]>SeatCapacity[i]) goto __Error;
		if(!SeatTypein[0]&&!SeatTypein[1]&&!SeatTypein[2]) goto __Error;
	}while(0);
	// 判断预定座位是否大于可用座位且预订了座位
	//
	do
	{
		PX_JsonInitialize(&pRuntime->mp_resources,&tmpJson);
		PX_JsonAddArray(&tmpJson,"","Ticket");
		PX_JsonAddString(&tmpJson,"","Name",NameString);
		PX_JsonAddString(&tmpJson,"","Tele",PhoneNoString);
		px_int i;
		PX_Json_Value __tmp;
		for(i=0;i<3;++i)
		{
			PX_memset(&__tmp,0,sizeof(PX_Json_Value));
			PX_JsonCreateNumberValue(&pRuntime->mp_resources,&__tmp,"",SeatTypein[i]);
			PX_ListPush(&PX_JsonGetValue(&tmpJson,"Ticket")->_array,&__tmp,sizeof(PX_Json_Value));
		}
	}while(0);
	//数据储存

	{
		PX_Object *Reassure_Date_Object;
		PX_Object *Reassure_Flight_Object;
		PX_Object *Reassure_Name_Object;
		PX_Object *Reassure_PhoneNo_Object;
		PX_Object *Reassure_Seat_Object[3];

		Reassure_Date_Object=PX_UIGetObjectByID(&pApp->ui,"ReassureDateContext");
		Reassure_Flight_Object=PX_UIGetObjectByID(&pApp->ui,"ReassureFlightContext");
		PX_Object_LabelSetText(Reassure_Date_Object,PX_Object_SelectBarGetCurrentText(SearchDateObject));
		PX_Object_LabelSetText(Reassure_Flight_Object,PX_Object_SelectBarGetCurrentText(SearchFlightNoObject));

		Reassure_Name_Object=PX_UIGetObjectByID(&pApp->ui,"ReassureNameContext");
		Reassure_PhoneNo_Object=PX_UIGetObjectByID(&pApp->ui,"ReassureTeleContext");
		PX_Object_LabelSetText(Reassure_Name_Object,NameString);
		PX_Object_LabelSetText(Reassure_PhoneNo_Object,PhoneNoString);

		{
			Reassure_Seat_Object[0]=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"ReassureBookSeat1");
			Reassure_Seat_Object[1]=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"ReassureBookSeat2");
			Reassure_Seat_Object[2]=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"ReassureBookSeat3");
		}while(0);
		px_int i;
		for(i=0;i<3;++i) PX_Object_LabelSetText(Reassure_Seat_Object[i],(const px_char*)SeatTypeinString[i]);

		goto_ensure_page(pApp);
		return;
	}while(0);
	//确认页面预渲染与转移


__Error:
	goto_check_fail_page(pApp);
	// 如果以上为否 goto 
}

px_void PX_ApplicationOnEnsureButtonClicked(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
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
	}
	while(0);// 获取开始时间结束时间

	PX_Object *SearchDateObject=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Date");
	PX_Object *SearchFlightNoObject=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Flight");

	PX_Object_SelectBar *SearchFlightNoSelectBar=PX_Object_GetSelectBar(SearchFlightNoObject);
	PX_Object_SelectBar *SearchDateSelectBar=PX_Object_GetSelectBar(SearchDateObject);

	px_char *TimeString=PX_Object_SelectBarGetCurrentText(SearchDateObject);
	px_int Current_Time_Stamp=calc_Date_Stamp(TimeString,"/");

	px_list_node *pNode;
	PX_Json_Value *pData;
	{
		for(pNode=rec[Current_Time_Stamp-Start_Time_Stamp];;pNode=pNode->pnext)
		{
			if(!pNode) break;
			pData=(PX_Json_Value*)pNode->pdata;
			if(PX_JsonGetObjectValue(pData,"Date_Stamp")->_number!=Current_Time_Stamp)
				break;
			if(stringequal(PX_JsonGetObjectValue(pData,"Flight No")->_string.buffer,(px_char*)PX_Object_SelectBarGetCurrentText(SearchFlightNoObject))) break;
		}
	}while(0);

	PX_JsonArrayAddValue(PX_JsonGetObjectValue(pData,"Requesting"),&tmpJson.rootValue);

	PX_JsonGetArrayValue(PX_JsonGetObjectValue(pData,"OccupiedSeats"),0)->_number+=PX_JsonGetNumber(&tmpJson,"Ticket[0]");
	PX_JsonGetArrayValue(PX_JsonGetObjectValue(pData,"OccupiedSeats"),1)->_number+=PX_JsonGetNumber(&tmpJson,"Ticket[1]");
	PX_JsonGetArrayValue(PX_JsonGetObjectValue(pData,"OccupiedSeats"),2)->_number+=PX_JsonGetNumber(&tmpJson,"Ticket[2]");

	Data_Write(pApp);

	goto_successful_page(pApp);
	return;

__Error:
	goto_check_fail_page(pApp);
}

px_void PX_ApplicationPageRefresh(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	PX_Application *pApp=(PX_Application *)ptr;
	PX_Runtime *pRuntime=&pApp->runtime;

	PX_UIFree(&pApp->ui);
	PX_UIInitialize(&pRuntime->mp_game,&pRuntime->mp_ui,&pApp->ui,&pApp->fontmodule);//初始化UI库
	PX_LoadJsonFromFile(&pApp->ui_json,"assets/BootUpElement.json");
	pApp->ui_root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->ui_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);//从JSON中加载UI数据

	PX_LoadJsonFromFile(&pApp->info_json,"assets/InfoElement.json");
	pApp->info_root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->info_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);//从JSON中加载UI数据

	PX_LoadJsonFromFile(&pApp->check_fail_page_json,"assets/CheckFailed.json");
	pApp->check_fail_page_root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->check_fail_page_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);//从JSON中加载UI数据

	PX_LoadJsonFromFile(&pApp->ensure_page_json,"assets/ReassurePageElement.json");
	pApp->ensure_page_root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->ensure_page_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);//从JSON中加载UI数据

	PX_LoadJsonFromFile(&pApp->book_successful_json,"assets/BookSuccessfulElement.json");//加载UI描述的JSON文件
	pApp->book_successful_root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->book_successful_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);//从JSON中加载UI数据

	goto_bootup_page(pApp);
}
