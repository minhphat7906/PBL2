#include "Clinic.h"
#include <iostream>
using namespace std;

int main() {
    Clinic clinic;                 // Khởi tạo hệ thống phòng khám
    int linkedPatientId = -1;      // Lưu ID bệnh nhân nếu role là PATIENT
    Role role;

    while (true) {
        // Bước 1: Màn hình đăng nhập
        role = clinic.login(linkedPatientId);

        // Bước 2: Vào menu chính theo vai trò
        bool logout = clinic.mainMenu(role, linkedPatientId);

        // Nếu người dùng chọn "Thoát chương trình"
        if (!logout) {
            cout << "\nCam on da su dung he thong!\n";
            break;
        }

        // Nếu chọn "Đăng xuất" → quay lại vòng lặp login
    }

    return 0;
}
