# Embeded System Pada SIstem Irigasi Persawahan
![Alt text](https://github.com/Valzzsxs/ProyekCprogram/blob/main/waduk1.jpg)

Proyek ini bertujuan untuk mengontrol gerbang air dan pompa pada sebuah waduk menggunakan mikrokontroler STM32. Sistem ini mengandalkan dua sensor untuk memantau level air di dalam waduk dan di luar waduk. Berdasarkan pembacaan dari sensor tersebut, sistem akan mengontrol gerbang dan pompa secara otomatis untuk irigasi sawah. Sistem ini menggunakan 3 relay:
1. **Relay 1 (Buka Gerbang)**: Mengontrol motor untuk membuka gerbang air.
2. **Relay 2 (Tutup Gerbang)**: Mengontrol motor untuk menutup gerbang air.
3. **Relay 3 (Pompa)**: Mengontrol pompa untuk memasukkan air ke dalam waduk ketika level air di dalam dan di luar waduk hampir sama.

## Authors

- Muhammad 'Azmilfadhil S. (2042231003)
- Muhammad Ali Makki (2042231023)
- Bagus Wijaksono (2042231039)
- Rivaldi Satrio W. (2042231043)
- Ahmad Radhy (supervisor)

Teknik Instrumentasi - Institut Teknologi Sepuluh Nopember

## Daftar Isi
- [Gambaran Sistem](#gambaran-sistem)
- [Pengaturan Perangkat Keras](#pengaturan-perangkat-keras)
- [Logika Perangkat Lunak](#logika-perangkat-lunak)
- [Penjelasan Kode](#penjelasan-kode)
- [Cara Penggunaan](#cara-penggunaan)
- [Lisensi](#lisensi)

## Gambaran Sistem
Sistem ini terdiri dari:
1. **Sensor Level Air**: Mengukur level air di dalam waduk (`sensor 1`) dan di luar waduk (`sensor 2`).
2. **Motor dan Relay**: Motor digunakan untuk mengontrol pembukaan dan penutupan gerbang air. Dua relay digunakan untuk mengontrol arah motor.
3. **Pompa dan Relay**: Pompa digunakan untuk memasukkan air ke dalam waduk ketika level air di dalam dan di luar waduk hampir sama.

### Pin GPIO
- **Relay 1 (Buka Gerbang)**: PA4
- **Relay 2 (Tutup Gerbang)**: PA5
- **Relay 3 (Pompa)**: PA6
- **Sensor Level Air 1 (Dalam Waduk)**: ADC1 (PA1)
- **Sensor Level Air 2 (Luar Waduk)**: ADC2 (PA2)

## Pengaturan Perangkat Keras
- **STM32**: Mikrokontroler digunakan untuk menangani input ADC dari sensor level air dan mengontrol pin GPIO untuk operasi relay.
- **Relay 1**: Mengontrol motor untuk membuka gerbang (terhubung ke pin GPIO PA4).
- **Relay 2**: Mengontrol motor untuk menutup gerbang (terhubung ke pin GPIO PA5).
- **Relay 3**: Mengontrol pompa (terhubung ke pin GPIO PA6).
- **Sensor Level Air**: Sensor terhubung ke saluran ADC (misalnya, ADC1 untuk sensor dalam waduk dan ADC2 untuk sensor luar waduk).

## Logika Perangkat Lunak

1. **Membaca Data Sensor**: Pembacaan level air dilakukan dari dua sensor:
   - `Sensor 1`: Mengukur level air di dalam waduk.
   - `Sensor 2`: Mengukur level air di luar waduk.

2. **Membandingkan Level Air**:
   - Jika level air di luar lebih tinggi dari dalam lebih dari 5 cm, maka gerbang dibuka dengan mengaktifkan Relay 1 (Buka Gerbang) dan Relay 2 (Tutup Gerbang) dimatikan.
   - Jika level air hampir sama (perbedaan < 1 cm), maka gerbang ditutup dengan mengaktifkan Relay 2 (Tutup Gerbang) dan Relay 1 (Buka Gerbang) dimatikan, dan pompa dihidupkan.
   - Jika level air di luar lebih rendah dari dalam, maka gerbang ditutup dan pompa dimatikan.

3. **Logika Kontrol**: Status relay dikontrol dalam sebuah loop berkelanjutan dengan delay untuk memeriksa level air pada interval waktu tertentu.

## Penjelasan Kode

### Fungsi Utama

- **`Convert_ADC_to_cm()`**:
  Mengonversi nilai ADC dari sensor level air menjadi tinggi air dalam satuan sentimeter.

- **`Read_Water_Level_Waduk_cm()`**:
  Membaca level air di dalam waduk dengan memicu konversi ADC dan mendapatkan hasilnya.

- **`Read_Water_Level_Luar_cm()`**:
  Membaca level air di luar waduk, mirip dengan fungsi di atas.

- **Loop Utama**:
  Pada fungsi `main()`, level air diperiksa secara berkala. Berdasarkan perbandingan antara `tinggiWaduk` dan `tinggiLuar`, relay untuk motor dan pompa diaktifkan sesuai dengan logika yang telah ditentukan.

### Kontrol Pin GPIO

- **Relay 1 (Buka Gerbang)**: Mengaktifkan pin `PA4` untuk menghidupkan relay dan membuka gerbang.
- **Relay 2 (Tutup Gerbang)**: Mengaktifkan pin `PA5` untuk menghidupkan relay dan menutup gerbang.
- **Relay 3 (Pompa)**: Mengaktifkan pin `PA6` untuk menghidupkan pompa sesuai kebutuhan.

### Alur Kerja Contoh

1. **Air di luar lebih tinggi dari dalam lebih dari 5 cm**:
   - Buka gerbang (`Relay 1` ON, `Relay 2` OFF).
   - Matikan pompa.

2. **Level air hampir sama**:
   - Tutup gerbang (`Relay 1` OFF, `Relay 2` ON).
   - Hidupkan pompa (`Relay 3` ON).

3. **Air di luar lebih rendah dari dalam**:
   - Tutup gerbang (`Relay 1` OFF, `Relay 2` ON).
   - Matikan pompa.

## Cara Penggunaan

1. **Hubungkan Perangkat Keras**: Pastikan sensor level air terhubung ke pin ADC yang benar (ADC1 dan ADC2). Hubungkan relay ke pin GPIO yang ditentukan (PA4, PA5, PA6) untuk kontrol motor dan pompa.
2. **Kompilasi dan Upload**: Gunakan STM32CubeIDE atau alat lain yang sesuai untuk mengkompilasi dan mengupload kode ke mikrokontroler STM32.
3. **Pantau dan Uji**: Setelah diupload, sistem akan memeriksa level air secara berkelanjutan dan mengontrol gerbang dan pompa sesuai dengan logika yang telah ditentukan.

## Lisensi

Kelompok 7 Mata Kuliah Pemrograman Kontroller.
