#include "lib.h"

px_int calc_Date_Stamp_by_int(px_int yy,px_int mm,px_int dd)
{
	const px_int tot_day[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	px_int ret=0;
	ret+=dd;
	for(int i=1;i<mm;++i) ret+=tot_day[i];
	ret+=((mm>2)&&((yy&3)==0));
	--yy;
	ret+=yy*365+(yy/4)-(yy/100)+(yy/400);
	return ret;
}

px_int calc_Date_Stamp(px_char s[])
{
	px_int yy=0,mm=0,dd=0;
	while(*s!='-')
	{
		yy=yy*10+*s-'0';
		++s;
	}
	++s;
	while(*s!='-')
	{
		mm=mm*10+*s-'0';
		++s;
	}
	++s;
	while(*s)
	{
		dd=dd*10+*s-'0';
		++s;
	}
	return calc_Date_Stamp_by_int(yy,mm,dd);
}

px_int calc_Year_tot_Stamp(px_int yy)
{
	return (yy*365)+(yy/4)-(yy/100)+(yy/400);
}

px_void Convert_Date_Stamp_to_char(px_int stamp,px_char ret[])
{
	const px_int tot_day[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	px_int yy,mm,dd;
	px_int l=2000,r=2101;
	px_int mid,tmp;
	while(l!=r)
	{
		mid=(l+r)>>1;
		tmp=calc_Year_tot_Stamp(mid);
		tmp>=stamp?r=mid:l=mid+1;
	}
	yy=l;
	stamp-=calc_Year_tot_Stamp(yy-1);
	for(mm=1;;mm++)
	{
		px_int cur=tot_day[mm]+((mm==2)&&((yy%4==0 && yy%100!=0)||yy%400==0));
		if(stamp<=cur) break;
		stamp-=cur;
	}
	dd=stamp;
	memset(ret,0,sizeof(px_char)*20);
	std::sprintf(ret,"%d/%02d/%02d",yy,mm,dd);
}

px_int myhash(const px_char s[])
{
#define MOD 1000000007
#define factor 1313
	px_int ret,len=PX_strlen(s);
	px_int i;
	for(i=0,ret=0;i<len;++i)
		ret=((long long)ret*1313+s[i]-'A')%MOD;
	return ret;
#undef factor
#undef MOD
}
