#ifndef INVOICE_H
#define INVOICE_H
#include "InvoiceItem.h"
#include "Date.h"
#include "Utils.h"
class Invoice{
    int id,patientId;Date date;vector<InvoiceItem>items;
public:
    Invoice(int i=0,int pid=0);
    void input(int newId,int pid);
    double total()const;
    void display()const;
    int getId()const;int getPid()const;Date getDate()const;
};
#endif
