#ifndef _PROJECT_CONFIG_H_
#define _PROJECT_CONFIG_H_

#include <reg52.h>
#include "intrins.h"
#include "motor.h"
#include "timer0.h"
#include "oled.h" 
#include "usart1.h" 
#include "usart2.h" 
#include "NRF24L01.h"
#include "GP2Y_ADC.h"

#define uchar unsigned char
#define uint unsigned int
#define open    101
#define close   102
#define stop    103
#define cont    104


sbit KEY1=P3^3;
sbit KEY2=P3^4;

#endif
