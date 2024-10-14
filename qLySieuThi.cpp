//chương trình quản lý siêu thị sử dụng hướng đối tượng 


#include <iostream>
#include <string>
#include <vector>
#include<iomanip>
using namespace std;

class sanPham {
private:
	string ten;
	string maSP;
	double gia;
	int soLuong;
public:
	sanPham(string ten, string maSP, double gia, int soLuong) {
		this->ten = ten;
		this->maSP = maSP;
		this->gia = gia;
		this->soLuong = soLuong;
	}
	string getName() {
		return this->ten;
	}
	string getMaSP() {
		return this->maSP;
	}
	double getGia() {
		return this->gia;
	}
	int getSoLuong() {
		return this->soLuong;
	}
	void setTen(string ten) {
		this->ten = ten;
	}
	void setMaSP(string maSP){
		this->maSP = maSP;
	}
	void setGia(double gia) {
		this->gia = gia;
	}
	void setSoLuong(int soLuong) {
		this->soLuong = soLuong;
	}
};
class khachHang {
private:
	string ten;
	string diaChi;
	string sdt;
public:
	khachHang() {
		ten = diaChi = sdt = " ";
	}
	khachHang(string ten, string diaChi, string sdt) {
		this->ten = ten;
		this->diaChi = diaChi;
		this->sdt = sdt;
	}
	string getTen() {
		return this->ten;
	}
	string getDiaChi() {
		return this->diaChi;
	}
	string getSdt() {
		return this->sdt;
	}
	void setTen(string ten) {
		this->ten = ten;
	}
	void setDiaChi(string diaChi) {
		this->diaChi = diaChi;
	}
	void setSdt(string sdt) {
		this->sdt = sdt;
	}
};
class nhanVien {
private:
	string tenNV;
	string diaChi;
	string sdt;
	string ngayVaoLam;
public:
	nhanVien() {
		tenNV = diaChi = sdt = ngayVaoLam = " ";
	}
	nhanVien(string tenNV, string diaChi, string sdt, string ngayVaoLam) {
		this->tenNV = tenNV;
		this->diaChi = diaChi;
		this->sdt = sdt;
		this->ngayVaoLam = ngayVaoLam;
	}
	string getTenNV() {
		return this->tenNV;
	}
	string getDiaChi() {
		return this->diaChi;
	}
	string getSdt() {
		return this->sdt;
	}
	string getNgayVaoLam() {
		return this->ngayVaoLam;
	}
	void setTen(string name) {
		this->tenNV = name;
	}
	void setDiaChi(string dc) {
		this->tenNV = dc;
	}
	void setSdt(string sdt) {
		this->tenNV = sdt;
	}
	void setNgayVaoLam(string nvl) {
		this->tenNV = nvl;
	}
};
class hoaDon {
private:
	string ngay;
	khachHang kh;
	nhanVien nv;
	vector<sanPham> sp;
public:
	hoaDon(string ngay, khachHang kh,nhanVien nv) {
		this->ngay = ngay;
		this->kh = kh;
		this->nv = nv;
	}
	void themSanPham(sanPham p) {
		sp.push_back(p);
	}
	double getTongTien() {
		double tt = 0;
		for (int i = 0; i < sp.size(); i++) {
			tt = tt + sp[i].getGia() * sp[i].getSoLuong();
		}
		return tt;
	}
	khachHang getKH() {
		return kh;
	}
	nhanVien  getNV() {
		return nv;
	}
	vector<sanPham> getSanPham() {
		return sp;
	}
};

class quanLy {
private:
	vector<sanPham> sp;
	vector<khachHang> kh;
	vector<hoaDon> hd;
	vector<nhanVien> nv;
public:
	void themSanPham() {
		string ten; string maSP; double gia; int soLuong;
		cout << "Nhap ten san pham: ";
		cin.ignore(100,'\n');
		getline(cin, ten);	
		cout << "Nhap ma san pham: ";
		cin >> maSP;
		cout << "Nhap gia  ban: ";
		cin >> gia;
		cout << "Nhap so luong san pham: ";
		cin >> soLuong;
		sanPham p(ten, maSP, gia, soLuong);
		sp.push_back(p);
		cout << "\nThem san pham thanh cong!!!\n";
	}
	void xoaSanPham() {
		int k;
		cout << "Nhap stt san pham can xoa: ";
		cin >> k;
		for (int i = 0; i < sp.size(); i++) {
			if (i == k-1) {
				sp.erase(sp.begin() + i);
			}
		}
	}
	void hienThiSanPham() {
		cout << setw(5)<<left << "STT" <<setw(15)<< "Ten san pham" << setw(15) << "Ma san pham"
			<< setw(15) << "Gia ban(VND)" << setw(10) << "So luong"<<endl;
		cout << setfill('-') << setw(53) << "-" << endl; // tạo đường kẻ ngang
		cout << setfill(' '); // đặt lại fill character là khoảng trắng
		for (int i = 0; i < sp.size(); i++) {
			cout<< setw(5)<<left<<i+1<<setw(15) <<sp[i].getName()<< setw(15) << sp[i].getMaSP()<< setw(15)
				<< sp[i].getGia()<< setw(10) << sp[i].getSoLuong() << endl;
		}
	}
	void timKiemSanPham() {
		string name;
		cout << "Nhap ten san pham can tim kiem: ";
		cin.ignore(100, '\n');
		getline(cin, name);
		bool tk = false;
		for (int i = 0; i < sp.size(); i++) {
			if (sp[i].getName() == name) {
				cout << "\nThong tin san phaam tim kiem\n\n";
				cout << setw(15) << left << "Ten san pham" << setw(15) << "Ma san pham"
					<< setw(15) << "Gia ban(VND)" << setw(10) << "So luong" << endl;
				cout << setfill('-') << setw(53) << "-" << endl;
				cout << setfill(' ');
				cout << setw(15) << left << sp[i].getName() << setw(15) << sp[i].getMaSP() << setw(15)
					<< sp[i].getGia() << setw(10) << sp[i].getSoLuong() << endl;
				tk = true;
				break;
			}
		}
		if (!tk)
			cout << "\nKhong tim thay san pham!!!\n";
	}
	void capNhatSanPham() {
		string name;
		cout << "Nhap ten san pham can cap nhat: ";
		cin.ignore(100, '\n');
		getline(cin, name);
		bool tk = false;
		for (int i = 0; i < sp.size(); i++) {
			if (sp[i].getName() == name) {
				string ma;
				double gia;
				int soLuong;
				cout << "Nhap gia moi: ";
				cin >> gia;
				cout << "Nhap so luong: ";
				cin >> soLuong;
				sp[i].setGia(gia);
				sp[i].setSoLuong(soLuong);
				cout << "Cap nhat thong tin san pham thanh cong!!";
				tk = true;
				break;
			}
		}
		if (!tk)
			cout << "Khong tim thay san pham!";
	}
	void themNhanVien() {
		string ten, diaChi, sdt, ngayVaoLam;
		cout << "Nhap ten nhan vien: ";
		cin.ignore(100, '\n');
		getline(cin, ten);
		cout << "Nhap dia chi nhan vien: ";;
		getline(cin, diaChi);
		cout << "Nhap sdt nhan vien: ";;
		cin >> sdt;
		cout << "Nhap ngay vao lam: ";
		cin >> ngayVaoLam;;
		nhanVien nv(ten, diaChi, sdt, ngayVaoLam);
		this->nv.push_back(nv);
		cout << "\nThem nhan vien thanh cong!!!\n";
	}
	void danhSachNhanVien() {
		cout << "Danh sach nhan vien!\n\n";
		cout << setw(20) << left << "Ten nhan vien" << setw(20) << "Dia chi" 
			<< setw(15) << "So dien thoai" << setw(15) << "Ngay vao lam" << endl;
		for (int i = 0; i < nv.size(); i++) {
			cout <<setw(20)<<left<< nv[i].getTenNV()<<setw(20)<< nv[i].getDiaChi() 
				<< setw(15)<< nv[i].getSdt() << setw(15) << nv[i].getNgayVaoLam() << endl;
		}
	}
	void ghiHoaDon() {
		vector<sanPham> sp2 = sp;
		nhanVien s;
		string tenKH, diaChi, sdt;
		string ngay;
		string tenNV;
		int i = 0;
		bool check = true;
		while (check) {
			cout << "Nhap ten nhan vien: ";
			cin.ignore(100, '\n');
			getline(cin, tenNV);
			int tam = 0;
			for (int i = 0; i < nv.size(); i++) {
				if (nv[i].getTenNV() == tenNV) {
					s = nv[i];
					tam = 1;
					break;
				}
			}
			if (tam == 1)
				break;
			cout << "Khong co thong tin nhan vien\n";
		}
		cout << "Nhap thong tin khach hang \n";
		cout << "Nhap ten khach hang: ";
		//cin.ignore(100, '\n');
		getline(cin, tenKH);
		cout << "Nhap dia chi khach hang: ";
		getline(cin, diaChi);
		cout << "Nhap so dien thoai: ";
		cin >> sdt;
		cout << "Nhap ngay tao hoa don: ";
		cin >> ngay;
		khachHang kh(tenKH, diaChi, sdt);
		this->kh.push_back(kh);
		hoaDon hd(ngay, kh,s);
		int tmp;
		do {
			cout << "San pham can them vao hoa don: \n";
			string name;
			int soLuong;
			bool tk = false;
			cout << "Nhap ten san pham can mua: ";
			cin.ignore(100, '\n');
			getline(cin, name);
			for (int i = 0; i < sp2.size(); i++) {
				if (sp2[i].getName() == name) {
					cout <<  "Nhap so luong can mua: ";
					cin >> soLuong;
					sp2[i].setSoLuong(soLuong);
					sp[i].setSoLuong(sp[i].getSoLuong() - soLuong);
					hd.themSanPham(sp2[i]);
					tk = true;
					break;					
				}
			}
			if (!tk)
				cout << "San pham khong ton tai!!";
			cout << "Chon 1 de tiep tuc mua hang hoac 0 de thoat!!! ";
			cin >> tmp;
		} while (tmp);
		this->hd.push_back(hd);
		cout << "Hoa don da tao: \n";
		cout << "Nhan vien ban: " << hd.getNV().getTenNV() << endl;
		cout << "Nguoi mua: " << hd.getKH().getTen() << endl;
		cout << "Cac mat hang da mua: \n";
		cout << setw(5) << left << "STT" << setw(15) << "Ten san pham" << setw(15) << "Ma san pham"
			<< setw(15) << "Gia ban(VND)" << setw(10) << "So luong" << endl;
		cout << setfill('-') << setw(53) << "-" << endl;
		cout << setfill(' '); // đặt lại fill character là khoảng trắng
		for (int i = 0; i < hd.getSanPham().size(); i++) {
			cout <<setw(5)<<left<<i+1 << setw(15) << hd.getSanPham()[i].getName() << setw(15) << hd.getSanPham()[i].getMaSP()
				<< setw(15) <<hd.getSanPham()[i].getGia() << setw(10) << hd.getSanPham()[i].getSoLuong() << endl;
		}
		double tt = hd.getTongTien();
		cout << "Tong tien: " << tt;
	}
	void timKiemKhachHang() {
		string name;
		cout << "Nhap ten khach hang can tim kiem: ";
		cin.ignore(100, '\n');
		getline(cin, name);
		for (int i = 0; i < hd.size(); i++) {
			if (hd[i].getKH().getTen() == name) {
				cout << "Hoa don khach da mua la: \n";
				cout << setw(15) << left << "Ten san pham" << setw(15) << "Ma san pham"
					<< setw(15) << "Gia ban(VND)" << setw(10) << "So luong" << endl;
				cout << setfill('-') << setw(53) << "-" << endl;
				cout << setfill(' ');
				for (int j = 0; j < hd[i].getSanPham().size(); j++) {
					cout <<setw(15)<<left<< hd[i].getSanPham()[j].getName() 
						<< setw(15) << hd[i].getSanPham()[j].getMaSP()
						<< setw(15) << hd[i].getSanPham()[j].getGia() 
						<< setw(10)<< hd[i].getSanPham()[j].getSoLuong() << endl;
				}
				cout << "\nTong tien: ";
				cout << hd[i].getTongTien();
			}
		}
	}
	void dangKyThanhVien() {
		string ten, diaChi, sdt;
		cout << "Nhap ten khach hang: ";
		cin.ignore(100, '\n');
		getline(cin, ten);
		cout << "Nhap dia chi khach hang: ";
		//cin.ignore();
		getline(cin, diaChi);
		cout << "Nhap so dien thoai khach hang: ";
		cin >> sdt;
		khachHang c(ten, diaChi, sdt);
		kh.push_back(c);		
	}
	void hienThiThanhVien() {
		cout << setw(5) << left << "STT" << setw(15) << "Ten khach hang" << setw(15) << "Dia chi"
			<< setw(10) << "So dien thoai" << endl;
		cout << setfill('-') << setw(53) << "-" << endl; 
		cout << setfill(' '); 
		for (int i = 0; i < kh.size(); i++) {
			cout <<setw(5)<<left<<i+1<<setw(15)<< kh[i].getTen() << setw(15) << kh[i].getDiaChi() 
				<< setw(10) << kh[i].getSdt() << endl;
		}
	}	
};

int main() {
	quanLy ql;
	int lc;
	do {
		cout << "\n------------------------------------\n";
		cout << "He thong quan ly sieu thi" << endl;
		cout << "1.Them san pham" << endl;
		cout << "2.Hien thi danh sach san pham" << endl;
		cout << "3.Tim kiem san pham" << endl;
		cout << "4.Cap nhat lai san pham" << endl;
		cout << "5.Xoa san pham" << endl;

		cout << "6.Them Nhan vien" << endl;
		cout << "7.Hien thi danh sach nhan vien" << endl;
		cout << "8.Tao hoa don ban hang" << endl;
		cout << "9.Tim kiem khach hang" << endl;
		cout << "10.Hien thi danh sach khach hang" << endl;	
		cout << "11.Thoat" << endl;
		cout << "\n------------------------------------\n";
		cout << "Nhap lua chon cua ban: ";
		cin >> lc;
		switch (lc) {
		case 1: {
			ql.themSanPham();
			break;
		}
		case 2: {
			ql.hienThiSanPham();
			break;
		}
		case 3: {
			ql.timKiemSanPham();
			break;
		}
		case 4: {
			ql.capNhatSanPham();
			break;
		}
		case 5: {
			ql.xoaSanPham();
			break;
		}
		case 6: {
			ql.themNhanVien();
			break;
		}
		case 7: {
			ql.danhSachNhanVien();
			break;
		}
		case 8: {
			ql.ghiHoaDon();
			break;
		}
		case 9: {
			ql.timKiemKhachHang();
			break;
		}
		case 10: {
			ql.hienThiThanhVien();
			break;
		}
		case 11: {
			break;
		}
		}
	} while (true);
	return 0;
}