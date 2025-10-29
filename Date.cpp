#include "Date.h"

bool isLeap(int y){return (y%4==0&&y%100!=0)||(y%400==0);}
int daysInMonth(int m,int y){
    if(m==4||m==6||m==9||m==11)return 30;
    if(m==2)return isLeap(y)?29:28;
    return 31;
}

Date::Date(int dd,int mm,int yy):d(dd),m(mm),y(yy){}
Date Date::today(){time_t now=time(0);tm* ltm=localtime(&now);return Date(ltm->tm_mday,1+ltm->tm_mon,1900+ltm->tm_year);}
bool Date::isCalendarValid() const{
    if(y<1900||m<1||m>12||d<1)return false;
    if(d>daysInMonth(m,y))return false;
    return true;
}
bool Date::isValidNotAfterToday() const{
    if(!isCalendarValid())return false;
    Date cur=today();
    if(y>cur.y)return false;
    if(y==cur.y&&m>cur.m)return false;
    if(y==cur.y&&m==cur.m&&d>cur.d)return false;
    return true;
}
bool Date::isBetweenInclusive(const Date& d1,const Date& d2) const{
    if(!isCalendarValid())return false;
    if(y<d1.y||(y==d1.y&&m<d1.m)||(y==d1.y&&m==d1.m&&d<d1.d))return false;
    if(y>d2.y||(y==d2.y&&m>d2.m)||(y==d2.y&&m==d2.m&&d>d2.d))return false;
    return true;
}
string Date::str() const{
    stringstream ss; ss<<setfill('0')<<setw(2)<<d<<"/"<<setw(2)<<m<<"/"<<y; return ss.str();
}
int cmpDate(const Date& a,const Date& b){
    if(a.y!=b.y)return a.y<b.y?-1:1;
    if(a.m!=b.m)return a.m<b.m?-1:1;
    if(a.d!=b.d)return a.d<b.d?-1:1;
    return 0;
}
Date addMonths(const Date& base,int months){
    int y=base.y,m=base.m+months,d=base.d;
    if(m>0){y+=(m-1)/12;m=(m-1)%12+1;}
    else{int dec=(-m)/12+1;y-=dec;m+=12*dec;}
    int dim=daysInMonth(m,y);
    if(d>dim)d=dim;
    return Date(d,m,y);
}
Date Date::input(){
    int dd,mm,yy;Date t;Date cur=today();
    cout<<"Hom nay: "<<cur.str()<<"\nNhap ngay (dd mm yyyy): ";
    while(true){
        if(!(cin>>dd>>mm>>yy)){cout<<"Nhap lai: ";cin.clear();cin.ignore(999,'\n');continue;}
        t=Date(dd,mm,yy);
        if(!t.isValidNotAfterToday()){cout<<"Ngay khong hop le. Nhap lai: ";cin.clear();cin.ignore(999,'\n');continue;}
        break;
    }
    cin.ignore(999,'\n'); return t;
}
Date Date::inputFutureWithinMonths(int maxMonths){
    int dd,mm,yy;Date t;Date cur=today();Date limit=addMonths(cur,maxMonths);
    cout<<"Hom nay: "<<cur.str()<<"\nChi duoc dat den: "<<limit.str()<<"\nNhap ngay hen (dd mm yyyy): ";
    while(true){
        if(!(cin>>dd>>mm>>yy)){cout<<"Nhap lai: ";cin.clear();cin.ignore(999,'\n');continue;}
        t=Date(dd,mm,yy);
        if(!t.isBetweenInclusive(cur,limit)){cout<<"Ngay hen khong hop le. Nhap lai: ";cin.clear();cin.ignore(999,'\n');continue;}
        break;
    }
    cin.ignore(999,'\n'); return t;
}
