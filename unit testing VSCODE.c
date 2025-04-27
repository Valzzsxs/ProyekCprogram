#include <stdio.h>
#include <math.h>

#define MAX_TINGGI_CM                     100.0f       // Tinggi maksimum air
#define LEVEL_DIFF_OPEN_THRESHOLD         5.0f         // Selisih cm untuk membuka gerbang
#define LEVEL_DIFF_EQUAL_THRESHOLD        1.0f         // Toleransi dianggap seimbang

// Fungsi untuk membaca tinggi air dalam waduk (simulasi input manual)
float Read_Water_Level_Waduk_cm() {
    float tinggiWaduk;
    printf("Masukkan tinggi air dalam waduk (dalam cm): ");
    scanf("%f", &tinggiWaduk);
    return tinggiWaduk;
}

// Fungsi untuk membaca tinggi air luar waduk (simulasi input manual)
float Read_Water_Level_Luar_cm() {
    float tinggiLuar;
    printf("Masukkan tinggi air luar waduk (dalam cm): ");
    scanf("%f", &tinggiLuar);
    return tinggiLuar;
}

// Fungsi untuk menampilkan status relay
void Display_Relay_Status(int openGate, int closeGate, int pump) {
    // Menampilkan status relay gerbang dan pompa
    printf("Status Relay:\n");
    printf("  Gerbang Buka: %s\n", openGate ? "ON" : "OFF");
    printf("  Gerbang Tutup: %s\n", closeGate ? "ON" : "OFF");
    printf("  Pompa: %s\n", pump ? "ON" : "OFF");
    printf("--------------------------------\n");
}

// Fungsi utama untuk simulasi
int main(void) {
    while (1) {
        float tinggiWaduk = Read_Water_Level_Waduk_cm();
        float tinggiLuar  = Read_Water_Level_Luar_cm();

        int openGate = 0, closeGate = 0, pump = 0;

        // Validasi input
        if (tinggiWaduk >= 0.0f && tinggiLuar >= 0.0f) {
            // Logika normal saat sensor valid
            if (tinggiLuar > tinggiWaduk + LEVEL_DIFF_OPEN_THRESHOLD) {
                // Gerbang dibuka, pompa mati
                openGate = 1;  // Gerbang Buka
                closeGate = 0; // Gerbang Tutup
                pump = 0;      // Pompa mati
            }
            else if (fabs(tinggiLuar - tinggiWaduk) < LEVEL_DIFF_EQUAL_THRESHOLD) {
                // Gerbang ditutup, pompa menyala
                openGate = 0;  // Gerbang Buka
                closeGate = 1; // Gerbang Tutup
                pump = 1;      // Pompa menyala
            }
            else {
                // Gerbang ditutup, pompa mati
                openGate = 0;  // Gerbang Buka
                closeGate = 1; // Gerbang Tutup
                pump = 0;      // Pompa mati
            }

            // Tambahan logika untuk menyalakan pompa jika air luar sama dengan 50cm dan pompa menyala sampai 30cm
            if (tinggiLuar == 50.0f) {
                printf("Air luar 50cm, Gerbang ditutup, Pompa menyala\n");
                while (tinggiLuar > 30.0f) {
                    printf("Pompa menyala, Tinggi luar waduk: %.2f cm\n", tinggiLuar);
                    printf("Masukkan tinggi air luar waduk untuk simulasi berikutnya: ");
                    scanf("%f", &tinggiLuar);
                }
                printf("Tinggi luar waduk telah mencapai 30cm, Pompa mati\n");
                pump = 0; // Pompa mati setelah mencapai 30cm
            }
        } else {
            printf("Error: Input tidak valid\n");
        }

        // Menampilkan status relay
        Display_Relay_Status(openGate, closeGate, pump);

        // Memberikan waktu simulasi dengan penundaan (5 detik)
        printf("Tekan Enter untuk melanjutkan simulasi...\n");
        getchar();  // Untuk menangkap tombol Enter
        getchar();  // Untuk menunggu user menekan Enter
    }

    return 0;
}
