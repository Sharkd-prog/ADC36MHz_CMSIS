/*
 * RCC_conf.c
 *
 *  Created on: Jun 8, 2026
 *      Author: Dima
 */
#include "RCC_comf.h"
#include "stm32f4xx.h"

void RCC_init(){

	//Вмикаємо HSE
	RCC->CR |= RCC_CR_HSEON;
	//Перевіряємо чи готовий HSE
	while(!(RCC->CR & RCC_CR_HSERDY));

	//Налаштування затримки Flash
	FLASH->ACR &= ~(0b11);
	FLASH->ACR |= 2;

	//Налаштовання Refetch
	FLASH->ACR |= (1 << 8);

	//Налаштування кешу для шин I-Code та D-Code
	FLASH->ACR |= (1 << 9);
	FLASH->ACR |= (1 << 10);

	//AHB прескалер
	RCC->CFGR &= ~RCC_CFGR_HPRE_DIV1;

	//APB1 прескалер
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

	//APH2 прескалер
	RCC->CFGR &= ~RCC_CFGR_PPRE2_DIV1;

	//Вмикаємо тактування AHB1 для PWR
	RCC->APB1ENR |= (1 << 28);

	//Налаштовуємо scale 2 VOS під 72MHz
	PWR->CR &= ~(0b11 << 14);
	PWR->CR |= (1 << 15);

	//Налаштування PLL
	RCC->PLLCFGR &= ~(0b111111 | (0b111111111 << 6) | (0b11 << 16) | (0b1111 << 24));
	RCC->PLLCFGR |= 25;
	RCC->PLLCFGR |= (144 << 6);
	RCC->PLLCFGR |= (3 << 24);
	RCC->PLLCFGR |= (1 << 22);

	//Увімкнення PLL
	RCC->CR |= (1 << 24);
	//перевіряємо чи готове PLL
	while(!(RCC->CR & RCC_CR_PLLRDY));

	//перемикаєм тактування системи з HSI на PLLCLK
	RCC->CFGR |= 2;
	//Чекаєм поки тактування перемкнеться на PLLCLK
	while((RCC->CFGR & (3 << 2)) != (2 << 2));
}
