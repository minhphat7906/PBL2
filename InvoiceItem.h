#ifndef INVOICEITEM_H
#define INVOICEITEM_H
#include <bits/stdc++.h>
using namespace std;
class InvoiceItem{
    string name;int qty;double price;
public:
    InvoiceItem(string n="",int q=1,double p=0);
    void input();
    double total()const;
    vector<string> toRow() const;
};
#endif
