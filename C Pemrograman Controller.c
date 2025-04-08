#include "stm32f4xx_hal.h"

/* GPIO Pin Macros */
#define SOIL_SENSOR_GPIO_PIN         GPIO_PIN_0U   /* Sensor Kelembaban Tanah (PA0) */
#define WATER_LEVEL_WADUK_GPIO_PIN   GPIO_PIN_1U   /* Sensor Level Air Waduk (PA1) */
#define WATER_LEVEL_LUAR_GPIO_PIN    GPIO_PIN_2U   /* Sensor Level Air Luar (PA2) */
#define MOTOR_OPEN_GPIO_PIN          GPIO_PIN_4U   /* Motor Membuka Gate (PA4) */
#define MOTOR_CLOSE_GPIO_PIN         GPIO_PIN_5U   /* Motor Menutup Gate (PA5) */
#define PUMP_GPIO_PIN                GPIO_PIN_6U   /* Pompa Air (PA6) */

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

/* Function Prototypes */
static void SystemClock_Config(void);
static void GPIO_Init(void);
static void ADC_Init(void);
static int Read_Soil_Moisture(void);
static int Read_Water_Level_Waduk(void);
static int Read_Water_Level_Luar(void);
static void Control_Gate(uint32_t soil, uint32_t waduk, uint32_t luar);

/* Fungsi Membaca Sensor */
int Read_Soil_Moisture(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10U);
    return (int)HAL_ADC_GetValue(&hadc1);
}

int Read_Water_Level_Waduk(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10U);
    return (int)HAL_ADC_GetValue(&hadc1);
}

int Read_Water_Level_Luar(void)
{
    HAL_ADC_Start(&hadc2);
    HAL_ADC_PollForConversion(&hadc2, 10U);
    return (int)HAL_ADC_GetValue(&hadc2);
}

/* Fungsi Kontrol Gerbang dan Pompa (versi gabungan) */
void Control_Gate(uint32_t soil, uint32_t waduk, uint32_t luar)
{
    if (luar > waduk)
    {
        // Air luar lebih tinggi → buka gerbang, pompa mati
        HAL_GPIO_WritePin(GPIOA, MOTOR_OPEN_GPIO_PIN, GPIO_PIN_SET);   // Motor buka
        HAL_GPIO_WritePin(GPIOA, MOTOR_CLOSE_GPIO_PIN, GPIO_PIN_RESET); // Motor tutup
        HAL_GPIO_WritePin(GPIOA, PUMP_GPIO_PIN, GPIO_PIN_RESET);       // Pompa mati
    }
    else if (waduk == luar)
    {
        // Air sama → gerbang ditutup, pompa aktif untuk alirkan dari luar ke dalam
        HAL_GPIO_WritePin(GPIOA, MOTOR_OPEN_GPIO_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, MOTOR_CLOSE_GPIO_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, PUMP_GPIO_PIN, GPIO_PIN_SET);         // Pompa ON
    }
    else
    {
        // Air luar lebih rendah dari dalam
        HAL_GPIO_WritePin(GPIOA, MOTOR_OPEN_GPIO_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, MOTOR_CLOSE_GPIO_PIN, GPIO_PIN_SET);

        if (soil < 1000 && waduk > 500)
        {
            HAL_GPIO_WritePin(GPIOA, PUMP_GPIO_PIN, GPIO_PIN_SET);     // Pompa ON (penyiraman)
        }
        else
        {
            HAL_GPIO_WritePin(GPIOA, PUMP_GPIO_PIN, GPIO_PIN_RESET);   // Pompa OFF
        }
    }
}

/* Fungsi Utama */
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();
    ADC_Init();

    while (1)
    {
        int soilMoisture    = Read_Soil_Moisture();
        int waterLevelWaduk = Read_Water_Level_Waduk();
        int waterLevelLuar  = Read_Water_Level_Luar();

        Control_Gate(soilMoisture, waterLevelWaduk, waterLevelLuar);

        HAL_Delay(5000U); // Delay 5 detik
    }
}

/* Inisialisasi Clock */
static void SystemClock_Config(void)
{
    /* Tambahkan konfigurasi sesuai kebutuhan board */
}

/* Inisialisasi GPIO */
static void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pin   = MOTOR_OPEN_GPIO_PIN | MOTOR_CLOSE_GPIO_PIN | PUMP_GPIO_PIN;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* Inisialisasi ADC */
static void ADC_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    __HAL_RCC_ADC1_CLK_ENABLE();
    __HAL_RCC_ADC2_CLK_ENABLE();

    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2;
    hadc1.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode          = ENABLE;
    hadc1.Init.ContinuousConvMode    = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion       = 1U;
    HAL_ADC_Init(&hadc1);

    sConfig.Channel      = ADC_CHANNEL_0; // Soil
    sConfig.Rank         = 1U;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    sConfig.Channel = ADC_CHANNEL_1; // Waduk
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    hadc2.Instance = ADC2;
    hadc2.Init     = hadc1.Init;
    HAL_ADC_Init(&hadc2);

    sConfig.Channel = ADC_CHANNEL_2; // Luar
    HAL_ADC_ConfigChannel(&hadc2, &sConfig);

    HAL_ADC_Start(&hadc1);
    HAL_ADC_Start(&hadc2);
}
