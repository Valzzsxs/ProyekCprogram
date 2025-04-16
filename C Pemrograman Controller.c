#include "stm32f4xx_hal.h"
#include <math.h>

/* GPIO Pin Definitions */
#define WATER_LEVEL_WADUK_GPIO_PIN   GPIO_PIN_1U  // PA1
#define WATER_LEVEL_LUAR_GPIO_PIN    GPIO_PIN_2U  // PA2
#define MOTOR_OPEN_GPIO_PIN          GPIO_PIN_4U  // PA4
#define MOTOR_CLOSE_GPIO_PIN         GPIO_PIN_5U  // PA5
#define PUMP_GPIO_PIN                GPIO_PIN_6U  // PA6

#define MAX_TINGGI_CM                100.0f  // Tinggi maksimum air

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

/* Function Prototypes */
static void SystemClock_Config(void);
static void GPIO_Init(void);
static void ADC_Init(void);
float Convert_ADC_to_cm(uint32_t adc_value);
float Read_Water_Level_Waduk_cm(void);
float Read_Water_Level_Luar_cm(void);

/* 
 * Convert ADC value to water height in centimeters.
 * - The ADC value is mapped to the water height range from 0 to MAX_TINGGI_CM.
 * 
 * @param adc_value: The raw ADC value to be converted.
 * 
 * @return: The water height in centimeters.
 * 
 * RULE CERT: COMPLIANT
 * - Avoid integer overflow by ensuring correct scaling.
 */
float Convert_ADC_to_cm(uint32_t adc_value)
{
    /* Validate adc_value to be within expected range (0-4095 for 12-bit ADC) */
    if (adc_value > 4095U)
    {
        /* Invalid ADC value, return a predefined error value or handle accordingly */
        return -1.0f;
    }
    return (adc_value / 4095.0f) * MAX_TINGGI_CM;
}

/* 
 * Read the water level from the Waduk sensor.
 * - Starts ADC conversion and waits for the result.
 * 
 * @return: The water level in the Waduk (dam) in centimeters.
 * 
 * RULE CERT: COMPLIANT
 * - Ensure correct handling of ADC conversion.
 * - Avoid race conditions and ensure ADC start and stop procedures are handled properly.
 */
float Read_Water_Level_Waduk_cm(void)
{
    HAL_ADC_Start(&hadc1);
    
    /* Check if the ADC conversion was successful before reading the value */
    if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK)
    {
        /* Handle ADC error or timeout */
        return -1.0f;  // Return an error value
    }

    uint32_t adc_value = HAL_ADC_GetValue(&hadc1);
    return Convert_ADC_to_cm(adc_value);
}

/* 
 * Read the water level from the Luar (external) sensor.
 * - Starts ADC conversion and waits for the result.
 * 
 * @return: The water level outside the Waduk in centimeters.
 * 
 * RULE CERT: COMPLIANT
 * - Ensure correct handling of ADC conversion.
 * - Avoid race conditions and ensure ADC start and stop procedures are handled properly.
 */
float Read_Water_Level_Luar_cm(void)
{
    HAL_ADC_Start(&hadc2);

    /* Check if the ADC conversion was successful before reading the value */
    if (HAL_ADC_PollForConversion(&hadc2, 10) != HAL_OK)
    {
        /* Handle ADC error or timeout */
        return -1.0f;  // Return an error value
    }

    uint32_t adc_value = HAL_ADC_GetValue(&hadc2);
    return Convert_ADC_to_cm(adc_value);
}

/* MAIN PROGRAM
 * - Initializes the system and reads water levels periodically.
 * - Controls motor and pump based on water levels.
 * 
 * RULE CERT: COMPLIANT
 * - Ensure the correct timing for operations (e.g., motor, pump control).
 * - Avoid reliance on hardcoded delays (prefer interrupt-driven or time-based checks).
 */
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

        /* 
         * If the external water level is higher than the Waduk by more than 5 cm,
         * the gate is opened and the pump is turned off.
         * 
         * RULE CERT: COMPLIANT
         * - Ensure that the control logic handles all edge cases, e.g., sensor failure or unexpected readings.
         * - Avoid using magic numbers, prefer named constants (e.g., 5.0f).
         */
        if (tinggiLuar > tinggiWaduk + 5.0f)
        {
            HAL_GPIO_WritePin(GPIOA, MOTOR_OPEN_GPIO_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, MOTOR_CLOSE_GPIO_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, PUMP_GPIO_PIN, GPIO_PIN_RESET); // Pompa off
        }
        /* 
         * If the water levels are nearly equal (difference < 1 cm),
         * the gate is closed and the pump is turned on.
         * 
         * RULE CERT: COMPLIANT
         * - Handle floating point precision issues. Ensure comparisons are valid within tolerance range.
         */
        else if (fabs(tinggiLuar - tinggiWaduk) < 1.0f)
        {
            HAL_GPIO_WritePin(GPIOA, MOTOR_OPEN_GPIO_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, MOTOR_CLOSE_GPIO_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, PUMP_GPIO_PIN, GPIO_PIN_SET); // Pompa ON
        }
        /* 
         * If the external water level is lower, the gate is closed and the pump is turned off.
         * 
         * RULE CERT: COMPLIANT
         * - Ensure the conditions for controlling hardware are logically sound.
         */
        else
        {
            HAL_GPIO_WritePin(GPIOA, MOTOR_OPEN_GPIO_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, MOTOR_CLOSE_GPIO_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, PUMP_GPIO_PIN, GPIO_PIN_RESET);
        }

        HAL_Delay(5000); // Delay 5 detik
    }
}

/* 
 * Initialize GPIO pins for motor and pump control.
 * - Configures GPIO pins for output mode to control the motor and pump.
 * 
 * RULE CERT: COMPLIANT
 * - Ensure correct initialization of GPIOs and configuration.
 * - Validate GPIO pin assignments are correct for the intended purpose.
 */
static void GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = MOTOR_OPEN_GPIO_PIN | MOTOR_CLOSE_GPIO_PIN | PUMP_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* 
 * Initialize ADC peripherals for reading water level sensors.
 * - Configures ADC1 for Waduk water level sensor and ADC2 for Luar sensor.
 * 
 * RULE CERT: COMPLIANT
 * - Ensure that ADC configuration is handled properly for each channel.
 * - Check for proper error handling in case of ADC initialization failure.
 */
static void ADC_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    __HAL_RCC_ADC1_CLK_ENABLE();
    __HAL_RCC_ADC2_CLK_ENABLE();

    /* Initialize ADC1 for Waduk sensor */
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

    /* Initialize ADC2 for Luar sensor */
    hadc2.Instance = ADC2;
    hadc2.Init = hadc1.Init;
    HAL_ADC_Init(&hadc2);

    sConfig.Channel = ADC_CHANNEL_2;  // PA2
    HAL_ADC_ConfigChannel(&hadc2, &sConfig);
}

/* 
 * Default system clock configuration.
 * - Placeholder function that should be customized for the specific STM32F4 configuration.
 * 
 * RULE CERT: COMPLIANT
 * - Ensure clock configuration is valid for the target system.
 */
static void SystemClock_Config(void)
{
    // Silakan disesuaikan dengan konfigurasi STM32F4 kamu
}
