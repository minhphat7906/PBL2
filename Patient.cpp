#include "Patient.h"
Patient::Patient(int i,string n,string p,string dis,Date d,Date lv):id(i),name(n),phone(p),disease(dis),dob(d),lastVisit(lv){}
void Patient::input(int newId){
    id=newId;
    cout<<"Ho ten: ";
    while(true){string raw;getline(cin>>ws,raw);bool ok;string nm=normalizeName(raw,ok);if(ok){name=nm;break;}cout<<"Khong hop le. Nhap lai: "; }
    cout<<"So dien thoai: ";
    while(true){string raw;getline(cin,raw);if(validatePhoneStrict(raw)){phone=raw;break;}cout<<"SDT khong hop le. Nhap lai: "; }
    cout<<"Benh ly: ";getline(cin,disease);
    cout<<"Ngay sinh:\n";dob=Date::input();
    cout<<"Ngay kham gan nhat:\n";while(true){Date lv=Date::input();if(lv.isBetweenInclusive(dob,Date::today())){lastVisit=lv;break;}cout<<"Ngay kham khong hop le.\n";}
}
vector<string> Patient::toRow() const{
    vector<string>cells;stringstream ss;ss<<id;cells.push_back(ss.str());
    cells.push_back(name);cells.push_back(phone);cells.push_back(disease);
    cells.push_back(dob.str());cells.push_back(lastVisit.str());return cells;
}
int Patient::getId()const{return id;}string Patient::getName()const{return name;}string Patient::getPhone()const{return phone;}
string Patient::getDisease()const{return disease;}Date Patient::getDob()const{return dob;}Date Patient::getLastVisit()const{return lastVisit;}
void Patient::setName(const string&n){name=n;}void Patient::setPhone(const string&p){phone=p;}void Patient::setDisease(const string&d){disease=d;}
