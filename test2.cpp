#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <ctime>
#include <algorithm>
#include <cctype>
using namespace std;

/* ======================== Phone validation ======================== */
bool validatePhoneStrict(const std::string& in) {
    if (in.empty()) return false;
    for (size_t i = 0; i < in.size(); ++i) {
        unsigned char ch = static_cast<unsigned char>(in[i]);
        if (!std::isdigit(ch)) return false; // ch? ch? s?
    }
    if (!(in.size() == 9 || in.size() == 10 || in.size() == 11)) return false;
    if (in[0] != '0') return false;
    if (in.size() >= 2 && in[1] == '0') return false;
    return true;
}

/* ======================== Name validation ======================== */
// H? & Tên: ch? cho ch? cái và kho?ng tr?ng; không s?/ký t? d?c bi?t.
// Chu?n hoá kho?ng tr?ng; không d? r?ng.
string normalizeName(const string& in, bool& ok) {
    ok = false;
    string out;
    bool lastSpace = true; // tránh space d?u
    bool hasLetter = false;

    for (size_t i = 0; i < in.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(in[i]);
        if (c == ' ') { if (!lastSpace) { out.push_back(' '); lastSpace = true; } continue; }
        if (std::isdigit(c) || std::ispunct(c) || std::iscntrl(c)) return string(); // invalid
        out.push_back(char(c)); lastSpace = false; hasLetter = true;
    }
    if (!out.empty() && out[out.size()-1] == ' ') out.erase(out.size()-1);
    if (!hasLetter || out.empty()) return string();
    ok = true; return out;
}

/* ======================== Date helpers ======================== */
bool isLeap(int y) { return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0); }
int daysInMonth(int m, int y) {
    if (m == 4 || m == 6 || m == 9 || m == 11) return 30;
    if (m == 2) return isLeap(y) ? 29 : 28;
    return 31;
}

/* ===== helper: c?ng thêm N tháng (không dùng chrono, tuong thích C++98) ===== */
struct Date;
Date addMonths(const Date& base, int months);

/* ======================== Date ======================== */
struct Date {
    int d, m, y;
    Date(int dd=1, int mm=1, int yy=2000): d(dd), m(mm), y(yy) {}

    static Date today() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        return Date(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
    }

    bool isCalendarValid() const {
        if (y < 1900 || m < 1 || m > 12 || d < 1) return false;
        if (d > daysInMonth(m, y)) return false;
        return true;
    }

    // cho DOB & lastVisit: không vu?t quá hôm nay
    bool isValidNotAfterToday() const {
        if (!isCalendarValid()) return false;
        Date cur = today();
        if (y > cur.y) return false;
        if (y == cur.y && m > cur.m) return false;
        if (y == cur.y && m == cur.m && d > cur.d) return false; // cho phép = hôm nay
        return true;
    }

    // d1 <= this <= d2
    bool isBetweenInclusive(const Date& d1, const Date& d2) const {
        if (!isCalendarValid()) return false;
        // compare this with d1
        if (y < d1.y) return false;
        if (y == d1.y && m < d1.m) return false;
        if (y == d1.y && m == d1.m && d < d1.d) return false;
        // compare this with d2
        if (y > d2.y) return false;
        if (y == d2.y && m > d2.m) return false;
        if (y == d2.y && m == d2.m && d > d2.d) return false;
        return true;
    }

    string str() const {
        stringstream ss;
        ss << setfill('0') << setw(2) << d << "/" << setw(2) << m << "/" << y;
        return ss.str();
    }

    // nh?p cho DOB/lastVisit = hôm nay
    static Date input() {
        int dd, mm, yy; Date temp; Date cur = today();
        cout << "Hom nay: " << setfill('0') << setw(2) << cur.d << "/"
             << setw(2) << cur.m << "/" << cur.y << "\n";
        cout << "Nhap ngay (dd mm yyyy): ";
        while (true) {
            if (!(cin >> dd >> mm >> yy) || dd < 1 || mm < 1 || mm > 12 || yy < 1900) {
                cout << "Ngay khong hop le (dd mm yyyy) hoac ngoai pham vi, nhap lai: ";
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue;
            }
            temp = Date(dd, mm, yy);
            if (!temp.isValidNotAfterToday()) {
                cout << "Ngay khong hop le hoac lon hon ngay hien tai, nhap lai: ";
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue;
            }
            break;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return temp;
    }

    // nh?p cho l?ch khám: hôm nay = ngày = hôm nay + maxMonths
    static Date inputFutureWithinMonths(int maxMonths) {
        int dd, mm, yy; Date temp; Date cur = today();
        Date limit = addMonths(cur, maxMonths);
        cout << "Hom nay: " << setfill('0') << setw(2) << cur.d << "/"
             << setw(2) << cur.m << "/" << cur.y << "\n";
        cout << "Chi duoc dat den: " << limit.str() << "\n";
        cout << "Nhap ngay hen (dd mm yyyy): ";
        while (true) {
            if (!(cin >> dd >> mm >> yy) || dd < 1 || mm < 1 || mm > 12 || yy < 1900) {
                cout << "Ngay khong hop le (dd mm yyyy) hoac ngoai pham vi, nhap lai: ";
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue;
            }
            temp = Date(dd, mm, yy);
            if (!temp.isCalendarValid()) {
                cout << "Ngay khong hop le, nhap lai: ";
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue;
            }
            if (!temp.isBetweenInclusive(cur, limit)) {
                cout << "Ngay hen phai tu " << cur.str() << " den " << limit.str() << ". Nhap lai: ";
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue;
            }
            break;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return temp;
    }
};

int cmpDate(const Date& a, const Date& b) {
    if (a.y != b.y) return a.y < b.y ? -1 : 1;
    if (a.m != b.m) return a.m < b.m ? -1 : 1;
    if (a.d != b.d) return a.d < b.d ? -1 : 1;
    return 0;
}

Date addMonths(const Date& base, int months) {
    int y = base.y;
    int m = base.m + months;
    int d = base.d;
    // chu?n hoá tháng
    if (m > 0) {
        y += (m - 1) / 12;
        m  = (m - 1) % 12 + 1;
    } else {
        int dec = (-m) / 12 + 1;
        y -= dec;
        m += 12 * dec;
    }
    int dim = daysInMonth(m, y);
    if (d > dim) d = dim;
    return Date(d, m, y);
}

/* ======================== TimeHM (C++98) ======================== */
struct TimeHM {
    int hour;
    int minute;

    TimeHM(int h = 0, int m = 0) : hour(h), minute(m) {}

    static TimeHM now() {
        time_t t = time(0);
        tm* l = localtime(&t);
        return TimeHM(l->tm_hour, l->tm_min);
    }

    bool isValid() const { return hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59; }

    string str() const {
        stringstream ss;
        ss << setfill('0') << setw(2) << hour << ":" << setw(2) << minute;
        return ss.str();
    }

    // n?u h?n hôm nay thì gi? ph?i >= hi?n t?i
    static TimeHM inputForDate(const Date& d) {
        TimeHM t;
        Date today = Date::today();
        TimeHM cur = TimeHM::now();

        cout << "Nhap gio hen (hh mm) 24h: ";
        while (true) {
            if (!(cin >> t.hour >> t.minute) || !t.isValid()) {
                cout << "Gio/phut khong hop le, nhap lai: ";
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue;
            }
            if (cmpDate(d, today) == 0) {
                if (t.hour < cur.hour || (t.hour == cur.hour && t.minute < cur.minute)) {
                    cout << "Khong the dat gio o qua khu (hom nay). Nhap lai: ";
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue;
                }
            }
            break;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return t;
    }
};

int cmpTime(const TimeHM& a, const TimeHM& b) {
    if (a.hour != b.hour) return a.hour < b.hour ? -1 : 1;
    if (a.minute != b.minute) return a.minute < b.minute ? -1 : 1;
    return 0;
}

/* ======================== B?ng (k? b?ng d?p) ======================== */
string fitLeft(const string& s, int w) {
    if ((int)s.size() >= w) return s.substr(0, w);
    return s + string(w - (int)s.size(), ' ');
}
void drawSep(const vector<int>& W) {
    cout << '+';
    for (size_t i = 0; i < W.size(); ++i) {
        cout << string(W[i], '-') << '+';
    }
    cout << "\n";
}
void drawRow(const vector<int>& W, const vector<string>& cells) {
    cout << '|';
    for (size_t i = 0; i < W.size(); ++i) {
        string cell = (i < cells.size() ? cells[i] : "");
        cout << fitLeft(cell, W[i]) << '|';
    }
    cout << "\n";
}

/* ======================== Patient ======================== */
class Patient {
    int id;
    string name, phone, disease;
    Date dob, lastVisit;
public:
    Patient(int i=0, string n="", string p="", string dis="", Date d=Date(), Date lv=Date())
        : id(i), name(n), phone(p), disease(dis), dob(d), lastVisit(lv) {}

    void input(int newId) {
        id = newId;

        cout << "Ho ten : ";
        while (true) {
            string raw;
            getline(cin >> ws, raw);
            bool ok;
            string nm = normalizeName(raw, ok);
            if (ok) { name = nm; break; }
            cout << "Ho ten khong hop le (khong so/ky tu dac biet). Nhap lai: ";
        }

        cout << "So dien thoai : ";
        while (true) {
            string raw; getline(cin, raw);
            if (validatePhoneStrict(raw)) { phone = raw; break; }
            cout << "SDT khong hop le. Yeu cau: chi chu so; do dai hop le; ";
        }

        cout << "Benh ly: ";
        getline(cin, disease);

        cout << "Ngay sinh:\n";
        dob = Date::input();

        cout << "Ngay kham gan nhat:\n";
        // lastVisit ph?i: dob <= lastVisit <= hôm nay
        while (true) {
            Date lv = Date::input();
            Date today = Date::today();
            if (lv.isBetweenInclusive(dob, today)) { lastVisit = lv; break; }
            cout << "Ngay kham gan nhat khong hop le (phai >= Ngay sinh va <= Hom nay). Nhap lai:\n";
        }
    }

    vector<string> toRow() const {
        string nameCut = name.size() > 20 ? name.substr(0,20) : name;
        string disCut  = disease.size() > 18 ? disease.substr(0,18) : disease;
        vector<string> cells;
        stringstream ss;
        ss << id; cells.push_back(ss.str()); ss.str(""); ss.clear();
        cells.push_back(nameCut);
        cells.push_back(phone);
        cells.push_back(disCut);
        cells.push_back(dob.str());
        cells.push_back(lastVisit.str());
        return cells;
    }

    int getId() const { return id; }
    string getName() const { return name; }
};

/* ======================== InvoiceItem ======================== */
class InvoiceItem {
    string name;
    int qty;
    double price;
public:
    InvoiceItem(string n="", int q=1, double p=0): name(n), qty(q), price(p) {}

    void input() {
        cout << "Ten dich vu: "; getline(cin >> ws, name);
        cout << "So luong: ";
        while (!(cin >> qty) || qty <= 0) {
            cout << "So luong khong hop le, nhap lai: ";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Don gia: ";
        while (!(cin >> price) || price < 0) {
            cout << "Don gia khong hop le, nhap lai: ";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    double total() const { return qty * price; }

    vector<string> toRow() const {
        string nameCut = name.size() > 20 ? name.substr(0,20) : name;
        stringstream ss;
        vector<string> cells;
        cells.push_back(nameCut);
        ss << qty; cells.push_back(ss.str()); ss.str(""); ss.clear();
        ss << fixed << setprecision(2) << price; cells.push_back(ss.str()); ss.str(""); ss.clear();
        ss << fixed << setprecision(2) << total(); cells.push_back(ss.str()); ss.str(""); ss.clear();
        return cells;
    }
};

/* ======================== Invoice ======================== */
class Invoice {
    int id;
    int patientId;
    Date date;
    vector<InvoiceItem> items;
public:
    Invoice(int i=0,int pid=0): id(i), patientId(pid) {}

    void input(int newId, int pid) {
        id = newId; patientId = pid;
        cout << "Nhap ngay lap hoa don:\n";
        date = Date::input();

        while (true) {
            InvoiceItem it;
            cout << "\n--- Nhap muc hoa don ---\n";
            it.input();
            items.push_back(it);

            char cont;
            cout << "Them muc khac? (y/n): ";
            if (!(cin >> cont)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (cont == 'n' || cont == 'N') break;
        }
    }

    double total() const {
        double s=0;
        for (size_t i=0; i<items.size(); ++i) s += items[i].total();
        return s;
    }

    void display() const {
        cout << "\n=== HOA DON #" << id << " cho Benh nhan ID " << patientId << " ===\n";
        cout << "Ngay: " << date.str() << "\n";
        vector<int> W; W.push_back(20); W.push_back(6); W.push_back(12); W.push_back(12);
        vector<string> H; H.push_back("Dich vu"); H.push_back("SL"); H.push_back("Don gia"); H.push_back("Thanh tien");
        drawSep(W); drawRow(W, H); drawSep(W);
        for (size_t i=0; i<items.size(); ++i) drawRow(W, items[i].toRow());
        drawSep(W);
        stringstream ss; ss << fixed << setprecision(2) << total() << " VND";
        cout << "Tong: " << ss.str() << "\n";
    }

    int getId() const { return id; }
    int getPid() const { return patientId; }
    Date getDate() const { return date; }
};

/* ======================== Appointment ======================== */
class Appointment {
    int id;
    int patientId;
    Date date;
    TimeHM time;
    string note;
public:
    Appointment(int i=0, int pid=0, Date d=Date(), TimeHM t=TimeHM(), string n="")
        : id(i), patientId(pid), date(d), time(t), note(n) {}

    void input(int newId, int pid) {
        id = newId; patientId = pid;
        cout << "=== Dat lich kham cho Benh nhan ID " << patientId << " ===\n";
        // Ngày h?n: hôm nay .. hôm nay+3 tháng (b?t bu?c)
        date = Date::inputFutureWithinMonths(3);
        // Gi? h?n: n?u hôm nay thì gi? >= hi?n t?i
        time = TimeHM::inputForDate(date);
        cout << "Ghi chu (co the de trong): ";
        getline(cin >> ws, note);
    }

    vector<string> toRow() const {
        string noteCut = note.size() > 30 ? note.substr(0,30) : note;
        vector<string> cells;
        stringstream ss;
        ss << id; cells.push_back(ss.str()); ss.str(""); ss.clear();
        ss << patientId; cells.push_back(ss.str()); ss.str(""); ss.clear();
        cells.push_back(date.str());
        cells.push_back(time.str());
        cells.push_back(noteCut);
        return cells;
    }

    int getId() const { return id; }
    int getPid() const { return patientId; }
    Date getDate() const { return date; }
    TimeHM getTime() const { return time; }

    bool operator<(const Appointment& other) const {
        int dc = cmpDate(date, other.date);
        if (dc != 0) return dc < 0;
        return cmpTime(time, other.time) < 0;
    }
};

/* ======================== Clinic ======================== */
class Clinic {
    vector<Patient> patients;
    vector<Invoice> invoices;
    vector<Appointment> appointments;
    int nextPid = 1000, nextInv = 5000, nextAppt = 8000;

    bool hasPatient(int pid) const {
        for (size_t i=0; i<patients.size(); ++i)
            if (patients[i].getId() == pid) return true;
        return false;
    }
public:
    void addPatient() {
        Patient p; p.input(nextPid++); patients.push_back(p);
        cout << "Da them benh nhan co ID: " << p.getId() << "!\n";
    }

    void listPatients() const {
        cout << "\n--- DANH SACH BENH NHAN ---\n";
        // tang d? r?ng 2 c?t ngày d? không c?t ch?/title
        vector<int> W; W.push_back(6); W.push_back(20); W.push_back(14); W.push_back(18); W.push_back(12); W.push_back(12);
        vector<string> H; H.push_back("ID"); H.push_back("Ho ten"); H.push_back("SDT"); H.push_back("Benh ly"); H.push_back("Ngay sinh"); H.push_back("Lan kham");
        drawSep(W); drawRow(W, H); drawSep(W);
        for (size_t i=0; i<patients.size(); ++i) drawRow(W, patients[i].toRow());
        drawSep(W);
        if (patients.empty()) cout << "(Chua co benh nhan)\n";
    }

    void createInvoice() {
        int pid;
        cout << "Nhap ID benh nhan: ";
        if (!(cin >> pid)) {
            cout << "ID khong hop le!\n";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!hasPatient(pid)) { cout << "Khong tim thay benh nhan voi ID " << pid << "!\n"; return; }
        Invoice iv; iv.input(nextInv++, pid); invoices.push_back(iv);
        cout << "Hoa don #" << iv.getId() << " da duoc tao thanh cong!\n";
    }

    void listInvoices() const {
        cout << "\n--- DANH SACH HOA DON ---\n";
        vector<int> W; W.push_back(8); W.push_back(10); W.push_back(12); W.push_back(16);
        vector<string> H; H.push_back("ID HD"); H.push_back("ID BN"); H.push_back("Ngay"); H.push_back("Tong tien");
        drawSep(W); drawRow(W, H); drawSep(W);
        for (size_t i=0; i<invoices.size(); ++i) {
            vector<string> row;
            stringstream ss;
            ss << invoices[i].getId(); row.push_back(ss.str()); ss.str(""); ss.clear();
            ss << invoices[i].getPid(); row.push_back(ss.str()); ss.str(""); ss.clear();
            row.push_back(invoices[i].getDate().str());
            ss << fixed << setprecision(2) << invoices[i].total() << " VND"; row.push_back(ss.str()); ss.str(""); ss.clear();
            drawRow(W, row);
        }
        drawSep(W);
        if (invoices.empty()) cout << "(Chua co hoa don)\n";
    }

    void showInvoiceDetail() const {
        int id;
        cout << "Nhap ID hoa don: ";
        if (!(cin >> id)) {
            cout << "ID khong hop le!\n";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        for (size_t i=0; i<invoices.size(); ++i)
            if (invoices[i].getId()==id) { invoices[i].display(); return; }

        cout << "Khong tim thay hoa don voi ID " << id << "!\n";
    }

    void stats() const {
        double sum=0;
        for (size_t i=0; i<invoices.size(); ++i) sum += invoices[i].total();
        cout << "\n--- THONG KE PHONG KHAM ---\n";
        vector<int> W; W.push_back(20); W.push_back(12);
        vector<string> H; H.push_back("Chi muc"); H.push_back("Gia tri");
        drawSep(W); drawRow(W, H); drawSep(W);
        vector<string> r1; r1.push_back("Tong benh nhan"); { stringstream ss; ss << patients.size(); r1.push_back(ss.str()); } drawRow(W, r1);
        vector<string> r2; r2.push_back("Tong hoa don");   { stringstream ss; ss << invoices.size(); r2.push_back(ss.str()); } drawRow(W, r2);
        vector<string> r3; r3.push_back("Tong doanh thu"); { stringstream ss; ss << fixed << setprecision(2) << sum << " VND"; r3.push_back(ss.str()); } drawRow(W, r3);
        vector<string> r4; r4.push_back("Tong lich kham"); { stringstream ss; ss << appointments.size(); r4.push_back(ss.str()); } drawRow(W, r4);
        drawSep(W);
    }

    // --- L?CH KHÁM ---
    void createAppointment() {
        if (patients.empty()) { cout << "Chua co benh nhan. Vui long them benh nhan truoc.\n"; return; }
        int pid; cout << "Nhap ID benh nhan can dat lich: ";
        if (!(cin >> pid)) { cout << "ID khong hop le!\n"; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); return; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!hasPatient(pid)) { cout << "Khong tim thay benh nhan voi ID " << pid << "!\n"; return; }
        Appointment ap; ap.input(nextAppt++, pid); appointments.push_back(ap);
        cout << "Da dat lich #" << (nextAppt-1) << " thanh cong cho BN " << pid << "!\n";
    }

    void listAppointments() {
        cout << "\n--- DANH SACH LICH KHAM (toi da 3 thang ke tu hom nay) ---\n";

        Date today = Date::today();
        Date limit = addMonths(today, 3);

        vector<Appointment> filtered;
        for (size_t i=0; i<appointments.size(); ++i) {
            const Date& d = appointments[i].getDate();
            if (cmpDate(d, today) >= 0 && cmpDate(d, limit) <= 0) {
                filtered.push_back(appointments[i]);
            }
        }

        if (filtered.empty()) {
            cout << "(Khong co lich hen trong 3 thang toi)\n";
            return;
        }

        sort(filtered.begin(), filtered.end());

        vector<int> W; W.push_back(8); W.push_back(10); W.push_back(12); W.push_back(8); W.push_back(30);
        vector<string> H; H.push_back("ID LK"); H.push_back("ID BN"); H.push_back("Ngay"); H.push_back("Gio"); H.push_back("Ghi chu");
        drawSep(W); drawRow(W, H); drawSep(W);
        for (size_t i=0; i<filtered.size(); ++i) drawRow(W, filtered[i].toRow());
        drawSep(W);

        cout << "Hien thi tu " << today.str() << " den " << limit.str() << ".\n";
    }
};

/* ======================== main ======================== */
int main() {
    Clinic c;
    while (true) {
        cout << "\n============================\n";
        cout << "  QUAN LY PHONG KHAM (CLASS)\n";
        cout << "============================\n";
        cout << "1. Them benh nhan\n";
        cout << "2. Danh sach benh nhan\n";
        cout << "3. Tao hoa don\n";
        cout << "4. Danh sach hoa don\n";
        cout << "5. Chi tiet hoa don\n";
        cout << "6. Thong ke\n";
        cout << "7. Dat lich kham\n";
        cout << "8. Danh sach lich kham\n";
        cout << "0. Thoat\n";
        cout << "Chon: ";

        int ch;
        if (!(cin >> ch)) {
            cout << "Lua chon khong hop le!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // d?n buffer

        switch (ch) {
            case 1: c.addPatient(); break;
            case 2: c.listPatients(); break;
            case 3: c.createInvoice(); break;
            case 4: c.listInvoices(); break;
            case 5: c.showInvoiceDetail(); break;
            case 6: c.stats(); break;
            case 7: c.createAppointment(); break;
            case 8: c.listAppointments(); break;
            case 0:
                cout << "Tam biet!\n";
                system("pause");
                return 0;
            default: cout << "Lua chon khong hop le!\n";
        }
    }
}

