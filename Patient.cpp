#include "Patient.h"
int Patient::NEXT_ID = 1000;

Patient::Patient() { id = NEXT_ID++; }

void Patient::input() {
    cout << "Nhap ho ten: "; getline(cin, name);
    cout << "Nhap so dien thoai: "; getline(cin, phone);
    cout << "Nhap benh ly: "; getline(cin, disease);
}

void Patient::display() const {
    cout << "[" << id << "] " << name << " | " << phone << " | " << disease << "\n";
}
