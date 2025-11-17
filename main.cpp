// ===========================
//  CLINIC MINI QUẢN LÝ PHÒNG KHÁM
//  - Có phân quyền: Admin / Doctor / Patient
//  - Lưu dữ liệu bằng file CSV (không dùng database)
//  - Giao diện: Qt Widgets
// ===========================

#include <QApplication>
#include <QMainWindow>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStackedWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QDateEdit>
#include <QComboBox>
#include <QFrame>

#include <QVector>
#include <queue>
#include <algorithm>
#include <functional>
#include "core.h"
#include "ui_pages.h"


// ====================== THEME (màu sắc nhẹ nhàng) ======================

static void applyTheme(QApplication &app)
{
    const char *css = R"CSS(
        QWidget {
            font-family: Segoe UI, Arial, sans-serif;
            font-size: 13px;
        }

        QMainWindow {
            background: #F3F4F6;   /* xám rất nhẹ */
        }

        QPushButton {
            background: #E0ECFF;
            color: #1E3A8A;
            border-radius: 6px;
            padding: 6px 12px;
            border: 1px solid #93C5FD;
            font-weight: 500;
        }
        QPushButton:hover   { background: #C7DDFF; }
        QPushButton:pressed { background: #B3D3FF; }

        QPushButton[variant="secondary"] {
            background: #F9FAFB;
            border: 1px solid #D1D5DB;
            color: #374151;
        }

        QPushButton[variant="danger"] {
            background: #FCA5A5;
            border: 1px solid #F87171;
            color: #7F1D1D;
        }
        QPushButton[variant="danger"]:hover {
            background: #F87171;
        }

        QLineEdit, QComboBox, QDateEdit, QTextEdit {
            background: #FFFFFF;
            border: 1px solid #D1D5DB;
            border-radius: 6px;
            padding: 4px 8px;
            color: #111827;
        }
        QLineEdit:focus, QComboBox:focus, QDateEdit:focus, QTextEdit:focus {
            border: 1px solid #3B82F6;
        }
        QLineEdit::placeholderText {
            color: #9CA3AF;
        }

        QHeaderView::section {
            background: #E5E7EB;
            color: #111827;
            border: none;
            padding: 4px 6px;
            font-weight: 600;
        }
        QTableWidget, QTableView {
            background: #FFFFFF;
            gridline-color: #E5E7EB;
            selection-background-color: transparent;
            selection-color: #111827;
            alternate-background-color: #F9FAFB;
            color: #111827;
        }
        QTableView::item:selected,
        QTableWidget::item:selected {
            background: transparent;
            border: 2px solid #3B82F6;      /* viền xanh nổi bật */
        }

        QLabel[role="title"] {
            font-size: 18px;
            font-weight: 600;
            color: #111827;
        }
        QLabel[role="subtitle"] {
            color: #4B5563;
        }
    )CSS";

    app.setStyleSheet(css);
}


// ====================== MAIN WINDOW (TRANG CHỦ + DASHBOARD) ======================

class MainWindow : public QMainWindow
{
public:
    Account acc;

    QStackedWidget *stack;
    QWidget        *homePage;
    PatientsPage   *patientsPage   = nullptr;
    ApptsPage      *apptsPage      = nullptr;
    InvoicesPage   *invoicesPage   = nullptr;
    VisitsPage     *visitsPage     = nullptr;
    DoctorsPage    *doctorsPage    = nullptr;

    MainWindow(const Account &a) : acc(a)
    {
        setWindowTitle("ClinicMini - " + acc.username +
                       " (" + roleToString(acc.role) + ")");
        resize(1100, 700);

        stack = new QStackedWidget(this);
        setCentralWidget(stack);

        buildHomePage();
        stack->addWidget(homePage);
        stack->setCurrentWidget(homePage);
    }

    // Dashboard nhỏ hiển thị thống kê hôm nay (tùy role)
    QString dashboardTextToday()
    {
        QString today = QDate::currentDate().toString("dd/MM/yyyy");

        if (acc.role == Role::ADMIN)
        {
            int ap = 0, hv = 0;
            double revenue = 0;

            for (const auto &x : gAppointments)
                if (x.date == today) ++ap;
            for (const auto &v : gVisits)
                if (v.date == today) ++hv;
            for (const auto &iv : gInvoices)
                if (iv.date == today) revenue += iv.total;

            return "Hôm nay (" + today + "):\n"
                                         "- Số lịch hẹn: " + QString::number(ap) + "\n"
                                           "- Số lần khám: " + QString::number(hv) + "\n"
                                           "- Doanh thu: " + QString::number(revenue, 'f', 2) + " VND";
        }
        else if (acc.role == Role::DOCTOR)
        {
            int ap = 0, hv = 0;
            for (const auto &x : gAppointments)
                if (x.date == today && x.doctor == acc.username) ++ap;
            for (const auto &v : gVisits)
                if (v.date == today && v.doctor == acc.username) ++hv;

            return "Hôm nay (" + today + ") - Bác sĩ " + acc.username + ":\n"
                                                                        "- Lịch hẹn của bạn: " + QString::number(ap) + "\n"
                                           "- Lần khám của bạn: " + QString::number(hv);
        }
        else // PATIENT
        {
            int ap = 0, hv = 0;
            for (const auto &x : gAppointments)
                if (x.date == today && x.patientId == acc.linkedPatientId) ++ap;
            for (const auto &v : gVisits)
                if (v.date == today && v.patientId == acc.linkedPatientId) ++hv;

            return "Hôm nay (" + today + ") - Bệnh nhân " +
                   patientNameById(acc.linkedPatientId) + ":\n"
                                                          "- Lịch hẹn của bạn: " + QString::number(ap) + "\n"
                                           "- Lần khám của bạn: " + QString::number(hv);
        }
    }

    // Tạo trang home gồm dashboard + các "block" chức năng
    void buildHomePage()
    {
        homePage = new QWidget;
        auto *root = new QVBoxLayout(homePage);

        auto *header = new QHBoxLayout;
        auto *title = new QLabel("Trang chủ - " + acc.username +
                                 " (" + roleToString(acc.role) + ")");
        title->setProperty("role","title");
        auto *btnLogout = new QPushButton("Đăng xuất");
        btnLogout->setProperty("variant","secondary");
        header->addWidget(title);
        header->addStretch();
        header->addWidget(btnLogout);
        root->addLayout(header);

        auto *dash = new QLabel(dashboardTextToday());
        dash->setProperty("role","subtitle");
        dash->setStyleSheet("background:#E5E7EB; border-radius:8px; padding:8px;");
        root->addWidget(dash);

        // Grid các block chức năng
        auto *grid = new QGridLayout;
        root->addLayout(grid);

        auto makeBlock = [&](const QString &text, std::function<void()> onClick)
        {
            auto *btn = new QPushButton(text);
            btn->setMinimumSize(220, 80);
            btn->setStyleSheet("font-size:14px; font-weight:500;");
            QObject::connect(btn, &QPushButton::clicked, onClick);
            return btn;
        };

        int r = 0, c = 0;
        auto addBlock = [&](QPushButton *b)
        {
            grid->addWidget(b, r, c);
            c = (c + 1) % 3;
            if (c == 0) ++r;
        };

        // Admin & Doctor: quản lý bệnh nhân, lịch khám, lịch sử
        if (acc.role == Role::ADMIN || acc.role == Role::DOCTOR)
        {
            addBlock(makeBlock("Bệnh nhân", [this](){ openPatients(); }));
            addBlock(makeBlock("Lịch khám", [this](){ openAppointments(); }));
            addBlock(makeBlock("Lịch sử khám", [this](){ openVisits(); }));
        }

        // Admin: hóa đơn + tài khoản bác sĩ + báo cáo
        if (acc.role == Role::ADMIN)
        {
            addBlock(makeBlock("Hóa đơn", [this](){ openInvoices(); }));
            addBlock(makeBlock("Tài khoản bác sĩ", [this](){ openDoctors(); }));
            addBlock(makeBlock("Xuất báo cáo tổng", [this](){ exportReport(); }));
        }

        // Doctor: báo cáo riêng bác sĩ
        if (acc.role == Role::DOCTOR)
        {
            addBlock(makeBlock("Báo cáo cá nhân", [this](){ exportReport(); }));
        }

        // Bệnh nhân: xem lịch, hóa đơn, lịch sử của chính mình
        if (acc.role == Role::PATIENT)
        {
            addBlock(makeBlock("Lịch khám của tôi", [this](){ openAppointments(); }));
            addBlock(makeBlock("Hóa đơn của tôi", [this](){ openInvoices(); }));
            addBlock(makeBlock("Lịch sử khám của tôi", [this](){ openVisits(); }));
        }

        // Đăng xuất
        connect(btnLogout, &QPushButton::clicked, [this]()
                {
                    gLogoutRequested = true;
                    close();
                });
    }

    // Xuất báo cáo ra file txt (tùy role)
    void exportReport()
    {
        QString today = QDate::currentDate().toString("yyyyMMdd");
        QString path;
        QFile f;
        QTextStream out;

        if (acc.role == Role::ADMIN)
        {
            path = "report_admin_" + today + ".txt";
            f.setFileName(path);
            if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::warning(this, "Báo cáo", "Không ghi được file.");
                return;
            }
            out.setDevice(&f);

            QString dStr = QDate::currentDate().toString("dd/MM/yyyy");
            out << "=== BAO CAO ADMIN NGAY " << dStr << " ===\n";

            int ap = 0, hv = 0;
            double rev = 0;
            for (const auto &a : gAppointments)
                if (a.date == dStr) ++ap;
            for (const auto &v : gVisits)
                if (v.date == dStr) ++hv;
            for (const auto &iv : gInvoices)
                if (iv.date == dStr) rev += iv.total;

            out << "So lich hen: "  << ap  << "\n";
            out << "So lan kham: "  << hv  << "\n";
            out << "Doanh thu: "    << rev << " VND\n";
            out << "Tong benh nhan: " << gPatients.size() << "\n";
            f.close();
        }
        else if (acc.role == Role::DOCTOR)
        {
            path = "report_doctor_" + acc.username + "_" + today + ".txt";
            f.setFileName(path);
            if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::warning(this, "Báo cáo", "Không ghi được file.");
                return;
            }
            out.setDevice(&f);

            QString dStr = QDate::currentDate().toString("dd/MM/yyyy");
            out << "=== BAO CAO BAC SI " << acc.username
                << " NGAY " << dStr << " ===\n";

            int ap = 0, hv = 0;
            for (const auto &a : gAppointments)
                if (a.date == dStr && a.doctor == acc.username) ++ap;
            for (const auto &v : gVisits)
                if (v.date == dStr && v.doctor == acc.username) ++hv;

            out << "So lich hen cua bac si: " << ap << "\n";
            out << "So lan kham cua bac si: " << hv << "\n";
            f.close();
        }
        else
        {
            QMessageBox::information(this, "Báo cáo",
                                     "Patient không có chức năng xuất báo cáo.");
            return;
        }

        QMessageBox::information(this, "Báo cáo", "Đã xuất: " + path);
    }

    // ---- mở các page ----
    void openPatients()
    {
        if (!(acc.role == Role::ADMIN || acc.role == Role::DOCTOR))
        {
            QMessageBox::warning(this, "Quyền", "Bạn không có quyền.");
            return;
        }
        if (!patientsPage)
        {
            patientsPage = new PatientsPage([this](){ stack->setCurrentWidget(homePage); });
            stack->addWidget(patientsPage);
        }
        stack->setCurrentWidget(patientsPage);
    }

    void openAppointments()
    {
        if (!apptsPage)
        {
            apptsPage = new ApptsPage(acc, [this](){ stack->setCurrentWidget(homePage); });
            stack->addWidget(apptsPage);
        }
        stack->setCurrentWidget(apptsPage);
    }

    void openInvoices()
    {
        if (!invoicesPage)
        {
            invoicesPage = new InvoicesPage(acc, [this](){ stack->setCurrentWidget(homePage); });
            stack->addWidget(invoicesPage);
        }
        stack->setCurrentWidget(invoicesPage);
    }

    void openVisits()
    {
        if (!visitsPage)
        {
            visitsPage = new VisitsPage(acc, [this](){ stack->setCurrentWidget(homePage); });
            stack->addWidget(visitsPage);
        }
        stack->setCurrentWidget(visitsPage);
    }

    void openDoctors()
    {
        if (acc.role != Role::ADMIN)
        {
            QMessageBox::warning(this, "Quyền", "Chỉ Admin dùng được mục này.");
            return;
        }
        if (!doctorsPage)
        {
            doctorsPage = new DoctorsPage([this](){ stack->setCurrentWidget(homePage); });
            stack->addWidget(doctorsPage);
        }
        doctorsPage->refreshTable();
        stack->setCurrentWidget(doctorsPage);
    }
};

// ====================== main() ======================

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    applyTheme(app);

    // Load dữ liệu từ CSV
    loadPatientsCSV();
    loadAppointmentsCSV();
    loadInvoicesCSV();
    loadVisitsCSV();
    ensureAccounts();

    // Vòng lặp: đăng nhập → mở main window
    while (true)
    {
        gLogoutRequested = false;

        LoginDialog loginDlg;
        if (loginDlg.exec() != QDialog::Accepted || !loginDlg.loginOK)
            break;  // người dùng bấm Thoát

        MainWindow w(loginDlg.loggedAccount);
        w.show();
        app.exec();

        if (!gLogoutRequested)
            break;  // nếu không phải do bấm Đăng xuất, thoát luôn
    }

    return 0;
}
