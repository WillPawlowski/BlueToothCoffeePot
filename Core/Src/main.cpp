/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */
#include "retarget.h"
#include "Timer.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
LPTIM_HandleTypeDef hlptim1;
UART_HandleTypeDef huart1; //Used with BT Module
UART_HandleTypeDef huart2; //Used only for debugging

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_LPTIM1_Init(void);

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define SINGLE_CHAR_BYTES 3

uint8_t receiveBT_Data(int size);
void printOptions();
void turnOnCoffeePot();
void turnOffCoffeePot();
void startNewTimer();
void translateTime(int timeInSeconds, int *hours, int *minutes, int *seconds);

Timer myTimer(turnOnCoffeePot);
char sleepMsg[] = {"Entering Sleep\r\n"};
bool uartRecieved = false;
char response[SINGLE_CHAR_BYTES];
uint8_t receiveBuffer[100];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_LPTIM1_Init();

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	RetargetInit(&huart1);
	HAL_UART_Receive_IT(&huart1, (uint8_t*) &response, SINGLE_CHAR_BYTES);
	printf("Device Initialized. Going to sleep\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t *)sleepMsg, strlen(sleepMsg), HAL_MAX_DELAY);
  HAL_PWR_EnableSleepOnExit();
  while (1)
  {
  	HAL_UART_Transmit(&huart2, (uint8_t *)"W", 1, HAL_MAX_DELAY);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_LPTIM1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.LptimClockSelection = RCC_LPTIM1CLKSOURCE_LSI;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief LPTIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPTIM1_Init(void)
{
  hlptim1.Instance = LPTIM1;
  hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV4;
  hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
  hlptim1.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
  hlptim1.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
  hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
  if (HAL_LPTIM_Init(&hlptim1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPTIM1_Init 2 */

	HAL_NVIC_SetPriority(LPTIM1_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(LPTIM1_IRQn);

	/* 46249 is the value loaded into LPTIM1's ARR register */
	/* Rate = ClkSrc/[Prescaler * ARR+1] = .2Hz*/
	HAL_LPTIM_Counter_Start_IT(&hlptim1, 46249);
  /* USER CODE END LPTIM1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
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
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|LCD_DC_Pin|Power_Toggle_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin LCD_DC_Pin Power_Toggle_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|LCD_DC_Pin|Power_Toggle_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_RST_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_CS_Pin */
  GPIO_InitStruct.Pin = LCD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_CS_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
uint8_t receiveBT_Data(int size)
{
	//Serial BT App always ends with \n\r
	//Add two bytes to receive data size to make sure UART is always empty
	uint8_t receivedBytes = 0;
	for(int i = 0; i<size; i++)
	{
		scanf("%c", (receiveBuffer+i));
		receivedBytes++;
	}
	return receivedBytes;
}


void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
    /* Blink onboard LED */
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	myTimer.incrementTimer(5);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
    printf("ERR\r\n");
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	//HAL_PWR_DisableSleepOnExit();
	printOptions();
	printf("Going Back To Sleep\r\n");
	HAL_UART_Receive_IT(&huart1, (uint8_t*) &response, SINGLE_CHAR_BYTES);
}

void printOptions()
{
	printf("Select an Option:\r\n");
	printf("1. Schedule Coffee\r\n");
	printf("2. Turn on Coffee Pot\r\n");
	printf("3. Turn off Coffee Pot\r\n");
	printf("4. Stop a Timer\r\n");
	printf("5. Get time left\r\n");
	receiveBT_Data(3);
	switch(receiveBuffer[0]){
		case '1':
			startNewTimer();
			break;
		case '2':
			turnOnCoffeePot();
			break;
		case '3':
			turnOffCoffeePot();
			break;
		case '4':
			myTimer.stopTimer();
			break;
		case '5':
			int hours, minutes, seconds;
			translateTime(myTimer.getTimeLeft(), &hours, &minutes, &seconds);
			printf("Hours: %d, Minutes %d, Seconds %d\r\n", hours, minutes, seconds);
			break;
		default:
			printf("Unrecognized Char\r\n");
		}
}

void turnOnCoffeePot()
{
	int h, m, s;
	HAL_GPIO_WritePin(LD2_GPIO_Port, Power_Toggle_Pin, GPIO_PIN_SET);
	translateTime(myTimer.setUpTimer(2, 1), &h, &m, &s);
	printf("Timer started for: Hours: %d, Minutes %d\r\n", h, m);
	myTimer.updateExecuteMethod(turnOffCoffeePot);
}

void turnOffCoffeePot()
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, Power_Toggle_Pin, GPIO_PIN_RESET);
}

void startNewTimer(){
	int hours, minutes, h, m, s;
	printf("How many hours to set timer for?\r\n");
	scanf("%d", &hours);
	printf("How many minutes to set timer for?\r\n");
	scanf("%d", &minutes);
	receiveBT_Data(2);
	translateTime(myTimer.setUpTimer(hours, minutes), &h, &m, &s);
	printf("Timer started for: Hours: %d, Minutes %d\r\n", h, m);
}

void translateTime(int timeInSeconds, int *hours, int *minutes, int *seconds)
{
	*hours = timeInSeconds/3600;
	int timeRemaining = timeInSeconds -((*hours) * 3600);
	*minutes = timeRemaining/60;
	*seconds = timeRemaining - *minutes * 60;
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
