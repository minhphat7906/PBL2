#ifndef DATE_H
#define DATE_H
#include <bits/stdc++.h>
using namespace std;

struct Date{
    int d,m,y;
    Date(int dd=1,int mm=1,int yy=2000);
    static Date today();
    bool isCalendarValid() const;
    bool isValidNotAfterToday() const;
    bool isBetweenInclusive(const Date& d1,const Date& d2) const;
    string str() const;
    static Date input();
    static Date inputFutureWithinMonths(int maxMonths);
};

bool isLeap(int y);
int daysInMonth(int m,int y);
int cmpDate(const Date& a,const Date& b);
Date addMonths(const Date& base,int months);

#endif
