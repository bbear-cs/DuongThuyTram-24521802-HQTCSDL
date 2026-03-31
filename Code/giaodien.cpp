#include <windows.h>
#include <sstream>
vector<SinhVien> ds;
Node root = NULL;

HWND txtMSSV, txtHoTen, txtGioiTinh;
HWND btnThem, btnXoa, btnTimMSSV, btnTimHoTen, btnLamMoi;
HWND listBangGoc, txtIndex, lblTrangThai;

#define ID_TXT_MSSV    101
#define ID_TXT_HOTEN   102
#define ID_TXT_GIOITINH 103
#define ID_BTN_THEM    201
#define ID_BTN_XOA     202
#define ID_BTN_TIM_MSSV 203
#define ID_BTN_TIM_HOTEN 204
#define ID_BTN_LAMMOI  205
#define ID_LIST_BANGGOC 301
#define ID_TXT_INDEX   302
#define ID_LBL_STATUS  401

string LayText(HWND h) {
    int len = GetWindowTextLengthA(h);
    char *buf = new char[len + 1];
    GetWindowTextA(h, buf, len + 1);
    string s = buf;
    delete[] buf;
    return s;
}

void DatText(HWND h, string s) {
    SetWindowTextA(h, s.c_str());
}

void DatTrangThai(string s) {
    SetWindowTextA(lblTrangThai, s.c_str());
}

string NodeToString(Node p) {
    if(p == NULL) return "";
    string s = "[";
    for(int i = 0; i < (int)p->data.size(); i++) {
        s += to_string(p->data[i].mssv);
        if(i != (int)p->data.size() - 1) s += " ";
    }
    s += "]";
    return s;
}

void ThemKhoangTrang(string &s, int n) {
    for(int i = 0; i < n; i++) s += " ";
}

void CayToString(Node root, string &out, int kc = 8) {
    if(root == NULL) {
        out += "Cay rong.\r\n";
        return;
    }

    ThemKhoangTrang(out, kc);
    out += NodeToString(root);
    out += "\r\n";

    if(root->la) return;

    ThemKhoangTrang(out, kc - 2);

    if((int)root->con.size() == 2) {
        out += "/   \\";
    }
    else if((int)root->con.size() == 3) {
        out += "/   |   \\";
    }
    else {
        for(int i = 0; i < (int)root->con.size(); i++) out += "| ";
    }
    out += "\r\n";

    ThemKhoangTrang(out, kc - 4);
    for(int i = 0; i < (int)root->con.size(); i++) {
        out += NodeToString(root->con[i]);
        if(i != (int)root->con.size() - 1) out += " ";
    }
    out += "\r\n";

    for(int i = 0; i < (int)root->con.size(); i++) {
        if(root->con[i] != NULL && !root->con[i]->la) {
            out += "\r\n";
            ThemKhoangTrang(out, kc - 4 + i * 4);
            out += "Nhanh con thu ";
            out += to_string(i + 1);
            out += ":\r\n";
            CayToString(root->con[i], out, kc);
        }
    }
}

void CapNhatBangGoc() {
    SendMessageA(listBangGoc, LB_RESETCONTENT, 0, 0);
    if(ds.size() == 0) {
        SendMessageA(listBangGoc, LB_ADDSTRING, 0, (LPARAM)"Danh sach rong.");
        return;
    }

    for(int i = 0; i < (int)ds.size(); i++) {
        string dong = to_string(ds[i].mssv) + " | " + ds[i].hoten + " | " + ds[i].gioitinh;
        SendMessageA(listBangGoc, LB_ADDSTRING, 0, (LPARAM)dong.c_str());
    }
}

void CapNhatIndex() {
    string s;
    CayToString(root, s, 10);
    SetWindowTextA(txtIndex, s.c_str());
}

void CapNhatTatCa() {
    CapNhatBangGoc();
    CapNhatIndex();
}

void XoaNhapLieu() {
    DatText(txtMSSV, "");
    DatText(txtHoTen, "");
    DatText(txtGioiTinh, "");
}

void XuLyThem() {
    string smssv = LayText(txtMSSV);
    string hoten = LayText(txtHoTen);
    string gioitinh = LayText(txtGioiTinh);

    if(smssv == "" || hoten == "" || gioitinh == "") {
        DatTrangThai("Vui long nhap day du thong tin.");
        return;
    }

    int mssv = atoi(smssv.c_str());

    for(int i = 0; i < (int)ds.size(); i++) {
        if(ds[i].mssv == mssv) {
            DatTrangThai("MSSV da ton tai.");
            return;
        }
    }

    SinhVien sv;
    sv.mssv = mssv;
    sv.hoten = hoten;
    sv.gioitinh = gioitinh;
    ds.push_back(sv);

    Key a;
    a.mssv = sv.mssv;
    a.key = (int)ds.size() - 1;
    insert(root, a);

    CapNhatTatCa();
    DatTrangThai("Them sinh vien thanh cong.");
    XoaNhapLieu();
}

void XuLyXoa() {
    string smssv = LayText(txtMSSV);
    if(smssv == "") {
        DatTrangThai("Nhap MSSV de xoa.");
        return;
    }

    int mssv = atoi(smssv.c_str());

    if(xoaSV(ds, root, mssv)) {
        CapNhatTatCa();
        DatTrangThai("Xoa sinh vien thanh cong.");
        XoaNhapLieu();
    }
    else {
        DatTrangThai("Khong tim thay MSSV can xoa.");
    }
}

void XuLyTimMSSV() {
    string smssv = LayText(txtMSSV);
    if(smssv == "") {
        DatTrangThai("Nhap MSSV de tim.");
        return;
    }

    int mssv = atoi(smssv.c_str());
    int pos = find(root, mssv);

    if(pos == -1) {
        DatTrangThai("Khong tim thay sinh vien.");
    }
    else {
        string s = "Tim thay: " + to_string(ds[pos].mssv) + " - " + ds[pos].hoten + " - " + ds[pos].gioitinh;
        DatTrangThai(s);
    }
}

void XuLyTimHoTen() {
    string hoten = LayText(txtHoTen);
    if(hoten == "") {
        DatTrangThai("Nhap ho ten de tim.");
        return;
    }

    bool check = false;
    SendMessageA(listBangGoc, LB_RESETCONTENT, 0, 0);

    for(int i = 0; i < (int)ds.size(); i++) {
        if(ds[i].hoten == hoten) {
            string dong = to_string(ds[i].mssv) + " | " + ds[i].hoten + " | " + ds[i].gioitinh;
            SendMessageA(listBangGoc, LB_ADDSTRING, 0, (LPARAM)dong.c_str());
            check = true;
        }
    }

    if(check) DatTrangThai("Da loc theo ho ten.");
    else {
        SendMessageA(listBangGoc, LB_ADDSTRING, 0, (LPARAM)"Khong tim thay sinh vien.");
        DatTrangThai("Khong tim thay sinh vien.");
    }
}

void TaoControl(HWND hwnd) {
    CreateWindowA("STATIC", "UNG DUNG GIA LAP HE QTCSDL DON GIAN",
        WS_VISIBLE | WS_CHILD, 250, 20, 420, 25, hwnd, NULL, NULL, NULL);

    CreateWindowA("STATIC", "Ma SV:", WS_VISIBLE | WS_CHILD,
        40, 70, 80, 20, hwnd, NULL, NULL, NULL);
    txtMSSV = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
        120, 68, 180, 24, hwnd, (HMENU)ID_TXT_MSSV, NULL, NULL);

    CreateWindowA("STATIC", "Ho ten:", WS_VISIBLE | WS_CHILD,
        40, 105, 80, 20, hwnd, NULL, NULL, NULL);
    txtHoTen = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
        120, 103, 260, 24, hwnd, (HMENU)ID_TXT_HOTEN, NULL, NULL);

    CreateWindowA("STATIC", "Gioi tinh:", WS_VISIBLE | WS_CHILD,
        40, 140, 80, 20, hwnd, NULL, NULL, NULL);
    txtGioiTinh = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
        120, 138, 180, 24, hwnd, (HMENU)ID_TXT_GIOITINH, NULL, NULL);

    btnThem = CreateWindowA("BUTTON", "Them", WS_VISIBLE | WS_CHILD,
        420, 68, 100, 28, hwnd, (HMENU)ID_BTN_THEM, NULL, NULL);
    btnXoa = CreateWindowA("BUTTON", "Xoa", WS_VISIBLE | WS_CHILD,
        530, 68, 100, 28, hwnd, (HMENU)ID_BTN_XOA, NULL, NULL);
    btnTimMSSV = CreateWindowA("BUTTON", "Tim MSSV", WS_VISIBLE | WS_CHILD,
        420, 103, 100, 28, hwnd, (HMENU)ID_BTN_TIM_MSSV, NULL, NULL);
    btnTimHoTen = CreateWindowA("BUTTON", "Tim Ho ten", WS_VISIBLE | WS_CHILD,
        530, 103, 100, 28, hwnd, (HMENU)ID_BTN_TIM_HOTEN, NULL, NULL);
    btnLamMoi = CreateWindowA("BUTTON", "Lam moi", WS_VISIBLE | WS_CHILD,
        420, 138, 210, 28, hwnd, (HMENU)ID_BTN_LAMMOI, NULL, NULL);

    CreateWindowA("STATIC", "Bang goc hien tai", WS_VISIBLE | WS_CHILD,
        40, 190, 160, 20, hwnd, NULL, NULL, NULL);
    listBangGoc = CreateWindowA("LISTBOX", "", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
        40, 215, 360, 270, hwnd, (HMENU)ID_LIST_BANGGOC, NULL, NULL);

    CreateWindowA("STATIC", "Index B-Tree", WS_VISIBLE | WS_CHILD,
        430, 190, 120, 20, hwnd, NULL, NULL, NULL);
    txtIndex = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER |
        ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
        430, 215, 340, 270, hwnd, (HMENU)ID_TXT_INDEX, NULL, NULL);

    lblTrangThai = CreateWindowA("STATIC", "San sang.",
        WS_VISIBLE | WS_CHILD, 40, 510, 730, 24, hwnd, (HMENU)ID_LBL_STATUS, NULL, NULL);

    CapNhatTatCa();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE:
            TaoControl(hwnd);
            break;

        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case ID_BTN_THEM:
                    XuLyThem();
                    break;
                case ID_BTN_XOA:
                    XuLyXoa();
                    break;
                case ID_BTN_TIM_MSSV:
                    XuLyTimMSSV();
                    break;
                case ID_BTN_TIM_HOTEN:
                    XuLyTimHoTen();
                    break;
                case ID_BTN_LAMMOI:
                    CapNhatTatCa();
                    XoaNhapLieu();
                    DatTrangThai("Da lam moi hien thi.");
                    break;
            }
            break;

        case WM_DESTROY:
            xoa(root);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}