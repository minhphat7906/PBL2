#ifndef PATIENT_H
#define PATIENT_H
#include "Date.h"
#include "Utils.h"
class Patient{
    int id; string name,phone,disease; Date dob,lastVisit;
public:
    Patient(int i=0,string n="",string p="",string dis="",Date d=Date(),Date lv=Date());
    void input(int newId);
    vector<string> toRow() const;
    int getId()const;string getName()const;string getPhone()const;string getDisease()const;
    Date getDob()const;Date getLastVisit()const;
    void setName(const string&n);void setPhone(const string&p);void setDisease(const string&d);
};
#endif
