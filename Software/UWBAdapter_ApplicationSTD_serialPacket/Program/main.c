/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"

#include "uwbAdapter.h"
/*====================================================================================================*/
/*====================================================================================================*/
int main( void )
{
  HAL_InitTick();

  UWBA_Init();
  UWBA_Loop();
}
/*====================================================================================================*/
/*====================================================================================================*/
