#include "stm32f4xx_hal.h"
#include <math.h>

/* GPIO Pin Definitions */
#define WATER_LEVEL_WADUK_GPIO_PIN        GPIO_PIN_1U  // PA1
#define WATER_LEVEL_LUAR_GPIO_PIN         GPIO_PIN_2U  // PA2

#define RELAY_OPEN_GATE_GPIO_PIN          GPIO_PIN_4U  // PA4: Buka Gerbang
#define RELAY_CLOSE_GATE_GPIO_PIN         GPIO_PIN_5U  // PA5: Tutup Gerbang
#define RELAY_PUMP_GPIO_PIN               GPIO_PIN_6U  // PA6: Pompa Air

#define MAX_TINGGI_CM                     100.0f       // Tinggi maksimum air
#define LEVEL_DIFF_OPEN_THRESHOLD         5.0f         // Selisih cm untuk membuka gerbang
#define LEVEL_DIFF_EQUAL_THRESHOLD        1.0f         // Toleransi dianggap seimbang

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

/* Function Prototypes */
static void SystemClock_Config(void);
static void GPIO_Init(void);
static void ADC_Init(void);
float Convert_ADC_to_cm(uint32_t adc_value);
float Read_Water_Level_Waduk_cm(void);
float Read_Water_Level_Luar_cm(void);

float Convert_ADC_to_cm(uint32_t adc_value)
{
    if (adc_value > 4095U)
    {
        return -1.0f;
    }
    return (adc_value / 4095.0f) * MAX_TINGGI_CM;
}

float Read_Water_Level_Waduk_cm(void)
{
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK)
    {
        return -1.0f;
    }

    uint32_t adc_value = HAL_ADC_GetValue(&hadc1);
    return Convert_ADC_to_cm(adc_value);
}

float Read_Water_Level_Luar_cm(void)
{
    HAL_ADC_Start(&hadc2);
    if (HAL_ADC_PollForConversion(&hadc2, 10) != HAL_OK)
    {
        return -1.0f;
    }

    uint32_t adc_value = HAL_ADC_GetValue(&hadc2);
    return Convert_ADC_to_cm(adc_value);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();
    ADC_Init();

    while (1)
    {
        float tinggiWaduk = Read_Water_Level_Waduk_cm();
        float tinggiLuar  = Read_Water_Level_Luar_cm();

        if (tinggiLuar > tinggiWaduk + LEVEL_DIFF_OPEN_THRESHOLD)
        {
            // Buka gerbang (motor arah buka), pompa mati
            HAL_GPIO_WritePin(GPIOA, RELAY_OPEN_GATE_GPIO_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, RELAY_CLOSE_GATE_GPIO_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, RELAY_PUMP_GPIO_PIN, GPIO_PIN_RESET);
        }
        else if (fabs(tinggiLuar - tinggiWaduk) < LEVEL_DIFF_EQUAL_THRESHOLD)
        {
            // Tutup gerbang (motor arah tutup), pompa hidup
            HAL_GPIO_WritePin(GPIOA, RELAY_OPEN_GATE_GPIO_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, RELAY_CLOSE_GATE_GPIO_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, RELAY_PUMP_GPIO_PIN, GPIO_PIN_SET);
        }
        else
        {
            // Tutup gerbang (motor arah tutup), pompa mati
            HAL_GPIO_WritePin(GPIOA, RELAY_OPEN_GATE_GPIO_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, RELAY_CLOSE_GATE_GPIO_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, RELAY_PUMP_GPIO_PIN, GPIO_PIN_RESET);
        }

        HAL_Delay(5000); // Delay 5 detik
    }
}

static void GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = RELAY_OPEN_GATE_GPIO_PIN | RELAY_CLOSE_GATE_GPIO_PIN | RELAY_PUMP_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void ADC_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    __HAL_RCC_ADC1_CLK_ENABLE();
    __HAL_RCC_ADC2_CLK_ENABLE();

    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    HAL_ADC_Init(&hadc1);

    sConfig.Channel = ADC_CHANNEL_1;  // PA1
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    hadc2.Instance = ADC2;
    hadc2.Init = hadc1.Init;
    HAL_ADC_Init(&hadc2);

    sConfig.Channel = ADC_CHANNEL_2;  // PA2
    HAL_ADC_ConfigChannel(&hadc2, &sConfig);
}

static void SystemClock_Config(void)
{
    // Silakan sesuaikan dengan konfigurasi clock pada STM32F4 Anda
}
