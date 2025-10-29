#include "Clinic.h"

bool Clinic::hasPatient(int pid) const {
    for (const auto& p : patients)
        if (p.getId() == pid) return true;
    return false;
}

/* 1 */ void Clinic::addPatient() {
    Patient p; p.input(nextPid++);
    patients.push_back(p);
    cout << "Da them benh nhan co ID: " << p.getId() << "!\n";
}

/* 2 */ void Clinic::listPatients() const {
    cout << "\n--- DANH SACH BENH NHAN ---\n";
    vector<int> W={6,20,14,18,12,12};
    vector<string> H={"ID","Ho ten","SDT","Benh ly","Ngay sinh","Lan kham"};
    drawSep(W); drawRow(W,H); drawSep(W);
    for (auto &p:patients) drawRow(W,p.toRow());
    drawSep(W);
    if(patients.empty()) cout<<"(Chua co benh nhan)\n";
}

/* 3 */ void Clinic::createInvoice() {
    int pid;
    cout<<"Nhap ID benh nhan: ";
    if(!(cin>>pid)){cout<<"ID khong hop le!\n";cin.clear();cin.ignore(999,'\n');return;}
    cin.ignore(999,'\n');
    if(!hasPatient(pid)){cout<<"Khong tim thay benh nhan!\n";return;}
    Invoice iv; iv.input(nextInv++,pid);
    invoices.push_back(iv);
    cout<<"Hoa don #"<<iv.getId()<<" da duoc tao!\n";
}

/* 4 */ void Clinic::listInvoices() const {
    cout<<"\n--- DANH SACH HOA DON ---\n";
    vector<int>W={8,10,12,16};
    vector<string>H={"ID HD","ID BN","Ngay","Tong tien"};
    drawSep(W); drawRow(W,H); drawSep(W);
    for(auto&i:invoices){
        vector<string>r;stringstream ss;
        ss<<i.getId();r.push_back(ss.str());ss.str("");ss.clear();
        ss<<i.getPid();r.push_back(ss.str());ss.str("");ss.clear();
        r.push_back(i.getDate().str());
        ss<<fixed<<setprecision(2)<<i.total()<<" VND";r.push_back(ss.str());
        drawRow(W,r);
    }
    drawSep(W);
    if(invoices.empty()) cout<<"(Chua co hoa don)\n";
}

/* 5 */ void Clinic::showInvoiceDetail() const {
    int id;
    cout<<"Nhap ID hoa don: ";
    if(!(cin>>id)){cout<<"ID khong hop le!\n";cin.clear();cin.ignore(999,'\n');return;}
    cin.ignore(999,'\n');
    for(auto&i:invoices)
        if(i.getId()==id){i.display();return;}
    cout<<"Khong tim thay hoa don!\n";
}

/* 6 */ void Clinic::stats() const {
    double sum=0;for(auto&i:invoices)sum+=i.total();
    cout<<"\n--- THONG KE ---\n";
    vector<int>W={20,12};vector<string>H={"Chi muc","Gia tri"};
    drawSep(W);drawRow(W,H);drawSep(W);
    vector<string>r1={"Tong benh nhan",to_string(patients.size())};drawRow(W,r1);
    vector<string>r2={"Tong hoa don",to_string(invoices.size())};drawRow(W,r2);
    stringstream ss;ss<<fixed<<setprecision(2)<<sum<<" VND";
    vector<string>r3={"Tong doanh thu",ss.str()};drawRow(W,r3);
    vector<string>r4={"Tong lich kham",to_string(appointments.size())};drawRow(W,r4);
    drawSep(W);
}

/* 7 */ void Clinic::createAppointment() {
    int pid;
    cout << "Nhap ID benh nhan can dat lich: ";
    if (!(cin >> pid)) {
        cout << "ID khong hop le!\n";
        cin.clear();
        cin.ignore(999, '\n');
        return;
    }
    cin.ignore(999, '\n');

    // Nếu chưa có bệnh nhân nào hoặc không tìm thấy ID
    if (patients.empty() || !hasPatient(pid)) {
        cout << "Khong tim thay benh nhan co ID " << pid << ".\n";
        cout << "Ban co muon them benh nhan moi va dat lich kham luon khong (y/n)? ";
        char c; cin >> c; cin.ignore(999, '\n');
        if (c == 'y' || c == 'Y') {
            addPatient();
            pid = nextPid - 1; // ID của bệnh nhân mới
            cout << "Dat lich kham cho benh nhan moi co ID " << pid << "...\n";
        } else {
            cout << "Huy thao tac.\n";
            return;
        }
    }

    // Lấy tên bệnh nhân để hiển thị trong form nhập lịch
    string name = "";
    for (auto &p : patients)
        if (p.getId() == pid) { name = p.getName(); break; }

    Appointment ap;
    ap.input(nextAppt++, pid, name);  // truyền thêm tên vào
    appointments.push_back(ap);
    cout << "✅ Da dat lich #" << (nextAppt - 1)
         << " cho benh nhan " << name << " thanh cong!\n";
}


/* 8 */ void Clinic::listAppointments() {
    cout<<"\n--- DANH SACH LICH KHAM ---\n";
    Date today=Date::today();Date limit=addMonths(today,3);
    vector<Appointment>filtered;
    for(auto&a:appointments){
        const Date&d=a.getDate();
        if(cmpDate(d,today)>=0&&cmpDate(d,limit)<=0)
            filtered.push_back(a);
    }
    if(filtered.empty()){cout<<"(Khong co lich hen trong 3 thang toi)\n";return;}
    sort(filtered.begin(),filtered.end());
    vector<int>W={8,10,12,8,30};
    vector<string>H={"ID LK","ID BN","Ngay","Gio","Ghi chu"};
    drawSep(W);drawRow(W,H);drawSep(W);
    for(auto&a:filtered)drawRow(W,a.toRow());
    drawSep(W);
    cout<<"Hien thi tu "<<today.str()<<" den "<<limit.str()<<".\n";
}

/* 9 */ void Clinic::searchPatient() const {
    if(patients.empty()){cout<<"Chua co benh nhan.\n";return;}
    cout<<"Nhap tu khoa (ID hoac ten): ";
    string key;getline(cin>>ws,key);
    vector<Patient>res;bool isNum=!key.empty()&&all_of(key.begin(),key.end(),::isdigit);
    for(auto&p:patients){
        stringstream ss;ss<<p.getId();
        if((isNum&&ss.str()==key)||(!isNum&&p.getName().find(key)!=string::npos))
            res.push_back(p);
    }
    if(res.empty()){cout<<"Khong tim thay.\n";return;}
    vector<int>W={6,20,14,18,12,12};
    vector<string>H={"ID","Ho ten","SDT","Benh ly","Ngay sinh","Lan kham"};
    drawSep(W);drawRow(W,H);drawSep(W);
    for(auto&p:res)drawRow(W,p.toRow());
    drawSep(W);
}

/* 10 */ void Clinic::editPatient() {
    if(patients.empty()){cout<<"Chua co benh nhan.\n";return;}
    int pid;cout<<"Nhap ID benh nhan can sua: ";
    if(!(cin>>pid)){cout<<"ID khong hop le!\n";cin.clear();cin.ignore(999,'\n');return;}
    cin.ignore(999,'\n');
    for(auto&p:patients){
        if(p.getId()==pid){
            cout<<"Sua thong tin cho BN "<<pid<<" (Enter de bo qua):\n";
            string raw;bool ok;
            cout<<"Ho ten moi ("<<p.getName()<<"): ";getline(cin,raw);
            if(!raw.empty()){string nm=normalizeName(raw,ok);if(ok)p.setName(nm);else cout<<"Ten khong hop le.\n";}
            cout<<"So dien thoai moi ("<<p.getPhone()<<"): ";getline(cin,raw);
            if(!raw.empty()){if(validatePhoneStrict(raw))p.setPhone(raw);else cout<<"SDT khong hop le.\n";}
            cout<<"Benh ly moi ("<<p.getDisease()<<"): ";getline(cin,raw);
            if(!raw.empty())p.setDisease(raw);
            cout<<"Da cap nhat!\n";return;
        }
    }
    cout<<"Khong tim thay benh nhan!\n";
}

/* 11 */ void Clinic::deletePatient() {
    if(patients.empty()){cout<<"Chua co benh nhan.\n";return;}
    int pid;cout<<"Nhap ID benh nhan can xoa: ";
    if(!(cin>>pid)){cout<<"ID khong hop le!\n";cin.clear();cin.ignore(999,'\n');return;}
    cin.ignore(999,'\n');
    for(size_t i=0;i<patients.size();++i){
        if(patients[i].getId()==pid){
            cout<<"Ban co chac muon xoa (y/n)? ";char c;cin>>c;cin.ignore(999,'\n');
            if(c=='y'||c=='Y'){patients.erase(patients.begin()+i);cout<<"Da xoa!\n";}else cout<<"Huy thao tac.\n";
            return;
        }
    }
    cout<<"Khong tim thay benh nhan!\n";
}
