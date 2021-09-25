/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f429i_discovery_lcd.h"
#include "SRlogosmall.h"
#include "Retron 2000 num.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

CAN_HandleTypeDef hcan2;

DMA2D_HandleTypeDef hdma2d;

I2C_HandleTypeDef hi2c3;

LTDC_HandleTypeDef hltdc;

SPI_HandleTypeDef hspi5;

SDRAM_HandleTypeDef hsdram1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA2D_Init(void);
static void MX_FMC_Init(void);
static void MX_I2C3_Init(void);
static void MX_LTDC_Init(void);
static void MX_SPI5_Init(void);
static void MX_ADC1_Init(void);
static void MX_CAN2_Init(void);
/* USER CODE BEGIN PFP */
char getSwtichMode();
void driverScreen();
void powertrainScreen();
void suspensionScreen();
void myDrawImage(int Ypos, int Xpos, tImage image);
void myDrawImageBit(int Ypos, int Xpos, fImage image);
void dvdThing(int *v, int *h, int *x, int *y);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1);
char *utoa(uint32_t num, char *str, int radix);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t pot_value;
enum Modes {Idle, DriverScreen, Suspension, Powertrain};

uint8_t CANresponse[50];
CAN_RxHeaderTypeDef RxCanHeader;
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

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_I2C3_Init();
  MX_LTDC_Init();
  MX_SPI5_Init();
  MX_ADC1_Init();
  MX_CAN2_Init();
  /* USER CODE BEGIN 2 */
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(LCD_BACKGROUND_LAYER, LCD_FRAME_BUFFER);
  BSP_LCD_LayerDefaultInit(LCD_FOREGROUND_LAYER, LCD_FRAME_BUFFER);
  BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
  BSP_LCD_DisplayOn();
  BSP_LCD_Clear(LCD_COLOR_BLACK);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

  HAL_SetTickFreq(HAL_TICK_FREQ_100HZ);
  BSP_LED_Init(LED3);
  BSP_LED_Toggle(LED3);
  BSP_LED_Init(LED4);
  int x = 0, y = 0;
  int v = 1, h = 1;
  //uint32_t val;
  HAL_ADC_ConvCpltCallback(&hadc1);
  //char str[20];
  char mode = 'x';
  char prevMode = '0';
  //int iter = 0x0;

  while(1)
  {
    BSP_LCD_SelectLayer(LCD_BACKGROUND_LAYER);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 1000);
    pot_value = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    mode = getSwtichMode();
    if(mode != prevMode)
    {
      prevMode = mode;
      BSP_LCD_Clear(LCD_COLOR_BLACK);
    }

    //Selecting Screen Mode
    switch(mode)
    {
      case Idle:
        dvdThing(&v, &h, &x, &y);
        break;
      case DriverScreen:
        driverScreen();
        break;
      case Suspension:
        suspensionScreen();
        break;
      case Powertrain:
        powertrainScreen();
        break;
      case 'a':
        break;
    }

    HAL_Delay(15);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 432;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
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
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief CAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */

  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 9;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_8TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */
  CAN_FilterTypeDef filterConfig;

  filterConfig.FilterBank = 15;
  filterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  filterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  filterConfig.FilterIdHigh = 0x0000;
  filterConfig.FilterIdLow = 0x0000;
  filterConfig.FilterMaskIdHigh = 0x0000;
  filterConfig.FilterMaskIdLow = 0x0000;
  filterConfig.FilterFIFOAssignment = 0;
  filterConfig.FilterActivation = ENABLE;

  HAL_CAN_ConfigFilter(&hcan2, &filterConfig);
  /* USER CODE END CAN2_Init 2 */

}

/**
  * @brief DMA2D Initialization Function
  * @param None
  * @retval None
  */
static void MX_DMA2D_Init(void)
{

  /* USER CODE BEGIN DMA2D_Init 0 */

  /* USER CODE END DMA2D_Init 0 */

  /* USER CODE BEGIN DMA2D_Init 1 */

  /* USER CODE END DMA2D_Init 1 */
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_M2M;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB565;
  hdma2d.Init.OutputOffset = 0;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0;
  if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DMA2D_Init 2 */

  /* USER CODE END DMA2D_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};
  LTDC_LayerCfgTypeDef pLayerCfg1 = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 7;
  hltdc.Init.VerticalSync = 3;
  hltdc.Init.AccumulatedHBP = 14;
  hltdc.Init.AccumulatedVBP = 5;
  hltdc.Init.AccumulatedActiveW = 334;
  hltdc.Init.AccumulatedActiveH = 245;
  hltdc.Init.TotalWidth = 340;
  hltdc.Init.TotalHeigh = 247;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 0;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 0;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg.Alpha = 0;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0;
  pLayerCfg.ImageWidth = 0;
  pLayerCfg.ImageHeight = 0;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg1.WindowX0 = 0;
  pLayerCfg1.WindowX1 = 0;
  pLayerCfg1.WindowY0 = 0;
  pLayerCfg1.WindowY1 = 0;
  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg1.Alpha = 0;
  pLayerCfg1.Alpha0 = 0;
  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg1.FBStartAdress = 0;
  pLayerCfg1.ImageWidth = 0;
  pLayerCfg1.ImageHeight = 0;
  pLayerCfg1.Backcolor.Blue = 0;
  pLayerCfg1.Backcolor.Green = 0;
  pLayerCfg1.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */

  /* USER CODE END LTDC_Init 2 */

}

/**
  * @brief SPI5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI5_Init(void)
{

  /* USER CODE BEGIN SPI5_Init 0 */

  /* USER CODE END SPI5_Init 0 */

  /* USER CODE BEGIN SPI5_Init 1 */

  /* USER CODE END SPI5_Init 1 */
  /* SPI5 parameter configuration*/
  hspi5.Instance = SPI5;
  hspi5.Init.Mode = SPI_MODE_MASTER;
  hspi5.Init.Direction = SPI_DIRECTION_2LINES;
  hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi5.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi5.Init.NSS = SPI_NSS_SOFT;
  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi5.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI5_Init 2 */

  /* USER CODE END SPI5_Init 2 */

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_1;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_DISABLE;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 16;
  SdramTiming.ExitSelfRefreshDelay = 16;
  SdramTiming.SelfRefreshTime = 16;
  SdramTiming.RowCycleDelay = 16;
  SdramTiming.WriteRecoveryTime = 16;
  SdramTiming.RPDelay = 16;
  SdramTiming.RCDDelay = 16;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
uint16_t bytearr[4];

char getSwtichMode()
{
  char mode;
  if(pot_value < 15)
    mode = Idle;
  else if(pot_value < 40)
    mode = DriverScreen;
  else if(pot_value < 150)
    mode = Suspension;
  else if(pot_value < 300)
    mode = Powertrain;

  return mode;
}

void driverScreen()
{
  BSP_LCD_DisplayStringAt(0,   0,   (uint8_t *) "OIL TEMP", LEFT_MODE);
  BSP_LCD_DisplayStringAt(50,  0,   (uint8_t *) "BATT", LEFT_MODE);
  BSP_LCD_DisplayStringAt(100, 0,   (uint8_t *) "OIL PRESS", LEFT_MODE);
  BSP_LCD_DisplayStringAt(150, 0, (uint8_t *) "EXH TEMP", LEFT_MODE);

  BSP_LCD_DisplayChar(100, 100, 'N');
  BSP_LCD_DisplayStringAt(140, 130, (uint8_t *) "RPM", LEFT_MODE);

//  BSP_LCD_DisplayStringAt(0,   200,   (uint8_t *) "WTR TEMP", LEFT_MODE);
//  BSP_LCD_DisplayStringAt(50,  200,   (uint8_t *) "WTR PUMP", LEFT_MODE);
//  BSP_LCD_DisplayStringAt(100, 200,   (uint8_t *) "FUEL PUMP", LEFT_MODE);
//  BSP_LCD_DisplayStringAt(150, 200,   (uint8_t *) "FAN", LEFT_MODE);

//  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *) "LAP", LEFT_MODE);
//  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *) "tLastLap", LEFT_MODE);
//  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *) "tCurrDelta", LEFT_MODE);
//  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *) "DRS", LEFT_MODE);

//  BSP_LCD_DisplayStringAt(72, 210, (uint8_t *) "67890", LEFT_MODE);
//  BSP_LCD_DisplayStringAt(22, 210, (uint8_t *) "67890", LEFT_MODE);
//  BSP_LCD_DisplayStringAt(72, 0,   (uint8_t *) "12345", LEFT_MODE);
//  BSP_LCD_DisplayStringAt(22, 0,   (uint8_t *) "12345", LEFT_MODE);
//  BSP_LCD_DisplayStringAt(172, 100, (uint8_t *) "123456", LEFT_MODE);
}

void powertrainScreen()
{
  BSP_LCD_DisplayStringAt(2,   0, (uint8_t *) "WTR TP:", LEFT_MODE);
  BSP_LCD_DisplayStringAt(22,  0, (uint8_t *) "OIL TP:", LEFT_MODE);
  BSP_LCD_DisplayStringAt(44,  0, (uint8_t *) "OIL PR:", LEFT_MODE);
  BSP_LCD_DisplayStringAt(66,  0, (uint8_t *) "MAP:", LEFT_MODE);
  BSP_LCD_DisplayStringAt(88,  0, (uint8_t *) "AFR:", LEFT_MODE);
  BSP_LCD_DisplayStringAt(110, 0, (uint8_t *) "EXH:", LEFT_MODE);
  BSP_LCD_DisplayStringAt(132, 0, (uint8_t *) "RMP:", LEFT_MODE);
  BSP_LCD_DisplayStringAt(154, 0, (uint8_t *) "WP1:", LEFT_MODE);
  BSP_LCD_DisplayStringAt(176, 0, (uint8_t *) "WP2:", LEFT_MODE);
  BSP_LCD_DisplayStringAt(198, 0, (uint8_t *) "FAN1:", LEFT_MODE);
  BSP_LCD_DisplayStringAt(220, 0, (uint8_t *) "FAN2:", LEFT_MODE);

  BSP_LCD_DisplayStringAt(2,   150, (uint8_t *) "12345", LEFT_MODE);
  BSP_LCD_DisplayStringAt(22,  150, (uint8_t *) "67890", LEFT_MODE);
  BSP_LCD_DisplayStringAt(44,  150, (uint8_t *) "12345", LEFT_MODE);
  BSP_LCD_DisplayStringAt(66,  150, (uint8_t *) "67890", LEFT_MODE);
  BSP_LCD_DisplayStringAt(88,  150, (uint8_t *) "12345", LEFT_MODE);
  BSP_LCD_DisplayStringAt(110, 150, (uint8_t *) "67890", LEFT_MODE);
  BSP_LCD_DisplayStringAt(132, 150, (uint8_t *) "12345", LEFT_MODE);
  BSP_LCD_DisplayStringAt(154, 150, (uint8_t *) "67890", LEFT_MODE);
  BSP_LCD_DisplayStringAt(176, 150, (uint8_t *) "12345", LEFT_MODE);
  BSP_LCD_DisplayStringAt(198, 150, (uint8_t *) "67890", LEFT_MODE);
  BSP_LCD_DisplayStringAt(220, 150, (uint8_t *) "12345", LEFT_MODE);
}

void suspensionScreen()
{
  BSP_LCD_DisplayStringAt(0,   0, (uint8_t *) "RR:", LEFT_MODE);
  BSP_LCD_DisplayStringAt(22,  0, (uint8_t *) "RL:", LEFT_MODE);
  BSP_LCD_DisplayStringAt(44,  0, (uint8_t *) "FR:", LEFT_MODE);
  BSP_LCD_DisplayStringAt(66,  0, (uint8_t *) "FL:", LEFT_MODE);
  BSP_LCD_DisplayStringAt(88,  0, (uint8_t *) "FB:", LEFT_MODE);
  BSP_LCD_DisplayStringAt(110, 0, (uint8_t *) "RB", LEFT_MODE);
  BSP_LCD_DisplayStringAt(132, 0, (uint8_t *) "FBP:", LEFT_MODE);
  BSP_LCD_DisplayStringAt(154, 0, (uint8_t *) "BBP:", LEFT_MODE);
  BSP_LCD_DisplayStringAt(176, 0, (uint8_t *) "STR ANG:", LEFT_MODE);

  BSP_LCD_DisplayStringAt(0,   150, (uint8_t *) "12345", LEFT_MODE);
  BSP_LCD_DisplayStringAt(22,  150, (uint8_t *) "67890", LEFT_MODE);
  BSP_LCD_DisplayStringAt(44,  150, (uint8_t *) "12345", LEFT_MODE);
  BSP_LCD_DisplayStringAt(66,  150, (uint8_t *) "67890", LEFT_MODE);
  BSP_LCD_DisplayStringAt(88,  150, (uint8_t *) "12345", LEFT_MODE);
  BSP_LCD_DisplayStringAt(110, 150, (uint8_t *) "67890", LEFT_MODE);
  BSP_LCD_DisplayStringAt(132, 150, (uint8_t *) "12345", LEFT_MODE);
  BSP_LCD_DisplayStringAt(154, 150, (uint8_t *) "67890", LEFT_MODE);
  BSP_LCD_DisplayStringAt(176, 150, (uint8_t *) "12345", LEFT_MODE);
}

void dvdThing(int *v, int *h, int *x, int *y)
{
  if(*y <= 0)
    *v = 1;
  if(*y >= 237 - SRlogosmall.height)
    *v = -1;
  if(*x <= 0)
    *h = 1;
  if(*x >= 320 - SRlogosmall.width)
    *h = -1;
  myDrawImage(*x, *y, SRlogosmall);
  *y += *v;
  *x += *h;
}

uint32_t convert16to32(uint16_t colour)
{
  for (int i=0; i < 4; i++)
  {
    bytearr[i] = ((colour >> i*4) & 0xF);
  }

  uint32_t long_colour = 0;

  for(int i=7; i>=0; i--)
  {
    long_colour = long_colour << 4;
    //if (i % 2)
    long_colour = long_colour | bytearr[i/2];
	}
  return long_colour;
}

void myDrawImageBit(int Ypos, int Xpos, fImage image)
{
  BSP_LCD_SelectLayer(LCD_BACKGROUND_LAYER);
  for(int i = 0; i < image.height; i++)
  {
    for(int j = 0; j < image.width; j++)
    {
      if(image.data[(i*image.width) + j])
        BSP_LCD_DrawPixel(i + Xpos, 320 - (j + Ypos), 0xffffffff);
      else
        BSP_LCD_DrawPixel(i + Xpos, 320 - (j + Ypos), 0x00000000);
    }
  }
}

void myDrawImage(int Ypos, int Xpos, tImage image)
{
  BSP_LCD_SelectLayer(LCD_BACKGROUND_LAYER);
  for(int i = 0; i < image.height; i++)
  {
    for(int j = 0; j < image.width; j++)
    {
      BSP_LCD_DrawPixel(i + Xpos, 320 - (j + Ypos), convert16to32(image.data[(i*image.width) + j]));
    }
  }
}

//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1)
//{
//  pot_value = HAL_ADC_GetValue(hadc1);
//  HAL_ADC_Start_IT(hadc1); // Re-Start ADC1 under Interrupt
//                           // this is necessary because we don'use
//                           // the Continuous Conversion Mode
//  //BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
//  //BSP_LCD_DisplayStringAtLine(1,(uint8_t*)pot_value);
//}

/*
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  //Get data
}
*/

/*******************************************************
* Code contributed by Chris Takahashi,                 *
* ctakahashi (at) users (dot) sourceforge (dot) net.   *
* See stdlib.h for licence.                            *
* $Date: 2005/08/31 11:39:47 $                         *
*******************************************************/

char *utoa(uint32_t num, char *str, int radix)
{
  char temp[17];  //an int can only be 16 bits long
                  //at radix 2 (binary) the string
                  //is at most 16 + 1 null long.
  int temp_loc = 0;
  int digit;
  int str_loc = 0;

  //construct a backward string of the number.
  do
  {
    digit = (uint32_t)num % radix;
    if (digit < 10)
      temp[temp_loc++] = digit + '0';
    else
      temp[temp_loc++] = digit - 10 + 'A';
    (num) /= radix;
  } while((uint32_t)num > 0);

  temp_loc--;

  //now reverse the string.
  while ( temp_loc >=0 ) // while there are still chars
  {
    str[str_loc++] = temp[temp_loc--];
  }
  str[str_loc] = 0; // add null termination.

  return str;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
