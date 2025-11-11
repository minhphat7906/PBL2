#ifndef STAFF_H
#define STAFF_H
#include <string>
#include <iostream>
using namespace std;

enum StaffRole { DOCTOR, NURSE };

class Staff {
    static int NEXT_ID;
    int id;
    string name;
    StaffRole role;
    string phone;
public:
    Staff();
    void input();
    void display() const;

    int getId() const { return id; }
    string getName() const { return name; }
    StaffRole getRole() const { return role; }
    static string roleName(StaffRole r) { return r==DOCTOR ? "Bac si" : "Y ta"; }
};
#endif
