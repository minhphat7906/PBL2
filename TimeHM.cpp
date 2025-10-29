#include "TimeHM.h"
TimeHM::TimeHM(int h,int m):hour(h),minute(m){}
TimeHM TimeHM::now(){time_t t=time(0);tm*l=localtime(&t);return TimeHM(l->tm_hour,l->tm_min);}
bool TimeHM::isValid() const{return hour>=0&&hour<=23&&minute>=0&&minute<=59;}
string TimeHM::str() const{stringstream ss;ss<<setfill('0')<<setw(2)<<hour<<":"<<setw(2)<<minute;return ss.str();}
int cmpTime(const TimeHM&a,const TimeHM&b){if(a.hour!=b.hour)return a.hour<b.hour?-1:1;if(a.minute!=b.minute)return a.minute<b.minute?-1:1;return 0;}
TimeHM TimeHM::inputForDate(const Date& d){
    TimeHM t;Date today=Date::today();TimeHM cur=TimeHM::now();
    cout<<"Nhap gio hen (hh mm) 24h: ";
    while(true){
        if(!(cin>>t.hour>>t.minute)||!t.isValid()){cout<<"Gio/phut khong hop le. Nhap lai: ";cin.clear();cin.ignore(999,'\n');continue;}
        if(cmpDate(d,today)==0&&(t.hour<cur.hour||(t.hour==cur.hour&&t.minute<cur.minute))){cout<<"Khong the dat gio qua khu. Nhap lai: ";cin.clear();cin.ignore(999,'\n');continue;}
        break;
    }
    cin.ignore(999,'\n');return t;
}
