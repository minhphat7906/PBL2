#ifndef TIMEHM_H
#define TIMEHM_H
#include "Date.h"
using namespace std;

struct TimeHM{
    int hour,minute;
    TimeHM(int h=0,int m=0);
    static TimeHM now();
    bool isValid() const;
    string str() const;
    static TimeHM inputForDate(const Date& d);
};
int cmpTime(const TimeHM&a,const TimeHM&b);
#endif
