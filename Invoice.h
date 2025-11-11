#ifndef INVOICE_H
#define INVOICE_H
#include <string>
#include <iostream>
using namespace std;

class Invoice {
    static int NEXT_ID;
    int id;
    int patientId;
    string patientName;
    double amount;
public:
    Invoice();
    void input();
    void display() const;

    int getId() const { return id; }
    int getPid() const { return patientId; }
};
#endif
