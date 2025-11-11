#ifndef CLINIC_H
#define CLINIC_H
#include <string>
#include <vector>
#include <unordered_map>
#include "Patient.h"
#include "Invoice.h"
#include "Staff.h"
using namespace std;

// Các vai trò trong hệ thống
enum Role { ADMIN, DOCTOR_USER, NURSE_USER, PATIENT_USER };

// Tài khoản đăng nhập
struct Account {
    string username, password;
    Role role;
    int linkedPatientId;
    Account(string u="", string p="", Role r=PATIENT_USER, int pid=-1)
        : username(u), password(p), role(r), linkedPatientId(pid) {}
};

class Clinic {
    vector<Account> accounts;
    vector<Patient> patients;
    vector<Invoice> invoices;
    vector<Staff> staffs;
    unordered_map<int,int> assignment; // patientId -> staffId
public:
    Clinic();

    // Đăng nhập và menu
    Role login(int &linkedPatientId);
    bool mainMenu(Role role, int linkedPatientId);

    // Bệnh nhân
    void addPatient();
    void listPatients(Role role, int pid) const;

    // Hóa đơn
    void createInvoice();
    void listInvoices(Role role, int pid) const;

    // Nhân viên
    void addStaff();
    void listStaff() const;

    // Phân công
    void assignPatientToStaff();
    void listAssignments(Role role, int pid) const;
};
#endif
