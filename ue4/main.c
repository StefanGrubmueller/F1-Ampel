/*
*/
#include "ue4.h"

volatile uint16_t rising_edge;

//External Interupt 1 Routine Handler
void EXTI1_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line1);
	rising_edge++;
	return;
}

//TIMER 4 Interrupt Routine Handler
//PWM Output Mode, Channel 3
void TIM4_IRQHandler(void)
{ 
	static char direction=1; //Auf- / Abdimmen
	TIM4->SR &=~0x00FF;		//Interrupt pending-bit löschen
	if(direction==0) 
	{
		//Increment Compare Value Channel 3 
		TIM_SetCompare3(TIM4, TIM_GetCapture3(TIM4)+1);
		if(TIM_GetCapture3(TIM4)>=(TIM4->ARR/2))
		{
			direction=1; //Danach abdimmen
		}
	}
	else 
	{
		//Decrement Compare Value Channel 3 
		TIM_SetCompare3(TIM4, TIM_GetCapture3(TIM4)-1);
		if(TIM_GetCapture3(TIM4)==0)
		{
			direction=0; //Danach aufdimmen
		}
	}
	return;
}


//main
int main()
{
	InitUsart2();		//USART2 Init (Log)
	InitGpio();			//GPIOB1 Init (Externe LEDs) (F1 Ampel)
	InitExti1();		//EXTI1 Init (Flankenzähler)
	TIM4_Config();	//TIM4 Output Compare Init (PWM) (Countdown)
	UartPutString(USART2,"Initialized\n");
	
	rising_edge=0;
	int ibuf=0;
	char buf[32];
	
	//Endlosschleife
	while(1)
	{
		if(rising_edge==ibuf)
		{
			ibuf = rising_edge;
			ibuf++;
			
			sprintf(buf,"%d sek:\n", rising_edge);
			UartPutString(USART2, buf);
			
			//Flanken werden gezählt
			switch (rising_edge)
			{
				//Erste LED an nach 5 Flanken (5sek)
				case 5:		
					UartPutString(USART2, "LED 1 AN\n");
					UartPutString(USART2, "----------\n");
					GPIO_SetBits(GPIOC,GPIO_Pin_2); 
					break;
				//Zweite LED an nach 6 Flanken (6sek)
				case 6:		
					UartPutString(USART2, "LED 2 AN\n");
					UartPutString(USART2, "----------\n");
					GPIO_SetBits(GPIOC,GPIO_Pin_3); 
					break;
				//Dritte LED an nach 7 Flanken (7ek)
				case 7:		
					UartPutString(USART2, "LED 3 AN\n");
					UartPutString(USART2, "----------\n");
					GPIO_SetBits(GPIOB,GPIO_Pin_7); 
					break;
				//Vierte LED an nach 8 Flanken (8sek)
				case 8:		
					UartPutString(USART2, "LED 4 AN\n");
					UartPutString(USART2, "----------\n");
					GPIO_SetBits(GPIOB,GPIO_Pin_6); 
					break;
				//Fuenfte LED an nach 9 Flanken (9sek)
				case 9:		
					UartPutString(USART2, "LED 5 AN\n");
					UartPutString(USART2, "----------\n");
					GPIO_SetBits(GPIOA,GPIO_Pin_5); 
					break;
				//Alle LEDs aus nach 11 Flanken (11sek) => Lights out
				case 11:	
					UartPutString(USART2, "LIGHTS OUT\n");
					UartPutString(USART2, "----------\n");
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);	//Externe LED aus (Raketenbooster aus)
					GPIO_ResetBits(GPIOB,GPIO_Pin_6);
					GPIO_ResetBits(GPIOB,GPIO_Pin_7);
					GPIO_ResetBits(GPIOC,GPIO_Pin_2);
					GPIO_ResetBits(GPIOC,GPIO_Pin_3);	
					ibuf=0;
					rising_edge = 0;	//Flankenzähler rücksetzen
					break;
					
				//Bei Error wird Fehlermeldung per USART angezeigt
				default:
						break;
			}
		}
	}
}
