#ifndef PATIENT_H
#define PATIENT_H
#include <string>
#include <iostream>
using namespace std;

class Patient {
    static int NEXT_ID;
    int id;
    string name, phone, disease;
public:
    Patient();
    void input();
    void display() const;

    int getId() const { return id; }
    string getName() const { return name; }
    string getPhone() const { return phone; }
};
#endif
