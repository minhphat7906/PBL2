#include "Clinic.h"
#include <iostream>
#include <limits>
using namespace std;

Clinic::Clinic() {
    // Danh sách tài khoản mặc định
    accounts.push_back(Account("admin", "123", ADMIN));
    accounts.push_back(Account("doctor", "123", DOCTOR_USER));
    accounts.push_back(Account("nurse",  "123", NURSE_USER));
    accounts.push_back(Account("patient", "123", PATIENT_USER, 1000));
}

Role Clinic::login(int &linkedPatientId) {
    string u, p;
    while (true) {
        system("cls");
        cout << "=====================================\n";
        cout << "     HE THONG QUAN LY PHONG KHAM\n";
        cout << "=====================================\n\n";
        cout << "Ten dang nhap: "; getline(cin, u);
        cout << "Mat khau: "; getline(cin, p);

        for (auto &a : accounts) {
            if (a.username==u && a.password==p) {
                linkedPatientId = a.linkedPatientId;
                cout << "\n✅ Dang nhap thanh cong voi vai tro: ";
                switch (a.role) {
                    case ADMIN: cout << "Admin"; break;
                    case DOCTOR_USER: cout << "Bac si"; break;
                    case NURSE_USER: cout << "Y ta"; break;
                    case PATIENT_USER: cout << "Benh nhan"; break;
                }
                cout << "\nNhan Enter de tiep tuc..."; cin.get();
                return a.role;
            }
        }
        cout << "\n❌ Sai thong tin! Nhan Enter de thu lai..."; cin.get();
    }
}

bool Clinic::mainMenu(Role role, int linkedPatientId) {
    while (true) {
        system("cls");
        cout << "============================\n";
        cout << "  HE THONG QUAN LY PHONG KHAM\n";
        cout << "============================\n";
        cout << "Vai tro hien tai: ";
        switch (role) {
            case ADMIN: cout << "Admin"; break;
            case DOCTOR_USER: cout << "Bac si"; break;
            case NURSE_USER: cout << "Y ta"; break;
            case PATIENT_USER: cout << "Benh nhan"; break;
        }
        cout << "\n============================\n";
        cout << "1. Them benh nhan\n";
        cout << "2. Danh sach benh nhan\n";
        cout << "3. Tao hoa don\n";
        cout << "4. Danh sach hoa don\n";
        cout << "5. Them nhan vien\n";
        cout << "6. Danh sach nhan vien\n";
        cout << "7. Phan cong nhan vien\n";
        cout << "8. Danh sach phan cong\n";
        cout << "9. 🔄 Dang xuat\n";
        cout << "0. ❌ Thoat chuong trinh\n";
        cout << "Chon: ";

        int ch;
        if (!(cin >> ch)) { cin.clear(); cin.ignore(999,'\n'); continue; }
        cin.ignore();

        switch (ch) {
            case 1:
                if (role==ADMIN) addPatient();
                else cout << "❌ Khong co quyen!\n";
                break;
            case 2:
                listPatients(role, linkedPatientId);
                break;
            case 3:
                if (role==ADMIN) createInvoice();
                else cout << "❌ Khong co quyen!\n";
                break;
            case 4:
                listInvoices(role, linkedPatientId);
                break;
            case 5:
                if (role==ADMIN) addStaff();
                else cout << "❌ Khong co quyen!\n";
                break;
            case 6:
                listStaff();
                break;
            case 7:
                if (role==ADMIN) assignPatientToStaff();
                else cout << "❌ Khong co quyen!\n";
                break;
            case 8:
                listAssignments(role, linkedPatientId);
                break;
            case 9:
                return true;   // 🔄 Đăng xuất → quay lại login
            case 0:
                return false;  // ❌ Thoát hẳn
            default:
                cout << "Lua chon khong hop le!\n";
                break;
        }
        cout << "\nNhan Enter de quay lai menu..."; cin.get();
    }
}

/* ===== BỆNH NHÂN ===== */
void Clinic::addPatient() {
    system("cls");
    cout << "===== THEM BENH NHAN =====\n";
    Patient p; 
    p.input();
    patients.push_back(p);

    // Tạo tài khoản đăng nhập cho bệnh nhân mới
    string username = "bn" + to_string(p.getId());
    string password = "123";
    accounts.push_back(Account(username, password, PATIENT_USER, p.getId()));

    cout << "\n✅ Da them benh nhan!\n";
    cout << "👉 Tai khoan dang nhap cua benh nhan: " << username 
         << " | Mat khau mac dinh: " << password << "\n";
}


void Clinic::listPatients(Role role, int pid) const {
    system("cls");
    cout << "===== DANH SACH BENH NHAN =====\n";
    if (patients.empty()) { cout << "(Chua co benh nhan)\n"; return; }

    for (const auto &p : patients)
        if (role!=PATIENT_USER || p.getId()==pid)
            p.display();
}

/* ===== HÓA ĐƠN ===== */
void Clinic::createInvoice() {
    system("cls");
    cout << "===== TAO HOA DON =====\n";
    Invoice iv; iv.input();
    invoices.push_back(iv);
    cout << "\n✅ Da tao hoa don!\n";
}

void Clinic::listInvoices(Role role, int pid) const {
    system("cls");
    cout << "===== DANH SACH HOA DON =====\n";
    if (invoices.empty()) { cout << "(Chua co hoa don)\n"; return; }

    for (const auto &iv : invoices)
        if (role!=PATIENT_USER || iv.getPid()==pid)
            iv.display();
}

/* ===== NHÂN VIÊN ===== */
void Clinic::addStaff() {
    system("cls");
    cout << "===== THEM NHAN VIEN =====\n";
    Staff s; s.input();
    staffs.push_back(s);
    cout << "\n✅ Da them nhan vien!\n";
}

void Clinic::listStaff() const {
    system("cls");
    cout << "===== DANH SACH NHAN VIEN =====\n";
    if (staffs.empty()) { cout << "(Chua co nhan vien)\n"; return; }

    for (const auto &s : staffs) s.display();
}

/* ===== PHÂN CÔNG ===== */
void Clinic::assignPatientToStaff() {
    system("cls");
    cout << "===== PHAN CONG NHAN VIEN =====\n";
    if (patients.empty() || staffs.empty()) {
        cout << "Chua co du lieu!\n";
        return;
    }

    cout << "\n--- Benh nhan ---\n";
    for (auto &p : patients) p.display();

    cout << "\n--- Nhan vien ---\n";
    for (auto &s : staffs) s.display();

    int pid, sid;
    cout << "Nhap ID benh nhan: "; cin >> pid;
    cout << "Nhap ID nhan vien: "; cin >> sid;
    cin.ignore();

    assignment[pid] = sid;
    cout << "✅ Da phan cong thanh cong!\n";
}

void Clinic::listAssignments(Role role, int pid) const {
    system("cls");
    cout << "===== DANH SACH PHAN CONG =====\n";
    if (assignment.empty()) { cout << "(Chua co phan cong)\n"; return; }

    for (auto &kv : assignment)
        if (role!=PATIENT_USER || kv.first==pid)
            cout << "BN #" << kv.first << " duoc quan ly boi NV #" << kv.second << "\n";
}
