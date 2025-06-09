#include <iostream>
#include <string>
#include <limits>
#include <iomanip> // Untuk formatting tampilan
using namespace std;

// Fungsi untuk membersihkan layar (cross-platform)
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Fungsi untuk menampilkan header
void tampilkanHeader(const string& judul) {
    clearScreen();
    cout << "========================================\n";
    cout << "||         ATM SEDERHANA BANK        ||\n";
    cout << "========================================\n";
    cout << "|| " << left << setw(36) << judul << " ||\n";
    cout << "========================================\n";
}

// Fungsi untuk "enkripsi" PIN menggunakan XOR
string enkripsiPIN(const string& pin, char kunci = 'K') {
    string terenkripsi = pin;
    for (char& c : terenkripsi) {
        c ^= kunci; // XOR tiap karakter
    }
    return terenkripsi;
}

// Struktur data untuk akun pengguna
struct Akun {
    string nama;
    string nomorRekening;
    string pinTerenkripsi;
    double saldo;
    Akun* berikutnya;
};

Akun* kepala = nullptr; // Head dari linked list

// Fungsi untuk mencari akun berdasarkan nomor rekening
Akun* cariAkun(const string& nomorRekening) {
    Akun* saatIni = kepala;
    while (saatIni != nullptr) {
        if (saatIni->nomorRekening == nomorRekening) {
            return saatIni;
        }
        saatIni = saatIni->berikutnya;
    }
    return nullptr;
}

// Fungsi untuk registrasi pengguna baru
void daftarAkun() {
    tampilkanHeader("PENDAFTARAN AKUN BARU");

    Akun* akunBaru = new Akun;

    cout << "Masukkan nama lengkap: ";
    getline(cin >> ws, akunBaru->nama);

    cout << "Masukkan nomor rekening (6 digit): ";
    cin >> akunBaru->nomorRekening;

    // Validasi nomor rekening
    if (akunBaru->nomorRekening.length() != 6) {
        cout << "\n[ERROR] Nomor rekening harus 6 digit!\n";
        cout << "Tekan enter untuk kembali...";
        cin.ignore();
        cin.get();
        delete akunBaru;
        return;
    }

    if (cariAkun(akunBaru->nomorRekening) != nullptr) {
        cout << "\n[ERROR] Nomor rekening sudah terdaftar!\n";
        cout << "Tekan enter untuk kembali...";
        cin.ignore();
        cin.get();
        delete akunBaru;
        return;
    }

    string pin;
    cout << "Masukkan PIN (4 digit): ";
    cin >> pin;

    // Validasi PIN
    if (pin.length() != 4) {
        cout << "\n[ERROR] PIN harus 4 digit!\n";
        cout << "Tekan enter untuk kembali...";
        cin.ignore();
        cin.get();
        delete akunBaru;
        return;
    }

    akunBaru->pinTerenkripsi = enkripsiPIN(pin);

    cout << "Masukkan saldo awal: Rp ";
    cin >> akunBaru->saldo;

    // Validasi saldo
    if (akunBaru->saldo < 0) {
        cout << "\n[ERROR] Saldo tidak boleh negatif!\n";
        cout << "Tekan enter untuk kembali...";
        cin.ignore();
        cin.get();
        delete akunBaru;
        return;
    }

    akunBaru->berikutnya = kepala;
    kepala = akunBaru;

    cout << "\n========================================\n";
    cout << " [SUKSES] Akun berhasil terdaftar!\n";
    cout << "========================================\n";
    cout << " Nama          : " << akunBaru->nama << endl;
    cout << " No. Rekening  : " << akunBaru->nomorRekening << endl;
    cout << " Saldo Awal    : Rp " << fixed << setprecision(2) << akunBaru->saldo << endl;
    cout << "========================================\n";
    cout << "Tekan enter untuk kembali ke menu utama...";
    cin.ignore();
    cin.get();
}

// Fungsi untuk login pengguna
Akun* masuk() {
    tampilkanHeader("LOGIN AKUN");

    string nomorRekening, pin;
    cout << "Masukkan nomor rekening: ";
    cin >> nomorRekening;
    cout << "Masukkan PIN: ";
    cin >> pin;

    Akun* pengguna = cariAkun(nomorRekening);
    if (pengguna != nullptr && pengguna->pinTerenkripsi == enkripsiPIN(pin)) {
        cout << "\n========================================\n";
        cout << " [SUKSES] Login berhasil!\n";
        cout << " Selamat datang, " << pengguna->nama << "!\n";
        cout << "========================================\n";
        cout << "Tekan enter untuk melanjutkan...";
        cin.ignore();
        cin.get();
        return pengguna;
    } else {
        cout << "\n========================================\n";
        cout << " [GAGAL] Login gagal!\n";
        cout << " Nomor rekening atau PIN salah.\n";
        cout << "========================================\n";
        cout << "Tekan enter untuk kembali...";
        cin.ignore();
        cin.get();
        return nullptr;
    }
}

// Fungsi untuk menarik saldo
void tarikSaldo(Akun* pengguna) {
    tampilkanHeader("PENARIKAN SALDO");

    cout << " Saldo saat ini: Rp " << fixed << setprecision(2) << pengguna->saldo << endl;
    cout << "========================================\n";

    double jumlah;
    cout << "Masukkan jumlah yang ingin ditarik: Rp ";
    cin >> jumlah;

    if (jumlah <= 0) {
        cout << "\n[ERROR] Jumlah penarikan harus positif!\n";
    } else if (jumlah > pengguna->saldo) {
        cout << "\n[ERROR] Saldo tidak mencukupi!\n";
    } else {
        pengguna->saldo -= jumlah;
        cout << "\n========================================\n";
        cout << " [SUKSES] Penarikan berhasil!\n";
        cout << " Jumlah ditarik : Rp " << fixed << setprecision(2) << jumlah << endl;
        cout << " Sisa saldo     : Rp " << fixed << setprecision(2) << pengguna->saldo << endl;
    }

    cout << "========================================\n";
    cout << "Tekan enter untuk kembali...";
    cin.ignore();
    cin.get();
}

// Fungsi untuk setor saldo
void setorSaldo(Akun* pengguna) {
    tampilkanHeader("SETORAN SALDO");

    cout << " Saldo saat ini: Rp " << fixed << setprecision(2) << pengguna->saldo << endl;
    cout << "========================================\n";

    double jumlah;
    cout << "Masukkan jumlah yang ingin disetor: Rp ";
    cin >> jumlah;

    if (jumlah <= 0) {
        cout << "\n[ERROR] Jumlah setoran harus positif!\n";
    } else {
        pengguna->saldo += jumlah;
        cout << "\n========================================\n";
        cout << " [SUKSES] Setoran berhasil!\n";
        cout << " Jumlah disetor : Rp " << fixed << setprecision(2) << jumlah << endl;
        cout << " Saldo sekarang : Rp " << fixed << setprecision(2) << pengguna->saldo << endl;
    }

    cout << "========================================\n";
    cout << "Tekan enter untuk kembali...";
    cin.ignore();
    cin.get();
}

// Fungsi untuk transfer saldo
void transferSaldo(Akun* pengguna) {
    tampilkanHeader("TRANSFER SALDO");

    cout << " Saldo Anda: Rp " << fixed << setprecision(2) << pengguna->saldo << endl;
    cout << "========================================\n";

    string nomorRekeningTujuan;
    double jumlah;

    cout << "Masukkan nomor rekening tujuan: ";
    cin >> nomorRekeningTujuan;

    // Cek apakah rekening tujuan sama dengan pengirim
    if (nomorRekeningTujuan == pengguna->nomorRekening) {
        cout << "\n[ERROR] Tidak dapat transfer ke rekening sendiri!\n";
        cout << "========================================\n";
        cout << "Tekan enter untuk kembali...";
        cin.ignore();
        cin.get();
        return;
    }

    Akun* tujuan = cariAkun(nomorRekeningTujuan);
    if (tujuan == nullptr) {
        cout << "\n[ERROR] Rekening tujuan tidak ditemukan!\n";
        cout << "========================================\n";
        cout << "Tekan enter untuk kembali...";
        cin.ignore();
        cin.get();
        return;
    }

    cout << "Atas nama: " << tujuan->nama << endl;
    cout << "Masukkan jumlah yang ingin ditransfer: Rp ";
    cin >> jumlah;

    if (jumlah <= 0) {
        cout << "\n[ERROR] Jumlah transfer harus positif!\n";
    } else if (jumlah > pengguna->saldo) {
        cout << "\n[ERROR] Saldo tidak mencukupi untuk transfer!\n";
    } else {
        pengguna->saldo -= jumlah;
        tujuan->saldo += jumlah;
        cout << "\n========================================\n";
        cout << " [SUKSES] Transfer berhasil!\n";
        cout << " Kepada       : " << tujuan->nama << endl;
        cout << " No. Rekening : " << tujuan->nomorRekening << endl;
        cout << " Jumlah       : Rp " << fixed << setprecision(2) << jumlah << endl;
        cout << " Sisa saldo   : Rp " << fixed << setprecision(2) << pengguna->saldo << endl;
    }

    cout << "========================================\n";
    cout << "Tekan enter untuk kembali...";
    cin.ignore();
    cin.get();
}

// Fungsi untuk cek saldo
void cekSaldo(Akun* pengguna) {
    tampilkanHeader("CEK SALDO");

    cout << " Nama          : " << pengguna->nama << endl;
    cout << " No. Rekening  : " << pengguna->nomorRekening << endl;
    cout << " Saldo         : Rp " << fixed << setprecision(2) << pengguna->saldo << endl;

    cout << "========================================\n";
    cout << "Tekan enter untuk kembali...";
    cin.ignore();
    cin.get();
}

// Menu utama setelah login
void menuPengguna(Akun* pengguna) {
    int pilihan;
    do {
        tampilkanHeader("MENU UTAMA");

        cout << " 1. Tarik Saldo\n";
        cout << " 2. Setor Saldo\n";
        cout << " 3. Transfer Saldo\n";
        cout << " 4. Cek Saldo\n";
        cout << " 5. Logout\n";
        cout << "========================================\n";
        cout << "Pilihan Anda [1-5]: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: tarikSaldo(pengguna); break;
            case 2: setorSaldo(pengguna); break;
            case 3: transferSaldo(pengguna); break;
            case 4: cekSaldo(pengguna); break;
            case 5:
                tampilkanHeader("LOGOUT");
                cout << " Terima kasih telah menggunakan layanan kami.\n";
                cout << "========================================\n";
                cout << "Tekan enter untuk kembali ke menu utama...";
                cin.ignore();
                cin.get();
                break;
            default:
                cout << "\n[ERROR] Pilihan tidak valid!\n";
                cout << "Tekan enter untuk mencoba lagi...";
                cin.ignore();
                cin.get();
        }
    } while (pilihan != 5);
}

// Program utama
int main() {
    int pilihan;
    do {
        tampilkanHeader("SELAMAT DATANG");

        cout << " 1. Daftar Akun Baru\n";
        cout << " 2. Login\n";
        cout << " 3. Keluar\n";
        cout << "========================================\n";
        cout << "Pilihan Anda [1-3]: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                daftarAkun();
                break;
            case 2: {
                Akun* pengguna = masuk();
                if (pengguna != nullptr) {
                    menuPengguna(pengguna);
                }
                break;
            }
            case 3:
                tampilkanHeader("TERIMA KASIH");
                cout << " Terima kasih telah menggunakan ATM Sederhana Bank.\n";
                cout << "========================================\n";
                break;
            default:
                cout << "\n[ERROR] Pilihan tidak valid!\n";
                cout << "Tekan enter untuk mencoba lagi...";
                cin.ignore();
                cin.get();
        }
    } while (pilihan != 3);

    // Membersihkan memory
    while (kepala != nullptr) {
        Akun* temp = kepala;
        kepala = kepala->berikutnya;
        delete temp;
    }

    return 0;
}
