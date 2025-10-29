#include "Invoice.h"
Invoice::Invoice(int i,int pid):id(i),patientId(pid){}
void Invoice::input(int newId,int pid){
    id=newId;patientId=pid;cout<<"Nhap ngay lap hoa don:\n";date=Date::input();
    while(true){InvoiceItem it;cout<<"\n--- Nhap muc hoa don ---\n";it.input();items.push_back(it);
        char cont;cout<<"Them muc khac? (y/n): ";if(!(cin>>cont)){cin.clear();cin.ignore(999,'\n');continue;}
        cin.ignore(999,'\n');if(cont=='n'||cont=='N')break;}
}
double Invoice::total()const{double s=0;for(auto&i:items)s+=i.total();return s;}
void Invoice::display()const{
    cout<<"\n=== HOA DON #"<<id<<" cho BN "<<patientId<<" ===\nNgay: "<<date.str()<<"\n";
    vector<int>W={20,6,12,12};vector<string>H={"Dich vu","SL","Don gia","Thanh tien"};
    drawSep(W);drawRow(W,H);drawSep(W);
    for(auto&i:items)drawRow(W,i.toRow());
    drawSep(W);stringstream ss;ss<<fixed<<setprecision(2)<<total()<<" VND";cout<<"Tong: "<<ss.str()<<"\n";
}
int Invoice::getId()const{return id;}int Invoice::getPid()const{return patientId;}Date Invoice::getDate()const{return date;}
