#ifndef APPOINTMENT_H
#define APPOINTMENT_H
#include "Date.h"
#include "TimeHM.h"
#include "Utils.h"
class Appointment{
    int id,patientId;Date date;TimeHM time;string note;
public:
    Appointment(int i=0,int pid=0,Date d=Date(),TimeHM t=TimeHM(),string n="");
    void input(int newId,int pid,const string& patientName);
    void input(int newId,int pid); // giữ bản cũ để tương thích
    vector<string> toRow()const;
    int getId()const;int getPid()const;Date getDate()const;TimeHM getTime()const;
    bool operator<(const Appointment&o)const;
};
#endif
