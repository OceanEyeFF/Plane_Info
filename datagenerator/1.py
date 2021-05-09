import sys
import io
import json
import random
import time
from datetime import datetime

def strTimeProp(start, end, prop, frmt):
    stime = time.mktime(time.strptime(start, frmt))
    etime = time.mktime(time.strptime(end, frmt))
    ptime = stime + prop * (etime - stime)
    return int(ptime)
def randomTimestamp(start, end, frmt='%Y-%m-%d'):
    return strTimeProp(start, end, random.random(), frmt)
def randomDate(start, end, frmt='%Y-%m-%d'):
    return time.strftime(frmt, time.localtime(strTimeProp(start, end, random.random(), frmt)))
def randomTimestampList(start, end, n, frmt='%Y-%m-%d'):
    return [randomTimestamp(start, end, frmt) for _ in xrange(n)]
def randomDateList(start, end, n, frmt='%Y-%m-%d'):
    return [randomDate(start, end, frmt) for _ in xrange(n)]

start_time="2020-01-01"
end_time  ="2020-01-06"

if __name__=="__main__":
    file=open("base.json","r")
    source=json.load(file)
    data=[]
    for i in range(30):
        #Dep=source['AirportList'][random.randint(0,len(source['AirportList']))]
        Dep=random.choice(source['AirportList'])
        Arv=random.choice(source['AirportList'])
        while Arv==Dep:
            Arv=random.choice(source['AirportList'])
        Airline=random.choice(source['Airlines'])
        TimeStamp=randomTimestamp(start_time,end_time)
        Date=time.strftime('%Y-%m-%d', time.localtime(TimeStamp))
        No="".join(random.sample('0123456789',4))
        tmp={
                'Departure':Dep,
                'Arrival':Arv,
                'Flight No':Airline+No,
                'Date':Date,
                'Seats':[10,20,100],
                'OccupiedSeats':[0,0,0],
                'Requesting':[
                        {
                    'Level':-1,
                    'Name':r"",
                    'Tele':r"",
                    },

                    ]
        }
        data.append(tmp)
    output={
            "start_time":"2020-01-01",
            "end_time":"2021-12-31",
            "data":data
            }
    file=open("../data.json","w")
    json.dump(output,file)
