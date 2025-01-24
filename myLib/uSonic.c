#include "main.h"

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;


void microDelay(int us) // us : micro second
{
   int t1 = htim2.Instance->CNT;
//   while(1)
//   {
//      if(htim2.Instance->CNT - t1 >= us) break;
//   }
   while(htim2.Instance->CNT - t1 < us);
//   htim2.Instance->CNT = 0;
//   while(htim2.Instance->CNT < us);
}

void Trigger()
{
   HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, 0);
   microDelay(10);
   HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, 1);
   microDelay(10);
   HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, 0);
}

