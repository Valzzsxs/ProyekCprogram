# Optimasi Distribusi Air Sawah melalui Sistem Irigasi Otomatis Berbasis Mikrokontroler STM32
![Alt text](https://github.com/Valzzsxs/ProyekCprogram/blob/main/waduk1.jpg)
## 📌 Deskripsi Proyek

Project ini merancang sistem irigasi otomatis berbasis mikrokontroler STM32 dengan sensor ketinggian air analog untuk mengatur pompa dan gerbang irigasi secara otomatis. Keputusan sistem didasarkan pada perbandingan ketinggian air antara waduk dan lahan sawah. Sistem mengenali tiga kondisi utama serta dilengkapi logika histeresis: pompa menyala saat air luar ≥50 cm dan mati saat ≤30 cm. Hasil simulasi menunjukkan respons real-time sesuai algoritma. Sistem ini berpotensi mendukung pertanian cerdas berbasis teknologi tertanam..  
Tujuan utamanya adalah:

- Membuka atau menutup **gerbang air** saat terjadi perbedaan tinggi air.
- Mengaktifkan **pompa air** saat kondisi tertentu terpenuhi.
- Menyediakan **mode kuras otomatis** saat kondisi ketinggian air tertentu terpenuhi.

Sistem ini cocok untuk proyek pengelolaan air irigasi, bendungan mini, atau sistem smart farming berbasis kontrol mikrokontroler.

---

## 🧠 Fitur Utama

- Pembacaan level air **dua titik** menggunakan ADC1 dan ADC2.
- Otomasi:
  - Buka/tutup **gerbang air otomatis** berdasarkan perbedaan ketinggian air.
  - Aktifkan **pompa air** saat level air seimbang atau saat mode kuras.
  - **Mode kuras otomatis**: menyedot air keluar jika tinggi air dalam dan luar 50 cm.
- Penanganan error: jika sensor gagal membaca, sistem masuk mode **fail-safe** (semua relay off).

---

## 🔧 Perangkat Keras yang Digunakan

- STM32Fxx (contoh: STM32F103C8T6 atau STM32F401)
- 2x Sensor Level Air (analog)
- 3x Relay Module (5V)
  - Relay 1: Buka gerbang air
  - Relay 2: Tutup gerbang air
  - Relay 3: Pompa air
- Catu daya 5V dan 3.3V
- Kabel dan breadboard (opsional)

---

## ⚙️ Pin Konfigurasi

| Perangkat       | Pin STM32 | Fungsi                  |
|-----------------|-----------|-------------------------|
| Sensor Waduk    | PA1 (ADC1)| Ketinggian air dalam    |
| Sensor Luar     | PA2 (ADC2)| Ketinggian air luar     |
| Relay Buka Gate | PA4       | Aktifkan relay buka     |
| Relay Tutup Gate| PA5       | Aktifkan relay tutup    |
| Relay Pompa     | PA6       | Aktifkan pompa air      |

---

## 🧪 Cara Kerja Sistem

1. **Sensor membaca** tinggi air dari dalam dan luar waduk melalui ADC.
2. Kode mengonversi nilai ADC ke satuan cm.
3. Logika berikut dieksekusi:
   - Jika tinggi air luar > dalam + 5 cm: buka gerbang.
   - Jika tinggi air luar ≈ dalam: tutup gerbang & nyalakan pompa.
   - Jika tinggi air luar < dalam: tutup gerbang & matikan pompa.
4. Jika tinggi air dalam dan luar **≈ 50 cm**, aktifkan mode **kuras otomatis**.
   - Pompa menyala sampai tinggi luar < 30 cm.
5. Bila pembacaan ADC gagal, **semua relay dimatikan** untuk keamanan.

---

## 🛠️ Cara Build dan Flash Kode

### 1. Instalasi Tools
- STM32CubeIDE **atau** VSCode + STM32CubeMX + arm-none-eabi toolchain
- ST-Link / USB-to-Serial Programmer

### 2. Konfigurasi STM32CubeMX (jika dari nol)
- Aktifkan ADC1 pada PA1 dan ADC2 pada PA2
- Aktifkan GPIO Output: PA4, PA5, PA6
- Clock: HSI Internal

### 3. Build Proyek
- Import kode `main.c` ini ke proyek STM32 Anda
- Tambahkan fungsi init (`MX_ADC1_Init`, dll) di `main.c`
- Build dan flash ke board STM32

### 4. Jalankan Sistem
- Sambungkan sensor dan relay sesuai tabel pin
- Nyalakan board, sistem akan otomatis berjalan sesuai kondisi air

---

## ✅ Tips Tambahan

- Pastikan sensor level air menghasilkan tegangan antara 0–3.3V agar terbaca oleh ADC.
- Gunakan multimeter untuk mengkalibrasi sensor sebelum digunakan.
- Tambahkan log serial (UART) untuk debugging saat pengujian awal.
- Pertimbangkan watchdog timer untuk meningkatkan keandalan sistem.

---

## 🧑‍💻 Sertifikasi dan Coding Standards

Kode ini telah mengikuti beberapa standar keamanan dan keandalan dari:
- **CERT C Secure Coding Standard**:
  - ERR33-C: Cek error dari fungsi HAL
  - EXP33-C: Validasi semua inisialisasi sistem
  - FLP34-C: Validasi nilai floating point
- **MISRA C (partial)** untuk manajemen kontrol dan validasi error

---

## 📄 Lisensi

Kode ini hanya sebagai contoh code embeded sistem irigasi -- Kelompok 7.

---

## ✍️ Penulis

- Muhammad 'Azmilfadhil S. (2042231003)
- Muhammad Ali Makki (2042231023)
- Bagus Wijaksono (2042231039)
- Rivaldi Satrio W. (2042231043)
- Ahmad Radhy (supervisor)

Teknik Instrumentasi - Institut Teknologi Sepuluh Nopember

