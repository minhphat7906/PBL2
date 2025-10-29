#ifndef CLINIC_H
#define CLINIC_H
#include "Patient.h"
#include "Invoice.h"
#include "Appointment.h"
#include "Utils.h"

class Clinic {
    std::vector<Patient> patients;
    std::vector<Invoice> invoices;
    std::vector<Appointment> appointments;
    int nextPid = 1000, nextInv = 5000, nextAppt = 8000;

    bool hasPatient(int pid) const;
public:
    void addPatient();
    void listPatients() const;
    void createInvoice();
    void listInvoices() const;
    void showInvoiceDetail() const;
    void stats() const;
    void createAppointment();
    void listAppointments();
    void searchPatient() const;
    void editPatient();
    void deletePatient();
};
#endif
