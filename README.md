# Sistem Otomatisasi Pengelolaan Air Waduk Berbasis STM32

Sistem ini bekerja dengan membaca data dari sensor kelembaban tanah, sensor ketinggian air di dalam waduk, dan sensor ketinggian air di luar waduk. Berdasarkan data ini, sistem akan mengambil keputusan secara otomatis untuk membuka atau menutup gerbang air, serta mengaktifkan atau mematikan pompa air. Tujuannya adalah untuk mengelola aliran air secara efisien, menjaga kestabilan air waduk, serta mendukung irigasi lahan secara pintar dan responsif terhadap kondisi lingkungan.

Sistem ini dirancang untuk aplikasi seperti:
- Irigasi pintar
- Manajemen banjir
- Pengelolaan air otomatis

## 👨‍💻 Penulis

- Muhammad 'Azmilfadhil Syamsudin (2042231003)
- Muhammad Ali Makki (2042231023)
- Bagus WiJaksono (2042231039)
- Rivaldi Satrio Wicaksono (2042231043) 
  Teknik Instrumentasi - Institut Teknologi Sepuluh Nopember

---

## ✨ Fitur Utama

- Membuka gerbang otomatis jika ketinggian air di luar > air di dalam waduk
- Menutup gerbang jika level air sama
- Mengaktifkan pompa jika tanah kering dan jika air di waduk sama dengan diluar untuk repompa

---

## 🛠️ Kebutuhan Perangkat Keras

- Board STM32F4 (mis. STM32F401RE Nucleo)
- Sensor kelembaban tanah (analog)
- Sensor ketinggian air (analog – dalam dan luar waduk)
- Motor penggerak gerbang (dengan relay atau driver motor)
- Pompa air (dengan relay atau driver pompa)

---

## 🧩 Struktur Modul Program

### File Program:
- `main.c` – Fungsi utama dan loop program
- `SystemClock_Config()` – Inisialisasi clock sistem
- `GPIO_Init()` – Inisialisasi pin GPIO untuk motor & pompa
- `ADC_Init()` – Konfigurasi ADC untuk pembacaan sensor
- `Read_Soil_Moisture()` – Membaca kelembaban tanah
- `Read_Water_Level_Waduk()` – Membaca level air waduk
- `Read_Water_Level_Luar()` – Membaca level air luar
- `Control_Gate()` – Logika pengambilan keputusan otomatis

---

## 🚀 Cara Menjalankan Proyek Ini

### 1. Clone Repository
```bash
git clone https://github.com/namakamu/stm32-waduk-otomatis.git
cd stm32-waduk-otomatis
```

### 2. Buka di STM32CubeIDE
- Import folder ke STM32CubeIDE sebagai project STM32 yang sudah ada
- Compile program
- Upload ke board STM32 menggunakan ST-Link

### 3. Hubungkan Perangkat Keras
- PA0: Sensor kelembaban tanah
- PA1: Sensor level air (dalam waduk)
- PA2: Sensor level air (luar waduk)
- PA4: Motor buka gerbang
- PA5: Motor tutup gerbang
- PA6: Pompa air

### 4. Amati Perilaku Sistem
- Gerbang akan terbuka/tertutup otomatis berdasarkan level air
- Pompa menyala jika tanah kering dan waduk memiliki cukup air

---

## 🧠 Alur Program

### `main()`
Melakukan inisialisasi HAL, Clock, GPIO, dan ADC. Membaca nilai sensor tiap 5 detik dan memanggil `Control_Gate()`.

### `Read_Soil_Moisture()`
Mengakses ADC1 pada pin PA0 untuk membaca sensor kelembaban.

### `Read_Water_Level_Waduk()`
Mengakses ADC1 pada pin PA1 untuk membaca level air dalam waduk.

### `Read_Water_Level_Luar()`
Mengakses ADC2 pada pin PA2 untuk membaca level air luar.

### `Control_Gate()`
- Jika air luar > dalam → gerbang terbuka
- Jika sama → gerbang tertutup
- Jika tanah kering & air waduk cukup → pompa menyala

---

## 📝 Catatan Tambahan

- Threshold default: kelembaban tanah < 1000, level air waduk > 500
- Silakan sesuaikan nilai ambang berdasarkan kalibrasi sensor Anda
- Semua I/O menggunakan port GPIOA

---

## 🔭 Rencana Pengembangan

- Tambahkan komunikasi UART untuk debug data sensor
- Tambahkan layar LCD/OLED untuk tampilan real-time
- Integrasi RTC untuk penyiraman terjadwal
- Koneksikan ke cloud atau IoT dengan ESP32/LoRa

---

## ✅ Checklist Pengujian

- [x] Gerbang terbuka saat air luar lebih tinggi dari waduk
- [x] Gerbang tertutup saat level sama
- [x] Pompa menyala saat tanah kering dan air waduk mencukupi
- [x] Delay antar pembacaan sensor (5 detik)

---

## 📄 Lisensi

Lisensi MIT – Bebas digunakan dan dimodifikasi untuk kepentingan pembelajaran atau penelitian.

