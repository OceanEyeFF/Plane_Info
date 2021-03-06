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
	PX_Object *SearchDateObject=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Date");
	PX_Object *SearchFlightNoObject=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Flight");

	px_char *FlightString=PX_Object_SelectBarGetCurrentText(SearchFlightNoObject);
	if(!PX_strlen(FlightString)) goto __Error;
	//???????????????????????????
	//
	goto_info_page(pApp);
	return;
__Error:
	goto_check_fail_page(pApp);
	// ?????????????????? goto 
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
			if(PX_JsonGetObjectValue(pData,"Date_Stamp")->_number!=Current_Time_Stamp) break;
			PX_Object_SelectBarAddItem(SearchFlightNoObject,PX_JsonGetObjectValue(pData,"Flight No")->_string.buffer);
		}
	}
	while(0);//??????selectbar

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
	while(0);// ??????????????????????????????

	if(Start_Time_Stamp>End_Time_Stamp) return;

	px_char *Departure,*Arrival;
	do
	{
		Departure=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"departure"));
		Arrival=PX_Object_SelectBarGetCurrentText(PX_UIGetObjectByID(&pApp->ui,(const px_char*)"arrival"));
	}
	while(0);//???????????????????????????

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
	while(0);//??????selectbar

	PX_ListClear(&query_saved);
	query_saved.mp=PX_NULL;

	PX_ListInitialize(&pRuntime->mp_resources,&query_saved);
	Data_Structure_query(&query_saved,Start_Time_Stamp,End_Time_Stamp,Departure,Arrival);//????????????


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

		if(!rec[cur-Start_Time_Stamp]) rec[cur-Start_Time_Stamp]=node;

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
	while(0);// ??????????????????????????????
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
	while(0);// ??????????????????????????????

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
	//???????????????????????????

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
	// ???????????????????????????????????????

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
//		for(i=0;i<3;++i)
//			if(SeatTypein[i]>SeatCapacity[i]) goto __Error;
		if(!SeatTypein[0]&&!SeatTypein[1]&&!SeatTypein[2]) goto __Error;
	}while(0);
	// ???????????????????????????
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
	//????????????

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
	//??????????????????????????????


__Error:
	goto_check_fail_page(pApp);
	// ?????????????????? goto 
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
	while(0);// ??????????????????????????????

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

	px_char *SeatTypeinString[3];
	px_int SeatTypein[3];
	px_bool flag=PX_TRUE;
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

		do
		{
			px_int i;
			for(i=0;i<3;++i)
			{
				SeatTypeinString[i]=PX_Object_EditGetText(SearchFlightSeatTypeinObject[i]);
				SeatTypein[i]=PX_atoi(SeatTypeinString[i]);
			}
		}while(0);

		px_int i;
		for(i=0;i<3;++i) if(SeatTypein[i]>SeatCapacity[i]) flag=PX_FALSE;
	}while(0);
	PX_JsonArrayAddValue(PX_JsonGetObjectValue(pData,flag?"Accepted":"Requesting"),&tmpJson.rootValue);

	PX_JsonGetArrayValue(PX_JsonGetObjectValue(pData,"OccupiedSeats"),0)->_number+=flag?PX_JsonGetNumber(&tmpJson,"Ticket[0]"):0;
	PX_JsonGetArrayValue(PX_JsonGetObjectValue(pData,"OccupiedSeats"),1)->_number+=flag?PX_JsonGetNumber(&tmpJson,"Ticket[1]"):0;
	PX_JsonGetArrayValue(PX_JsonGetObjectValue(pData,"OccupiedSeats"),2)->_number+=flag?PX_JsonGetNumber(&tmpJson,"Ticket[2]"):0;

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
	PX_UIInitialize(&pRuntime->mp_game,&pRuntime->mp_ui,&pApp->ui,&pApp->fontmodule);//?????????UI???
	pApp->ui_root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->ui_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);//???JSON?????????UI??????

	pApp->info_root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->info_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);//???JSON?????????UI??????

	pApp->check_fail_page_root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->check_fail_page_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);//???JSON?????????UI??????

	pApp->ensure_page_root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->ensure_page_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);//???JSON?????????UI??????

	pApp->book_successful_root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->book_successful_json.rootValue,pRuntime->surface_width,pRuntime->surface_height);//???JSON?????????UI??????

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

	goto_bootup_page(pApp);
}

px_void PX_ApplicationOnInfoGrepButtonClicked(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	PX_Application *pApp=(PX_Application *)ptr;
	PX_Runtime *pRuntime=&pApp->runtime;
	//?????????
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
	while(0);// ??????????????????????????????

	PX_Object *SearchDateObject=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Date");
	PX_Object *SearchFlightNoObject=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Flight");

	PX_Object_SelectBar *SearchFlightNoSelectBar=PX_Object_GetSelectBar(SearchFlightNoObject);
	PX_Object_SelectBar *SearchDateSelectBar=PX_Object_GetSelectBar(SearchDateObject);

	px_char *TimeString=PX_Object_SelectBarGetCurrentText(SearchDateObject);
	px_int Current_Time_Stamp=calc_Date_Stamp(TimeString,"/");

	px_list_node *pNode;
	PX_Json_Value *pData;
	do
	{
		for(pNode=rec[Current_Time_Stamp-Start_Time_Stamp];pNode;pNode=pNode->pnext)
		{
			pData=(PX_Json_Value*)pNode->pdata;
			if(PX_JsonGetObjectValue(pData,"Date_Stamp")->_number!=Current_Time_Stamp)
				break;
			if(stringequal(PX_JsonGetObjectValue(pData,"Flight No")->_string.buffer,(px_char*)PX_Object_SelectBarGetCurrentText(SearchFlightNoObject))) break;
		}
	}while(0);
	//????????????????????????PX_Json_Value
	px_char* __name,*__tele;
	px_char* Name,*Tele;
	PX_Object *NameObject,*TeleObject;
	PX_Object *SeatObject[3];

	do
	{
		NameObject=PX_UIGetObjectByID(&pApp->ui,"LeftMostEditText1");
		TeleObject=PX_UIGetObjectByID(&pApp->ui,"LeftMostEditText2");
		__name=PX_Object_EditGetText(NameObject);
		__tele=PX_Object_EditGetText(TeleObject);
	}
	while(0);

	if(!PX_strlen(__name)||!PX_strlen(__tele)) return;

	do
	{
		PX_Json_Value *tmp;
		for(pNode=PX_JsonGetObjectValue(pData,"Accepted")->_array.head;pNode;pNode=pNode->pnext)
		{
			tmp=pNode->pdata;
			if(stringequal(PX_JsonGetObjectValue(tmp,"Name")->_string.buffer,__name)&&stringequal(PX_JsonGetObjectValue(tmp,"Tele")->_string.buffer,__tele)) break;
		}
		if(pNode)
		{
			SeatObject[0]=PX_UIGetObjectByID(&pApp->ui,"InfoLeftContentText1");
			SeatObject[1]=PX_UIGetObjectByID(&pApp->ui,"InfoLeftContentText2");
			SeatObject[2]=PX_UIGetObjectByID(&pApp->ui,"InfoLeftContentText3");

			px_int i;
			px_char tmprec[3][4];
			PX_Json_Value *__tmpJsonValue;
			__tmpJsonValue=PX_JsonGetObjectValue(tmp,"Ticket");
			for(i=0;i<3;++i)
			{
				PX_itoa(PX_JsonGetArrayValue(__tmpJsonValue,i)->_number,tmprec[i],4,10);
				PX_Object_LabelSetText(SeatObject[i],tmprec[i]);
			}
		}
		else
		{
			px_int i;
			SeatObject[0]=PX_UIGetObjectByID(&pApp->ui,"InfoLeftContentText1");
			SeatObject[1]=PX_UIGetObjectByID(&pApp->ui,"InfoLeftContentText2");
			SeatObject[2]=PX_UIGetObjectByID(&pApp->ui,"InfoLeftContentText3");
			for(i=0;i<3;++i) PX_Object_LabelSetText(SeatObject[i],"");
		}
	}
	while(0);//???????????????????????????????????????

	do
	{
		PX_Json_Value *tmp;
		for(pNode=PX_JsonGetObjectValue(pData,"Requesting")->_array.head;pNode;pNode=pNode->pnext)
		{
			tmp=pNode->pdata;
			if(stringequal(PX_JsonGetObjectValue(tmp,"Name")->_string.buffer,__name)&&stringequal(PX_JsonGetObjectValue(tmp,"Tele")->_string.buffer,__tele)) break;
		}
		if(pNode)
		{
			SeatObject[0]=PX_UIGetObjectByID(&pApp->ui,"InfoRightContentText1");
			SeatObject[1]=PX_UIGetObjectByID(&pApp->ui,"InfoRightContentText2");
			SeatObject[2]=PX_UIGetObjectByID(&pApp->ui,"InfoRightContentText3");

			px_int i;
			px_char tmprec[3][4];
			PX_Json_Value *__tmpJsonValue;
			__tmpJsonValue=PX_JsonGetObjectValue(tmp,"Ticket");
			for(i=0;i<3;++i)
			{
				PX_itoa(PX_JsonGetArrayValue(__tmpJsonValue,i)->_number,tmprec[i],4,10);
				PX_Object_LabelSetText(SeatObject[i],tmprec[i]);
			}
		}
		else
		{
			px_int i;
			SeatObject[0]=PX_UIGetObjectByID(&pApp->ui,"InfoRightContentText1");
			SeatObject[1]=PX_UIGetObjectByID(&pApp->ui,"InfoRightContentText2");
			SeatObject[2]=PX_UIGetObjectByID(&pApp->ui,"InfoRightContentText3");
			for(i=0;i<3;++i) PX_Object_LabelSetText(SeatObject[i],"");
		}
	}
	while(0);//????????????????????????????????????
}

px_void PX_ApplicationOnDeleteAcceptButtonClicked(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	PX_Application *pApp=(PX_Application *)ptr;
	PX_Runtime *pRuntime=&pApp->runtime;
	//?????????
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
	while(0);// ??????????????????????????????

	PX_Object *SearchDateObject=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Date");
	PX_Object *SearchFlightNoObject=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Flight");

	PX_Object_SelectBar *SearchFlightNoSelectBar=PX_Object_GetSelectBar(SearchFlightNoObject);
	PX_Object_SelectBar *SearchDateSelectBar=PX_Object_GetSelectBar(SearchDateObject);

	px_char *TimeString=PX_Object_SelectBarGetCurrentText(SearchDateObject);
	px_int Current_Time_Stamp=calc_Date_Stamp(TimeString,"/");

	px_list_node *pNode;
	PX_Json_Value *pData;
	do
	{
		for(pNode=rec[Current_Time_Stamp-Start_Time_Stamp];pNode;pNode=pNode->pnext)
		{
			pData=(PX_Json_Value*)pNode->pdata;
			if(PX_JsonGetObjectValue(pData,"Date_Stamp")->_number!=Current_Time_Stamp)
				break;
			if(stringequal(PX_JsonGetObjectValue(pData,"Flight No")->_string.buffer,(px_char*)PX_Object_SelectBarGetCurrentText(SearchFlightNoObject))) break;
		}
	}while(0);
	//????????????????????????PX_Json_Value
	px_char* __name,*__tele;
	px_char* Name,*Tele;
	PX_Object *NameObject,*TeleObject;
	PX_Object *SeatObject[3];

	do
	{
		px_int _;
		NameObject=PX_UIGetObjectByID(&pApp->ui,"LeftMostEditText1");
		TeleObject=PX_UIGetObjectByID(&pApp->ui,"LeftMostEditText2");
		__name=PX_Object_EditGetText(NameObject);
		__tele=PX_Object_EditGetText(TeleObject);
		if(!PX_strlen(__name)||!PX_strlen(__tele)) return;
		SeatObject[0]=PX_UIGetObjectByID(&pApp->ui,"InfoLeftContentText1");
		SeatObject[1]=PX_UIGetObjectByID(&pApp->ui,"InfoLeftContentText2");
		SeatObject[2]=PX_UIGetObjectByID(&pApp->ui,"InfoLeftContentText3");
		for(_=0;_<3;++_) if(!PX_strlen(PX_Object_LabelGetText(SeatObject[_]))) return;
	}
	while(0);

	do
	{
		PX_Json_Value *tmp;
		for(pNode=PX_JsonGetObjectValue(pData,"Accepted")->_array.head;pNode;pNode=pNode->pnext)
		{
			tmp=pNode->pdata;
			if(stringequal(PX_JsonGetObjectValue(tmp,"Name")->_string.buffer,__name)&&stringequal(PX_JsonGetObjectValue(tmp,"Tele")->_string.buffer,__tele)) break;
		}
		if(pNode)
		{
			px_int i;
			PX_Json_Value *__tmpJsonValue;
			__tmpJsonValue=PX_JsonGetObjectValue(tmp,"Ticket");
			for(i=0;i<3;++i)
				PX_JsonGetArrayValue(PX_JsonGetObjectValue(pData,"OccupiedSeats"),i)->_number-=PX_JsonGetArrayValue(__tmpJsonValue,i)->_number;
			PX_JsonFreeValue(PX_NULL,tmp);
			PX_ListPop(&PX_JsonGetObjectValue(pData,"Accepted")->_array,pNode);
		}
	}
	while(0);//???????????????

	do
	{
		px_int Seat_Capacity[3];
		px_int _;
		for(_=0;_<3;++_)
			Seat_Capacity[_]=PX_JsonGetArrayValue(PX_JsonGetObjectValue(pData,"Seats"),_)->_number-PX_JsonGetArrayValue(PX_JsonGetObjectValue(pData,"OccupiedSeats"),_)->_number;
		PX_Json_Value *tmp;
		PX_Json_Value *__tmpJsonValue;
		for(pNode=PX_JsonGetObjectValue(pData,"Requesting")->_array.head;pNode;pNode=pNode->pnext)
		{
			tmp=pNode->pdata;
			__tmpJsonValue=PX_JsonGetObjectValue(tmp,"Ticket");
			for(_=0;_<3;++_)
				if(PX_JsonGetArrayValue(__tmpJsonValue,_)->_number>Seat_Capacity[_]) break;
			if(_==3) break;
		}

		if(pNode)
		{
			PX_JsonArrayAddValue(PX_JsonGetObjectValue(pData,"Accepted"),tmp);
			for(_=0;_<3;++_)
				PX_JsonGetArrayValue(PX_JsonGetObjectValue(pData,"OccupiedSeats"),_)->_number+=PX_JsonGetArrayValue(__tmpJsonValue,_)->_number;

			PX_ListPop(&PX_JsonGetObjectValue(pData,"Requesting")->_array,pNode);
		}
	}
	while(0);//????????????????????????????????????

	Data_Write(pApp);

	goto_successful_page(pApp);
}

px_void PX_ApplicationOnDeleteRequestingButtonClicked(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	PX_Application *pApp=(PX_Application *)ptr;
	PX_Runtime *pRuntime=&pApp->runtime;
	//?????????
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
	while(0);// ??????????????????????????????

	PX_Object *SearchDateObject=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Date");
	PX_Object *SearchFlightNoObject=PX_UIGetObjectByID(&pApp->ui,(const px_char*)"Flight");

	PX_Object_SelectBar *SearchFlightNoSelectBar=PX_Object_GetSelectBar(SearchFlightNoObject);
	PX_Object_SelectBar *SearchDateSelectBar=PX_Object_GetSelectBar(SearchDateObject);

	px_char *TimeString=PX_Object_SelectBarGetCurrentText(SearchDateObject);
	px_int Current_Time_Stamp=calc_Date_Stamp(TimeString,"/");

	px_list_node *pNode;
	PX_Json_Value *pData;
	do
	{
		for(pNode=rec[Current_Time_Stamp-Start_Time_Stamp];pNode;pNode=pNode->pnext)
		{
			pData=(PX_Json_Value*)pNode->pdata;
			if(PX_JsonGetObjectValue(pData,"Date_Stamp")->_number!=Current_Time_Stamp)
				break;
			if(stringequal(PX_JsonGetObjectValue(pData,"Flight No")->_string.buffer,(px_char*)PX_Object_SelectBarGetCurrentText(SearchFlightNoObject))) break;
		}
	}while(0);
	//????????????????????????PX_Json_Value
	px_char* __name,*__tele;
	px_char* Name,*Tele;
	PX_Object *NameObject,*TeleObject;
	PX_Object *SeatObject[3];
	px_int _;

	do
	{
		NameObject=PX_UIGetObjectByID(&pApp->ui,"LeftMostEditText1");
		TeleObject=PX_UIGetObjectByID(&pApp->ui,"LeftMostEditText2");
		__name=PX_Object_EditGetText(NameObject);
		__tele=PX_Object_EditGetText(TeleObject);
	}
	while(0);

	if(!PX_strlen(__name)||!PX_strlen(__tele)) return;

	SeatObject[0]=PX_UIGetObjectByID(&pApp->ui,"InfoRightContentText1");
	SeatObject[1]=PX_UIGetObjectByID(&pApp->ui,"InfoRightContentText2");
	SeatObject[2]=PX_UIGetObjectByID(&pApp->ui,"InfoRightContentText3");

	for(_=0;_<3;++_) if(!PX_strlen(PX_Object_LabelGetText(SeatObject[_]))) return;

	do
	{
		PX_Json_Value *tmp;
		for(pNode=PX_JsonGetObjectValue(pData,"Requesting")->_array.head;pNode;pNode=pNode->pnext)
		{
			tmp=pNode->pdata;
			if(stringequal(PX_JsonGetObjectValue(tmp,"Name")->_string.buffer,__name)&&stringequal(PX_JsonGetObjectValue(tmp,"Tele")->_string.buffer,__tele)) break;
		}
		if(pNode)
		{
			PX_Json_Value *__tmpJsonValue;
			__tmpJsonValue=PX_JsonGetObjectValue(tmp,"Ticket");
			PX_JsonFreeValue(PX_NULL,tmp);
			PX_ListPop(&PX_JsonGetObjectValue(pData,"Requesting")->_array,pNode);
		}
	}
	while(0);//???????????????

	Data_Write(pApp);

	goto_successful_page(pApp);
}
