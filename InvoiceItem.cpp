#include "InvoiceItem.h"
InvoiceItem::InvoiceItem(string n,int q,double p):name(n),qty(q),price(p){}
void InvoiceItem::input(){
    cout<<"Ten dich vu: ";getline(cin>>ws,name);
    cout<<"So luong: ";while(!(cin>>qty)||qty<=0){cout<<"Nhap lai: ";cin.clear();cin.ignore(999,'\n');}
    cout<<"Don gia: ";while(!(cin>>price)||price<0){cout<<"Nhap lai: ";cin.clear();cin.ignore(999,'\n');}
    cin.ignore(999,'\n');
}
double InvoiceItem::total()const{return qty*price;}
vector<string> InvoiceItem::toRow() const{
    stringstream ss;vector<string>cells;string nm=name.size()>20?name.substr(0,20):name;cells.push_back(nm);
    ss<<qty;cells.push_back(ss.str());ss.str("");ss.clear();
    ss<<fixed<<setprecision(2)<<price;cells.push_back(ss.str());ss.str("");ss.clear();
    ss<<fixed<<setprecision(2)<<total();cells.push_back(ss.str());
    return cells;
}
