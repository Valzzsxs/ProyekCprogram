/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <math.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define MAX_TINGGI_CM                     100.0f       // Tinggi maksimum air
#define LEVEL_DIFF_OPEN_THRESHOLD         5.0f         // Selisih cm untuk membuka gerbang
#define LEVEL_DIFF_EQUAL_THRESHOLD        1.0f         // Toleransi dianggap seimbang

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

/* GPIO Pin Definitions */
#define WATER_LEVEL_WADUK_GPIO_PIN        GPIO_PIN_1  // PA1
#define WATER_LEVEL_LUAR_GPIO_PIN         GPIO_PIN_2  // PA2

#define RELAY_OPEN_GATE_GPIO_PIN          GPIO_PIN_4  // PA4: Buka Gerbang
#define RELAY_CLOSE_GATE_GPIO_PIN         GPIO_PIN_5  // PA5: Tutup Gerbang
#define RELAY_PUMP_GPIO_PIN               GPIO_PIN_6  // PA6: Pompa Air

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
float Convert_ADC_to_cm(uint32_t adc_value);
float Read_Water_Level_Waduk_cm(void);
float Read_Water_Level_Luar_cm(void);

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  
  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();

  /* Infinite loop */
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

/**
  * @brief Convert ADC value to cm for water level calculation
  * @param adc_value: ADC value from sensor
  * @retval Water level in cm
  */
float Convert_ADC_to_cm(uint32_t adc_value)
{
    if (adc_value > 4095U)
    {
        return -1.0f;
    }
    return (adc_value / 4095.0f) * MAX_TINGGI_CM;
}

/**
  * @brief Read water level from waduk (reservoir) using ADC1
  * @retval Water level in cm
  */
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

/**
  * @brief Read water level from luar (outside) using ADC2
  * @retval Water level in cm
  */
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

/**
  * @brief System Clock Configuration
  * @retval None
  */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIOA clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure GPIO pins : Relay_BUka_Pin Relay_Tutup_Pin Relay_Pompa_Pin */
    GPIO_InitStruct.Pin = RELAY_OPEN_GATE_GPIO_PIN | RELAY_CLOSE_GATE_GPIO_PIN | RELAY_PUMP_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    /* Configure the global features of the ADC */
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure for the selected ADC regular channel */
    sConfig.Channel = ADC_CHANNEL_1;  // PA1
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    /* Configure the global features of the ADC */
    hadc2.Instance = ADC2;
    hadc2.Init = hadc1.Init;
    if (HAL_ADC_Init(&hadc2) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure for the selected ADC regular channel */
    sConfig.Channel = ADC_CHANNEL_2;  // PA2
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
    HAL_ADC_ConfigChannel(&hadc2, &sConfig);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}
