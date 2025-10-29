#include "Clinic.h"
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
        cout << "9. Tim kiem benh nhan\n";       
        cout << "10. Sua thong tin benh nhan\n"; 
        cout << "11. Xoa benh nhan\n";           
        cout << "0. Thoat\n";
        cout << "Chon: ";

        int ch;
        if (!(cin >> ch)) {
            cout << "Lua chon khong hop le!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (ch) {
            case 1: c.addPatient(); break;
            case 2: c.listPatients(); break;
            case 3: c.createInvoice(); break;
            case 4: c.listInvoices(); break;
            case 5: c.showInvoiceDetail(); break;
            case 6: c.stats(); break;
            case 7: c.createAppointment(); break;
            case 8: c.listAppointments(); break;
            case 9: c.searchPatient(); break;     
            case 10: c.editPatient(); break;      
            case 11: c.deletePatient(); break;    
            case 0: cout << "Tam biet!\n"; return 0;
            default: cout << "Lua chon khong hop le!\n";
        }
    }
}