#include "Staff.h"
int Staff::NEXT_ID = 2000;

Staff::Staff() { id = NEXT_ID++; }

void Staff::input() {
    cout << "Nhap ho ten nhan vien: "; getline(cin, name);
    int r; 
    do {
        cout << "Chon vai tro (0=Bac si, 1=Y ta): ";
        if (!(cin >> r)) { cin.clear(); cin.ignore(999,'\n'); r=-1; }
    } while (r!=0 && r!=1);
    role = (r==0 ? DOCTOR : NURSE);
    cin.ignore();
    cout << "Nhap so dien thoai: "; getline(cin, phone);
}

void Staff::display() const {
    cout << "[" << id << "] " << name << " | " << roleName(role) 
         << " | " << phone << "\n";
}
