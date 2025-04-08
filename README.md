# 🛠️ STM32 Proyek Otomatisasi Gate Waduk dan Pompa Air

Proyek ini menggunakan **STM32F4** dan bahasa pemrograman **C (HAL Library)** untuk mengontrol **gate waduk dan pompa air** secara otomatis berdasarkan:
- Sensor kelembaban tanah
- Sensor ketinggian air waduk
- Sensor ketinggian air luar (misal sungai)

## 🚀 Fitur Utama
- Otomatis membuka gate saat air luar lebih tinggi dari waduk
- Menutup gate saat ketinggian setara
- Mengaktifkan pompa jika tanah kering & air waduk mencukupi

---

## ⚙️ Perangkat Keras yang Digunakan
- STM32F4xx board (misal: STM32F401RE, STM32F411)
- Sensor kelembaban tanah (analog output)
- Sensor level air (misal: sensor tekanan atau pelampung analog)
- Motor servo / DC untuk gate (diaktifkan via GPIO)
- Pompa air (diaktifkan via GPIO)

---

## 🧠 Struktur Modul Program
- `main.c` – Fungsi utama dan loop kontrol
- `GPIO_Init()` – Konfigurasi pin untuk motor & pompa
- `ADC_Init()` – Setup ADC untuk membaca sensor
- `Read_Soil_Moisture()` – Membaca kelembaban tanah
- `Read_Water_Level_Waduk()` – Membaca tinggi air waduk
- `Read_Water_Level_Luar()` – Membaca tinggi air luar
- `Control_Gate()` – Logika kontrol otomatis berdasarkan pembacaan sensor

---

## 📥 Cara Menggunakan
1. Clone repository:
   ```bash
   git clone https://github.com/namakamu/stm32-waduk-otomatis.git
   ```

2. Buka project ini di **STM32CubeIDE**.

3. Hubungkan sensor dan aktuator sesuai pin berikut:

| Fungsi               | Pin GPIO |
|----------------------|----------|
| Kelembaban Tanah     | PA0      |
| Level Air Waduk      | PA1      |
| Level Air Luar       | PA2      |
| Motor Buka Gate      | PA4      |
| Motor Tutup Gate     | PA5      |
| Pompa Air            | PA6      |

4. Build project lalu flash ke board STM32 kamu.

5. Jalankan dan monitor hasilnya secara fisik atau lewat debugger.

## 📄 Lisensi
Project ini dilisensikan di bawah MIT License.

