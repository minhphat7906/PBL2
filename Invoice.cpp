#include "Invoice.h"
int Invoice::NEXT_ID = 5000;

Invoice::Invoice() { id = NEXT_ID++; }

void Invoice::input() {
    cout << "Nhap ID benh nhan: ";
    if (!(cin >> patientId)) { cin.clear(); cin.ignore(999,'\n'); patientId = -1; }
    cin.ignore();
    cout << "Nhap ten benh nhan: "; getline(cin, patientName);
    cout << "Nhap tong tien (VND): ";
    if (!(cin >> amount)) { amount = 0; }
    cin.ignore();
}

void Invoice::display() const {
    cout << "Hoa don #" << id << " | BN ID: " << patientId 
         << " | " << patientName << " | Tong: " << amount << " VND\n";
}
