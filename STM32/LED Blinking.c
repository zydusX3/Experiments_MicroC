/*LED blinking at PC13*/
//#include<stdio.h>
#include <stdio.h>
#include "stm32f103x6.h"

#define RCC_AHBENR_REG (0x40021014)//(0x40021000) //AHBENR regtr
#define RCC_AHBENR_IOPCEN (1 << 4)  //PORT C Enable

#define RCC_APB2ENR_REG (0x40021018) //APB2ENR regtr
#define RCC_APB2ENR_IOPCEN (1 << 4)  //PORT C Enable

#define GPIOC_MODER_REG (0x40011004)
#define GPIOC_MODER_OUT (1 << 21)
#define GPIOC_OTYPER_REG (0x40010C04)
#define GPIOC_OTYPER_PP (0 << 13)

#define GPIOC_ODR_REG (0x4001100c)


/*
 *
 * CRL 0x40011000 == MOD
 * ODR 0x4001100c
 * */

int main(void)
{

	volatile uint32_t *RCC_AHBENR = (volatile uint32_t *)RCC_AHBENR_REG;
	*RCC_AHBENR |= RCC_AHBENR_IOPCEN;

	volatile uint32_t *RCC_APB2ENR = (volatile uint32_t *)RCC_APB2ENR_REG;
	*RCC_APB2ENR |= RCC_APB2ENR_IOPCEN;

	volatile uint32_t *GPIOC_MODER = (volatile uint32_t *)GPIOC_MODER_REG;
	*GPIOC_MODER |= GPIOC_MODER_OUT;

	//volatile uint32_t *GPIOC_OTYPER = (volatile uint32_t *)GPIOC_OTYPER_REG;
	//*GPIOC_OTYPER &= ~GPIOC_OTYPER_PP;

	volatile uint32_t *GPIOC_ODR = (volatile uint32_t *)GPIOC_ODR_REG;
	*GPIOC_ODR |= (1 << 13);
	//printf("Hello World\n");

	 while (1) {
	    *GPIOC_ODR |= (1 << 13);
	    for (int i = 0; i < 1000000; i++);
	    *GPIOC_ODR &= ~(1 << 13);
	    for (int i = 0; i < 1000000; i++);
	    //printf("\nHello World with delay\n");
	  }
}
