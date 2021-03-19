/* --------------Define to prevent recursive inclusion ----------------*/ 
#ifndef __UE4_H__
#define __UE4_H__

//includes
#include  "stm32f10x.h"                 //standard library 
#include 	"stm32f10x_rcc.h"							//RCC Clock library
#include 	"stm32f10x_rtc.h"							//R
#include 	"stm32f10x_gpio.h"						//GPIO I/O library
#include 	"stm32f10x_usart.h"						//USART communication library
#include 	"stm32f10x_exti.h"
#include 	"stm32f10x_tim.h"
#include 	"string.h"										//strcat()

//constants


//configuration for USART2
extern void InitExti1(void);
extern void InitUsart2(void);
extern void InitGpio(void);
extern void TIM4_Config(void);
void Waitms(int ms);
void UartPutString(USART_TypeDef *USARTx, char *str);

#endif 
