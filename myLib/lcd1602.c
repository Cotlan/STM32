/*
 * lcd1602.c
 *
 *  Created on: Nov 21, 2024
 *      Author: user
 *      Function : 1602 LCD device control
 */
#include "main.h"//project driver header file copy

extern I2C_HandleTypeDef *hi2c;//extern reference

#define I2C_ADDR 0x4E // 0x27 <<	 1

I2C_HandleTypeDef *hi2c=NULL;


void lcd_command(char cmd)// cmd_bit : abcd efgh
{
	char n1, n2,n3,n4, dd[4];
	n1 = cmd & 0xf0; //n1: abcd 0000 upper nibble
	n2 = (cmd <<4); // n2: efgh 0000
	n3 = (1<<3)|(1<<2)|0|0;//(RW|EN_1|NC|RS) 0x0c
	n4 = (1<<3)|0|0|0;//(RW|EN_0|NC|RS) 0x08
	dd[0] = n1 | n3;
	dd[1] = n1 | n4;// enable, disable, valid data(keep data)
	dd[2] = n2 | n3;
	dd[3] = n2 | n4;// enable, disable, valid data(keep data)
	HAL_I2C_Master_Transmit(hi2c, I2C_ADDR, dd, 4, 10);// 4byte, timeout 10
}

void lcd_data(char ch)
{
	char n1, n2,n3,n4, dd[4];
	n1 = ch & 0xf0; //n1: abcd 0000 upper nibble
	n2 = (ch <<4); // n2: efgh 0000
	n3 = (1<<3)|(1<<2)|0|(1<<0);//(RW|EN_1|NC|RS) 0x0d
	n4 = (1<<3)|0|0|(1<<0);//(RW|EN_0|NC|RS) 0x09
	dd[0] = n1 | n3;
	dd[1] = n1 | n4;// enable, disable, valid data(keep data)
	dd[2] = n2 | n3;
	dd[3] = n2 | n4;// enable, disable, valid data(keep data)
	HAL_I2C_Master_Transmit(hi2c, I2C_ADDR, dd, 4, 10);// 4byte, timeout 10
}

void lcd_init()
{
	lcd_command(0x01); // screen clear
	lcd_command(0x02); // cursor home
	lcd_command(0x06); //
	lcd_command(0x0f);
	HAL_Delay(10);
}

int i2c_init(I2C_HandleTypeDef *p)
{
	hi2c = p;
}

void lcd_printEx(char *str, int ln)
{
	if(ln == 0) lcd_command(0x80);
	if(ln == 1) lcd_command(0xc0);
	lcd_print(str);
}

int ln2 =0; //current line
char sBuf[20];
void lcd_printEx2(char *str)
{
	if(ln2 == 0)
	{
		lcd_command(0x80);
		ln2++;
	}
	else if(ln2 == 1)
	{
		lcd_command(0x80);//Line1
		lcd_print(sBuf);
		lcd_command(0xc0);
		strcpy(sBuf,str);
	}
	lcd_print(str);
}

void lcd_print(char *str)//character parameter pointer
{
	while(*str) lcd_data(*str++);
}

int i2c_scan()
{
	if(hi2c == NULL) return;
	for(int addr=0;addr<128;addr++)
	{
		if(HAL_I2C_IsDeviceReady(hi2c, addr, 1, 10) == HAL_OK)
		{
			printf("  %02x ", addr);
		}
		else
		{
			printf("   . ");
		}
		if((addr % 16) == 0) printf("\r\n");
	}
}

void Printnum(unsigned short number, int ln)
{
	char str[5];
    number = number % 10000;

    str[0] = (number / 1000) + '0';
    str[1] = (number / 100 % 10) + '0';
    str[2] = (number / 10 % 10) + '0';
    str[3] = (number % 10) + '0';
    str[4] = '\0';
    lcd_printEx(number, ln);
}
