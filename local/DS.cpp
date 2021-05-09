#include "DS.h"
#include "lib.h"
/*
typedef _Balanced_Tree_Node
{
	struct _Balanced_Tree_Node *fa;
	struct _Balanced_Tree_Node *child[2];
	px_int key_val;
	px_void *;
}Balanced_Tree_Node;
*/

struct Segment_Tree_Node
{
	px_int l,r;
	Segment_Tree_Node *child[2];
	std::map<px_int,std::map<px_int,std::vector<PX_Json_Value*> > > root;
};

#define Lc child[0]
#define Rc child[1]

px_bool SegmentTreeNodeRequestMemory(px_memorypool *mp,Segment_Tree_Node *&Node)
{
	Node=(Segment_Tree_Node*)MP_Malloc(mp,sizeof(Segment_Tree_Node));
	PX_memset(Node,0,sizeof(Segment_Tree_Node));
	return Node?PX_TRUE:PX_FALSE;
}

px_void SegmentTreeNodeInitialize(px_int l,px_int r,Segment_Tree_Node* Node)
{
	Node->l=l;
	Node->r=r;
	Node->child[0]=Node->child[1]=PX_NULL;
	Node->root.clear();
}

px_bool SegmentTreeNodeInsert(Segment_Tree_Node* Node,px_int pos,PX_Json_Value* pdata,px_int stringhash[],px_memorypool* mp)
{
	px_int mid=(Node->l+Node->r)>>1;
	Node->root[stringhash[0]][stringhash[1]].push_back(pdata);

	if(Node->l==Node->r) return PX_TRUE;

	if(mid<pos)
	{
		if(Node->Rc==PX_NULL)
		{
			if(!SegmentTreeNodeRequestMemory(mp,Node->Rc)) return PX_FALSE;
			SegmentTreeNodeInitialize(mid+1,Node->r,Node->Rc);
		}
		return SegmentTreeNodeInsert(Node->Rc,pos,pdata,stringhash,mp);
	}
	else
	{
		if(Node->Lc==PX_NULL)
		{
			if(!SegmentTreeNodeRequestMemory(mp,Node->Lc)) return PX_FALSE;
			SegmentTreeNodeInitialize(Node->l,mid,Node->Lc);
		}
		return SegmentTreeNodeInsert(Node->Lc,pos,pdata,stringhash,mp);
	}
}

px_bool SegmentTreeNodeQuery(Segment_Tree_Node* Node,px_int l,px_int r,px_list *psavedata,px_int stringhash[])
{
	if(Node->root[stringhash[0]].find(stringhash[1])==Node->root[stringhash[0]].end()) return PX_FALSE;
	if(Node->l<=l && r<=Node->r)
	{
		std::vector<PX_Json_Value* > vec=Node->root[stringhash[0]][stringhash[1]];
		for(int i=0;i<vec.size();++i)
			if(!PX_ListPush(psavedata,vec[i],sizeof(PX_Json_Value))) return PX_FALSE;
		return PX_TRUE;
	}
	px_int mid=(Node->l+Node->r)>>1;
	px_bool ret=PX_TRUE;
	if(l<=mid&&Node->Lc!=PX_NULL)
		ret&=SegmentTreeNodeQuery(Node->Lc,l,r,psavedata,stringhash);
	if(mid< r&&Node->Rc!=PX_NULL)
		ret&=SegmentTreeNodeQuery(Node->Rc,l,r,psavedata,stringhash);
	return ret;
}

#undef Lc
#undef Rc

struct Segment_Tree
{
	px_memorypool *mp;
	Segment_Tree_Node root;
	px_int L,R;
};

Segment_Tree Tree;

px_bool SegmentTreeInsert(Segment_Tree *_T,const px_int& pos,const px_char S1[],const px_char S2[],PX_Json_Value* pdata)
{
	px_int stringhash[]={myhash(S1),myhash(S2)};
	return SegmentTreeNodeInsert(&_T->root,pos,pdata,stringhash,_T->mp);
}

px_bool SegmentTreeQuery(Segment_Tree *_T,const px_int &l,const px_int &r,px_char S1[],const px_char S2[],px_list* res)
{
	px_int stringhash[]={myhash(S1),myhash(S2)};
	return SegmentTreeNodeQuery(&_T->root,l,r,res,stringhash);
}

px_bool SegmentTreeInitialize(px_memorypool *mp,Segment_Tree *_T,px_int l,px_int r)
{
	memset(_T,0,sizeof(Segment_Tree));
	_T->mp=mp;
	_T->L=l;
	_T->R=r;

	{
		SegmentTreeNodeInitialize(l,r,&_T->root);
	}while(0);

	return PX_TRUE;
}

px_bool calc_Data_Date_stamp(PX_Json_Value *data,px_memorypool *mp)
{
	PX_Json_Value *tmp;
	px_list *List=&data->_array;
	px_list_node *node=List->head;

	PX_Json_Value *Date=PX_NULL,_Date_Stamp;
	px_int Date_Stamp;

	PX_Json_Value *pExistValue;
	while(node!=PX_NULL)
	{
		tmp=(PX_Json_Value*)node->pdata;
		node=node->pnext;
		Date=PX_JsonGetObjectValue(tmp,"Date");
		Date_Stamp=calc_Date_Stamp(Date->_string.buffer,"-");
		pExistValue=PX_JsonGetObjectValue(tmp,"Date_Stamp");
		if(!pExistValue)
		{
			PX_JsonCreateNumberValue(mp,&_Date_Stamp,"Date_Stamp",Date_Stamp);
			PX_ListPush(&tmp->_array,&_Date_Stamp,sizeof(_Date_Stamp));
			//PX_JsonObjectAddValue(tmp,_Date_Stamp);
		}
		else
			if(pExistValue->type!=PX_JSON_VALUE_TYPE_NUMBER)
				return PX_FALSE;
			else
				pExistValue->_number=Date_Stamp;
	}
	return PX_TRUE;
}

PX_Json JsonData;
px_int Start_Time_int=0,End_Time_int=0;

px_bool Data_Structure_load(PX_Application *pApp)
{
	PX_Runtime* pRuntime=&pApp->runtime;
	if(!PX_JsonInitialize(&pRuntime->mp_resources,&JsonData)) return PX_FALSE;//初始化JSON库
	if(!PX_LoadJsonFromFile(&JsonData,"assets/data.json"))return PX_FALSE;//加载UI描述的JSON文件
	return PX_TRUE;
}

px_bool Data_Structure_init(PX_Application *pApp)
{
	PX_Json_Value *Data=NULL,*Start_Time_String=NULL,*End_Time_String=NULL;
	PX_Runtime* pRuntime=&pApp->runtime;
	px_int l=0,r=0;
	Data=PX_JsonGetValue(&JsonData,"data");
	if(!calc_Data_Date_stamp(Data,JsonData.mp)) return PX_FALSE;

	Start_Time_String=PX_JsonGetValue(&JsonData,"start_time");
	Start_Time_int=calc_Date_Stamp(Start_Time_String->_string.buffer,"-");
	PX_JsonAddNumber(&JsonData,"","start_time_stamp",Start_Time_int);
	End_Time_String=PX_JsonGetValue(&JsonData,"end_time");
	End_Time_int=calc_Date_Stamp(End_Time_String->_string.buffer,"-");
	PX_JsonAddNumber(&JsonData,"","end_time_stamp",End_Time_int);
	px_int ret=PX_JsonSortJsonArray(Data,"Date_Stamp",PX_FALSE);
	if(ret) return PX_FALSE;
	if(!PX_WriteJsonToFile(&pRuntime->mp_resources,&JsonData,"assets/data.json")) return PX_FALSE;

	l=1; r=End_Time_int-Start_Time_int+1;
	SegmentTreeInitialize(&pRuntime->mp_resources,&Tree,l,r);

	px_list_node *node;
	PX_Json_Value *tmp,*timestamp,*Departure,*Arrival;
	node=Data->_array.head;
	while(node!=PX_NULL)
	{
		tmp=(PX_Json_Value*)node->pdata;
		timestamp=PX_JsonGetObjectValue(tmp,"Date_Stamp");
		Departure=PX_JsonGetObjectValue(tmp,"Departure");
		Arrival=PX_JsonGetObjectValue(tmp,"Arrival");
		node=node->pnext;
		SegmentTreeInsert(&Tree,(px_int)timestamp->_number-Start_Time_int+1,Departure->_string.buffer,Arrival->_string.buffer,tmp);
	}

	return PX_TRUE;
}

px_bool Data_Structure_query(px_list *result,px_int from,px_int to,px_char Departure[],px_char Arrival[])
{
	return SegmentTreeQuery(&Tree,std::max(1,from-Start_Time_int+1),std::min(End_Time_int-Start_Time_int+1,to-Start_Time_int),Departure,Arrival,result);
}
