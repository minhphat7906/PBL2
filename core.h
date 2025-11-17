#pragma once

#include <QString>
#include <QStringList>
#include <QVector>
#include <queue>

class QWidget; // forward declare, dùng cho exportAppointmentToICS

// ====================== PHÂN QUYỀN, TÀI KHOẢN ======================

enum class Role { ADMIN = 0, DOCTOR = 1, PATIENT = 2 };

QString roleToString(Role r);

// ====================== CẤU TRÚC DỮ LIỆU CHÍNH ======================

struct Account
{
    QString username;
    QString password;
    Role    role;
    int     linkedPatientId;   // với PATIENT: id bệnh nhân tương ứng; các role khác = -1
};

struct Patient
{
    int     id {};
    int     hasBHYT {};           // 0/1
    QString name;
    QString phone;
    QString disease;
    QString dob;                  // dd/MM/yyyy
    QString lastVisit;            // lần khám gần nhất
    QString address;
};

struct Appointment
{
    int     id {};
    int     patientId {};
    QString date;                 // dd/MM/yyyy
    QString time;                 // HH:mm
    QString doctor;               // username bác sĩ
    QString priority;             // "Thường" / "Ưu tiên" / "Khẩn"
    QString note;                 // ghi chú thêm
};

struct Invoice
{
    int     id {};
    int     patientId {};
    QString date;
    double  total {};
};

struct Visit
{
    int     id {};
    int     patientId {};
    QString date;
    QString doctor;
    QString diagnosis;            // chẩn đoán cho bệnh nhân
    QString internalNote;         // ghi chú nội bộ (chỉ bác sĩ/admin)
};

// ====================== BIẾN TOÀN CỤC (in-memory) ======================

extern QVector<Patient>     gPatients;
extern QVector<Appointment> gAppointments;
extern QVector<Invoice>     gInvoices;
extern QVector<Visit>       gVisits;
extern QVector<Account>     gAccounts;

extern int gNextPatientId;
extern int gNextAppointment;
extern int gNextInvoice;
extern int gNextVisit;

extern bool gLogoutRequested;
extern std::queue<int> gQueue;    // hàng chờ khám (chỉ lưu ID bệnh nhân)

// ====================== HÀM CSV (LOAD/SAVE) ======================

void loadPatientsCSV(const QString &path = "patients.csv");
void savePatientsCSV(const QString &path = "patients.csv");

void loadAppointmentsCSV(const QString &path = "appointments.csv");
void saveAppointmentsCSV(const QString &path = "appointments.csv");

void loadInvoicesCSV(const QString &path = "invoices.csv");
void saveInvoicesCSV(const QString &path = "invoices.csv");

void loadVisitsCSV(const QString &path = "visits.csv");
void saveVisitsCSV(const QString &path = "visits.csv");

void loadAccountsCSV(const QString &path = "accounts.csv");
void saveAccountsCSV(const QString &path = "accounts.csv");

// tạo sẵn account, xóa account bệnh nhân
void ensureAccounts();
void removePatientAccount(int pid);

// ====================== HÀM TIỆN ÍCH ======================

// Lấy tên bệnh nhân theo ID
QString patientNameById(int id);

// Danh sách username bác sĩ (từ accounts)
QStringList listDoctors();

// Kiểm tra trùng lịch (cùng BN hoặc cùng bác sĩ cùng ngày/giờ)
bool isConflict(const Appointment &cand, int ignoreId = -1);

// Gợi ý các khung giờ trống cho 1 bác sĩ trong ngày (mỗi 30 phút)
QStringList suggestSlots(const QString &date, const QString &doctor, int stepMin = 30);

// Xuất 1 lịch khám thành file .ics (có thể import vào Google Calendar, Outlook...)
bool exportAppointmentToICS(const Appointment &a, QWidget *parent);
