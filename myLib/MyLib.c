/*
 * MyLib.c
 *
 *  Created on: Nov 15, 2024
 *      Author: user
 */
#include "main.h"

extern UART_HandleTypeDef huart2;


int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart2, &ch, 1, 10);
	return(ch);
}

void StandBy()
{
	while(HAL_GPIO_ReadPin(B1_GPIO_Port,B1_Pin));
}

void ProgramStart(char * str)
{
	cls();
	Cursor(0,0);
	printf("\033[2J");//화면 clear
	printf("\033[0;0H"); //cursor다시 이동
	printf("Program Start - %s\r\n", str);
	printf("Press Blue-button(B1) to Start...\r\n");
	StandBy();
}


void cls()
{
	printf("\033[2J");//화면 clear
}

void Cursor(int x,int y)
{
	char buf[20];
	sprintf(buf,"\033[%d;%dH",y,x);
	puts(buf);//문자열 출력함수
}

