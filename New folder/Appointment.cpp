#include "Appointment.h"
Appointment::Appointment(int i,int pid,Date d,TimeHM t,string n):id(i),patientId(pid),date(d),time(t),note(n){}
void Appointment::input(int newId,int pid,const string& patientName){
    id = newId;
    patientId = pid;
    cout << "\n=== DAT LICH KHAM MOI ===\n";
    if (!patientName.empty())
        cout << "Benh nhan: " << patientName << " (ID: " << pid << ")\n";
    else
        cout << "Benh nhan ID: " << pid << "\n";

    date = Date::inputFutureWithinMonths(3);
    time = TimeHM::inputForDate(date);
    cout << "Ghi chu: ";
    getline(cin >> ws, note);
}

// Giữ lại bản cũ cho tương thích
void Appointment::input(int newId,int pid){
    input(newId, pid, "");
}

vector<string> Appointment::toRow()const{
    vector<string>cells;stringstream ss;string noteCut=note.size()>30?note.substr(0,30):note;
    ss<<id;cells.push_back(ss.str());ss.str("");ss.clear();
    ss<<patientId;cells.push_back(ss.str());ss.str("");ss.clear();
    cells.push_back(date.str());cells.push_back(time.str());cells.push_back(noteCut);
    return cells;
}
int Appointment::getId()const{return id;}int Appointment::getPid()const{return patientId;}
Date Appointment::getDate()const{return date;}TimeHM Appointment::getTime()const{return time;}
bool Appointment::operator<(const Appointment&o)const{
    int dc=cmpDate(date,o.date);if(dc!=0)return dc<0;return cmpTime(time,o.time)<0;
}
