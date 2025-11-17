#include "core.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QDate>
#include <QTime>
#include <QDateTime>

#include <algorithm>

// ====================== BIẾN TOÀN CỤC ĐỊNH NGHĨA Ở ĐÂY ======================

QVector<Patient>     gPatients;
QVector<Appointment> gAppointments;
QVector<Invoice>     gInvoices;
QVector<Visit>       gVisits;
QVector<Account>     gAccounts;

int gNextPatientId   = 1000;
int gNextAppointment = 8000;
int gNextInvoice     = 5000;
int gNextVisit       = 9000;

bool gLogoutRequested = false;
std::queue<int> gQueue;

// ====================== ROLE TO STRING ======================

QString roleToString(Role r)
{
    if (r == Role::ADMIN)  return "ADMIN";
    if (r == Role::DOCTOR) return "DOCTOR";
    return "PATIENT";
}

// ====================== HÀM PHỤ TRỢ CSV (NỘI BỘ) ======================

static QStringList splitCSV(const QString &line)
{
    return line.split(',', Qt::KeepEmptyParts);
}

// ----- Patients CSV -----

static QString patientToCSV(const Patient &p)
{
    return QString("%1,%2,%3,%4,%5,%6,%7,%8")
    .arg(p.id)
        .arg(p.name)
        .arg(p.phone)
        .arg(p.disease)
        .arg(p.dob)
        .arg(p.lastVisit)
        .arg(p.address)
        .arg(p.hasBHYT);
}

void loadPatientsCSV(const QString &path)
{
    gPatients.clear();
    gNextPatientId = 1000;

    QFile f(path);
    if (!f.exists()) return;
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&f);
    qint64 pos = f.pos();
    QString first = in.readLine();
    if (!first.startsWith("id,name"))
    {
        // file không có header → đọc lại từ đầu
        f.seek(pos);
        in.seek(pos);
    }

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        auto c = splitCSV(line);
        if (c.size() < 6) continue;

        Patient p;
        p.id        = c[0].toInt();
        p.name      = c[1];
        p.phone     = c[2];
        p.disease   = c[3];
        p.dob       = c[4];
        p.lastVisit = c[5];
        p.address   = (c.size() >= 7 ? c[6] : "");
        p.hasBHYT   = (c.size() >= 8 ? c[7].toInt() : 0);

        gPatients.push_back(p);
        gNextPatientId = std::max(gNextPatientId, p.id + 1);
    }
    f.close();
}

void savePatientsCSV(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "CSV", "Không ghi được patients.csv");
        return;
    }
    QTextStream out(&f);
    out << "id,name,phone,disease,dob,lastVisit,address,hasBHYT\n";
    for (const auto &p : gPatients)
        out << patientToCSV(p) << "\n";
    f.close();
}

// ----- Appointments CSV -----

static QString appointmentToCSV(const Appointment &a)
{
    return QString("%1,%2,%3,%4,%5,%6,%7")
    .arg(a.id)
        .arg(a.patientId)
        .arg(a.date)
        .arg(a.time)
        .arg(a.doctor)
        .arg(a.priority)
        .arg(a.note);
}

void loadAppointmentsCSV(const QString &path)
{
    gAppointments.clear();
    gNextAppointment = 8000;

    QFile f(path);
    if (!f.exists()) return;
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&f);
    qint64 pos = f.pos();
    QString first = in.readLine();
    if (!first.startsWith("id,patientId"))
    {
        f.seek(pos);
        in.seek(pos);
    }

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        auto c = splitCSV(line);
        if (c.size() < 5) continue;

        Appointment a;
        a.id        = c[0].toInt();
        a.patientId = c[1].toInt();
        a.date      = c[2];
        a.time      = c[3];

        // cũ: [0..4] = id, pid, date, time, note
        // mới: [0..6] = id, pid, date, time, doctor, priority, note
        if (c.size() >= 7)
        {
            a.doctor   = c[4];
            a.priority = c[5];
            a.note     = c[6];
        }
        else if (c.size() == 5)
        {
            a.doctor   = "(N/A)";
            a.priority = "Thường";
            a.note     = c[4];
        }
        else
        {
            a.doctor   = "(N/A)";
            a.priority = "Thường";
        }

        gAppointments.push_back(a);
        gNextAppointment = std::max(gNextAppointment, a.id + 1);
    }
    f.close();
}

void saveAppointmentsCSV(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "CSV", "Không ghi được appointments.csv");
        return;
    }
    QTextStream out(&f);
    out << "id,patientId,date,time,doctor,priority,note\n";
    for (const auto &a : gAppointments)
        out << appointmentToCSV(a) << "\n";
    f.close();
}

// ----- Invoices CSV -----

static QString invoiceToCSV(const Invoice &iv)
{
    return QString("%1,%2,%3,%4")
    .arg(iv.id)
        .arg(iv.patientId)
        .arg(iv.date)
        .arg(iv.total, 0, 'f', 2);
}

void loadInvoicesCSV(const QString &path)
{
    gInvoices.clear();
    gNextInvoice = 5000;

    QFile f(path);
    if (!f.exists()) return;
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&f);
    qint64 pos = f.pos();
    QString first = in.readLine();
    if (!first.startsWith("id,patientId"))
    {
        f.seek(pos);
        in.seek(pos);
    }

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        auto c = splitCSV(line);
        if (c.size() < 4) continue;

        Invoice iv;
        iv.id        = c[0].toInt();
        iv.patientId = c[1].toInt();
        iv.date      = c[2];
        iv.total     = c[3].toDouble();

        gInvoices.push_back(iv);
        gNextInvoice = std::max(gNextInvoice, iv.id + 1);
    }
    f.close();
}

void saveInvoicesCSV(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "CSV", "Không ghi được invoices.csv");
        return;
    }
    QTextStream out(&f);
    out << "id,patientId,date,total\n";
    for (const auto &iv : gInvoices)
        out << invoiceToCSV(iv) << "\n";
    f.close();
}

// ----- Visits CSV -----

static QString visitToCSV(const Visit &v)
{
    return QString("%1,%2,%3,%4,%5,%6")
    .arg(v.id)
        .arg(v.patientId)
        .arg(v.date)
        .arg(v.doctor)
        .arg(v.diagnosis)
        .arg(v.internalNote);
}

void loadVisitsCSV(const QString &path)
{
    gVisits.clear();
    gNextVisit = 9000;

    QFile f(path);
    if (!f.exists()) return;
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&f);
    qint64 pos = f.pos();
    QString first = in.readLine();
    if (!first.startsWith("id,patientId"))
    {
        f.seek(pos);
        in.seek(pos);
    }

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        auto c = splitCSV(line);
        if (c.size() < 5) continue;

        Visit v;
        v.id        = c[0].toInt();
        v.patientId = c[1].toInt();
        v.date      = c[2];
        v.doctor    = c[3];
        v.diagnosis = c[4];
        v.internalNote = (c.size() >= 6 ? c[5] : "");

        gVisits.push_back(v);
        gNextVisit = std::max(gNextVisit, v.id + 1);
    }
    f.close();
}

void saveVisitsCSV(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "CSV", "Không ghi được visits.csv");
        return;
    }
    QTextStream out(&f);
    out << "id,patientId,date,doctor,diagnosis,internalNote\n";
    for (const auto &v : gVisits)
        out << visitToCSV(v) << "\n";
    f.close();
}

// ----- Accounts CSV -----

static QString accountToCSV(const Account &a)
{
    return QString("%1,%2,%3,%4")
    .arg(a.username)
        .arg(a.password)
        .arg((int)a.role)
        .arg(a.linkedPatientId);
}

void loadAccountsCSV(const QString &path)
{
    gAccounts.clear();

    QFile f(path);
    if (!f.exists()) return;
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&f);
    qint64 pos = f.pos();
    QString first = in.readLine();
    if (!first.startsWith("username,password"))
    {
        f.seek(pos);
        in.seek(pos);
    }

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        auto c = splitCSV(line);
        if (c.size() < 4) continue;

        Account a;
        a.username       = c[0];
        a.password       = c[1];
        a.role           = (Role)c[2].toInt();
        a.linkedPatientId= c[3].toInt();

        gAccounts.push_back(a);
    }
    f.close();
}

void saveAccountsCSV(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "CSV", "Không ghi được accounts.csv");
        return;
    }
    QTextStream out(&f);
    out << "username,password,role,linkedPatientId\n";
    for (const auto &a : gAccounts)
        out << accountToCSV(a) << "\n";
    f.close();
}

// Tạo sẵn tk mặc định nếu chưa có
void ensureAccounts()
{
    loadAccountsCSV();
    if (!gAccounts.isEmpty()) return;

    // Admin & 1 doctor mặc định
    gAccounts.push_back({"admin",  "123", Role::ADMIN,  -1});
    gAccounts.push_back({"doctor", "123", Role::DOCTOR, -1});

    // Tự tạo tài khoản cho từng bệnh nhân đã có trong CSV
    for (const auto &p : gPatients)
        gAccounts.push_back({"bn" + QString::number(p.id), "123", Role::PATIENT, p.id});

    saveAccountsCSV();
}

// Xóa account khi xóa bệnh nhân
void removePatientAccount(int pid)
{
    gAccounts.erase(
        std::remove_if(gAccounts.begin(), gAccounts.end(),
                       [pid](const Account &a)
                       {
                           return a.role == Role::PATIENT && a.linkedPatientId == pid;
                       }),
        gAccounts.end());
    saveAccountsCSV();
}

// ====================== HÀM TIỆN KHÁC ======================

QString patientNameById(int id)
{
    for (const auto &p : gPatients)
        if (p.id == id) return p.name;
    return QString("(ID %1)").arg(id);
}

// Danh sách username bác sĩ (từ accounts)
QStringList listDoctors()
{
    QStringList ds;
    for (const auto &a : gAccounts)
        if (a.role == Role::DOCTOR)
            ds << a.username;

    ds.removeDuplicates();
    if (ds.isEmpty())
        ds << "doctor";
    return ds;
}

// Kiểm tra trùng lịch (cùng BN hoặc cùng bác sĩ cùng ngày/giờ)
bool isConflict(const Appointment &cand, int ignoreId)
{
    for (const auto &ap : gAppointments)
    {
        if (ignoreId != -1 && ap.id == ignoreId) continue;
        if (ap.date == cand.date && ap.time == cand.time)
        {
            if (ap.patientId == cand.patientId) return true;
            if (ap.doctor    == cand.doctor)    return true;
        }
    }
    return false;
}

// Gợi ý các khung giờ trống cho 1 bác sĩ trong ngày (mỗi 30 phút)
QStringList suggestSlots(const QString &date, const QString &doctor, int stepMin)
{
    QStringList res;
    QTime t(8,0), end(17,0);
    while (t <= end)
    {
        QString hhmm = t.toString("HH:mm");
        bool busy = false;
        for (const auto &ap : gAppointments)
        {
            if (ap.date == date && ap.time == hhmm && ap.doctor == doctor)
            {
                busy = true;
                break;
            }
        }
        if (!busy) res << hhmm;
        t = t.addSecs(stepMin * 60);
    }
    return res;
}

// Xuất 1 lịch khám thành file .ics (có thể import vào Google Calendar, Outlook...)
bool exportAppointmentToICS(const Appointment &a, QWidget *parent)
{
    QDate d = QDate::fromString(a.date, "dd/MM/yyyy");
    QTime t = QTime::fromString(a.time, "HH:mm");
    if (!d.isValid() || !t.isValid())
    {
        QMessageBox::warning(parent, ".ics", "Ngày/Giờ không hợp lệ");
        return false;
    }

    QDateTime start(d, t);
    QDateTime end = start.addSecs(1800); // mặc định 30 phút

    QString dtStart = start.toUTC().toString("yyyyMMdd'T'HHmmss'Z'");
    QString dtEnd   = end.toUTC().toString("yyyyMMdd'T'HHmmss'Z'");

    QString defName = QString("appt_%1_%2.ics")
                          .arg(a.patientId)
                          .arg(d.toString("yyyyMMdd"));

    QString path = QFileDialog::getSaveFileName(
        parent,
        "Lưu file .ics",
        defName,
        "iCalendar (*.ics)");

    if (path.isEmpty()) return false;

    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&f);
    out << "BEGIN:VCALENDAR\nVERSION:2.0\nPRODID:-//ClinicMini//VN\n";
    out << "BEGIN:VEVENT\n";
    out << "UID:" << a.id << "@ClinicMini\n";
    out << "DTSTAMP:" << QDateTime::currentDateTimeUtc().toString("yyyyMMdd'T'HHmmss'Z'") << "\n";
    out << "DTSTART:" << dtStart << "\n";
    out << "DTEND:"   << dtEnd   << "\n";
    out << "SUMMARY:Kham benh - " << patientNameById(a.patientId) << "\n";
    out << "DESCRIPTION:Bac si: " << a.doctor
        << "; Muc do: " << a.priority
        << "; Ghi chu: " << a.note << "\n";
    out << "END:VEVENT\nEND:VCALENDAR\n";

    f.close();
    QMessageBox::information(parent, ".ics", "Đã lưu: " + path);
    return true;
}
