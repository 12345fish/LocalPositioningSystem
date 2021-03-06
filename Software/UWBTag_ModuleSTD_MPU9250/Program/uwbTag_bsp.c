/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\imu.h"

#include "uwbTag_bsp.h"
/*====================================================================================================*/
/*====================================================================================================*/
void UWBT_GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Clk ******************************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  /* GPIO all analog input *****************************************************/
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_All;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  GPIO_Init(GPIOC, &GPIO_InitStruct);
  GPIO_Init(GPIOD, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_All & (~(GPIO_Pin_13 | GPIO_Pin_14));
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* GPIO Pin ******************************************************************/
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = LED_R_PIN;
  GPIO_Init(LED_R_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = LED_G_PIN;
  GPIO_Init(LED_G_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = LED_B_PIN;
  GPIO_Init(LED_B_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_InitStruct.GPIO_Pin   = KEY_PIN;
  GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStruct);

  LED_R_Set();
  LED_G_Set();
  LED_B_Set();
}
/*====================================================================================================*/
/*====================================================================================================*/
pFunc UART1_irqEven = NULL;
void UWBT_UART_Config( pFunc pUARTx )
{
  UART1_irqEven = pUARTx;

  Serial_Config();
  printf("\r\nHello World!\r\n\r\n");
}
/*====================================================================================================*/
/*====================================================================================================*/
extern IMU_DataTypeDef IMU;
void UWBT_IMU_Config( void )
{
  IMU_InitTypeDef IMU_InitStruct;

  IMU_Config();
  delay_ms(50);

  printf("IMU Init ... ");
  IMU_InitStruct.Data                       = &IMU;
  IMU_InitStruct.Init.MPU_Gyr_FullScale     = MPU_GyrFS_2000dps;
  IMU_InitStruct.Init.MPU_Gyr_LowPassFilter = MPU_GyrLPS_41Hz;
  IMU_InitStruct.Init.MPU_Acc_FullScale     = MPU_AccFS_4g;
  IMU_InitStruct.Init.MPU_Acc_LowPassFilter = MPU_AccLPS_41Hz;
  IMU_InitStruct.Init.MPU_Mag_FullScale     = MPU_MagFS_16b;
//  IMU_InitStruct.InitLPS.LPS_PresAVG        = LPS_PresAVG_8;
//  IMU_InitStruct.InitLPS.LPS_TempAVG        = LPS_TempAVG_8;
//  IMU_InitStruct.InitLPS.LPS_ODR            = LPS_ODR_12P5Hz;
  if(IMU_Init(&IMU_InitStruct) != SUCCESS) {
    printf("ERROR\r\n");
    while(1) {
      LED_R_Toggle();
      delay_ms(100);
    }
  }
  printf("SUCCESS\r\n\r\n");
  delay_ms(50);
}
/*====================================================================================================*/
/*====================================================================================================*/
