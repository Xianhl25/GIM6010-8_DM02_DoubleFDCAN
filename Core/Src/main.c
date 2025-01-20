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
#include "fdcan.h"
#include "memorymap.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_fdcan.h"
#include "MWMotor.h"
#include "lcd.h"
#include "pic.h"
#include "decoder.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void leftMotorSet(uint8_t busId, uint8_t can_id, uint8_t *data, uint8_t dataSize) {
    if(busId == 0x001) CAN_Send_StdDataFrame(&hfdcan1, can_id, data);
}

void rightMotorSet(uint8_t busId, uint8_t can_id, uint8_t *data, uint8_t dataSize) {
    if(busId == 0x001) CAN_Send_StdDataFrame(&hfdcan2, can_id, data);
}

void leftMotorNotice(uint8_t busId, uint8_t nodeId, MW_CMD_ID cmdId) {
    if(busId == 0x001) return;
}

void rightMotorNotice(uint8_t busId, uint8_t nodeId, MW_CMD_ID cmdId) {
    if(busId == 0x001) return;
}

MW_MOTOR_DATA leftMotorData;
MW_MOTOR_DATA rightMotorData;

uint8_t leftMotorRxData[8];
uint8_t leftMotorTorqueSetpoint[4];
uint8_t leftMotorTorqueFeedback[4];
uint8_t rightMotorRxData[8];
uint8_t rightMotorTorqueSetpoint[4];
uint8_t rightMotorTorqueFeedback[4];
float leftMotorTorqueFeedbackValue;
float rightMotorTorqueFeedbackValue;
char leftMotorTorqueFeedbackString[10];
char rightMotorTorqueFeedbackString[10];

MWMotorAccessInfo leftMotor = {.busId = 1, .nodeId = 0, .motorData = &leftMotorData, .sender = leftMotorSet, .notifier = leftMotorNotice};
MWMotorAccessInfo rightMotor = {.busId = 1, .nodeId = 1, .motorData = &rightMotorData, .sender = rightMotorSet, .notifier = rightMotorNotice};

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
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

  /* Enable the CPU Cache */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

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
  MX_FDCAN1_Init();
  MX_FDCAN2_Init();
  MX_SPI1_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  
	/* LCD配置 */
	LCD_Init();
	LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
	
	/* FDCAN配置 */
	fdcan1.rx_Filter_Init(); /* 初始化FDCAN1滤波器 */
	fdcan2.rx_Filter_Init(); /* 初始化FDCAN2滤波器 */
	fdcan1.start();
	fdcan2.start();
	fdcan1.rx_Interrupt_Init(); /* 打开FDCAN1的接收中断 */
	fdcan2.rx_Interrupt_Init(); /* 打开FDCAN2的接收中断 */
	
	_CANMSG can_msg;
	
	/* 注册电机 */
	MWRegisterMotor(leftMotor);
	
	HAL_Delay(100);
	
	MWRegisterMotor(rightMotor);
	
	HAL_Delay(100);
	
	/* 设置模式为直接力矩控制模式 */
	MWSetControllerMode(1, 0, MW_TORQUE_CONTROL, MW_DIRECT_CONTROL_INPUT);
	
	HAL_Delay(100);
	
	MWSetControllerMode(1, 1, MW_TORQUE_CONTROL, MW_DIRECT_CONTROL_INPUT);
	
	HAL_Delay(100);
	
    /* 进入闭环控制状态 */
    MWSetAxisState(1, 0, MW_AXIS_STATE_CLOSED_LOOP_CONTROL);
	
	HAL_Delay(100);
	
	MWSetAxisState(1, 1, MW_AXIS_STATE_CLOSED_LOOP_CONTROL);
	
	HAL_Delay(100);
	
	/* 输入控制力矩 */
	MWTorqueControl(1, 0, 1.0);
	
	HAL_Delay(100);
	
	MWTorqueControl(1, 1, 1.0);
	
	HAL_Delay(100);

//	MWDisableCAN(1, 1);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  convertFloatToString(leftMotorTorqueFeedbackValue, leftMotorTorqueFeedbackString);
	  convertFloatToString(rightMotorTorqueFeedbackValue, rightMotorTorqueFeedbackString);
	  LCD_ShowString(20, 72,(uint8_t *)"Left Torque: ", BRRED, BLACK, 24, 0);
	  LCD_ShowString(200, 72,(uint8_t *)leftMotorTorqueFeedbackString, BRRED, BLACK, 24, 0);
	  LCD_ShowString(20, 102,(uint8_t *)"Right Torque: ", BRRED, BLACK, 24, 0);
	  LCD_ShowString(200, 102,(uint8_t *)rightMotorTorqueFeedbackString, BRRED, BLACK, 24, 0);

	  
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLQ = 24;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance==TIM6)
	{
		if (fdcan1.rx_MSG.id == 0x14)
		{
			memcpy(leftMotorRxData, (const void *)(fdcan1.rx_MSG.buffer), 8*sizeof(uint8_t));
			memcpy(leftMotorTorqueFeedback, leftMotorRxData+4, 4*sizeof(uint8_t));
			leftMotorTorqueFeedbackValue = getFloat(convertUint8ArrayToUint32(leftMotorTorqueFeedback))*LEFT_MOTOR_TORQUE_CONSTANT;
		}
		
		if (fdcan2.rx_MSG.id == 0x34)
		{
			memcpy(rightMotorRxData, (const void *)(fdcan2.rx_MSG.buffer), 8*sizeof(uint8_t));
			memcpy(rightMotorTorqueFeedback, rightMotorRxData+4, 4*sizeof(uint8_t));
			rightMotorTorqueFeedbackValue = getFloat(convertUint8ArrayToUint32(rightMotorTorqueFeedback))*RIGHT_MOTOR_TORQUE_CONSTANT;
		}
	}
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
