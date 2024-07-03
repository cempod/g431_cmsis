#include "main.h"
#include <stdint.h>
#include "stm32g4xx.h"

volatile uint32_t delayTimerValue = 0;

//	SysTick interrupt handle
void SysTick_Handler(void)
{
	delayTimerValue--;
}


//	RCC initialization
static void 
init_clock(void)
{
	SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_PWREN);
    CLEAR_BIT(PWR->CR5, PWR_CR5_R1MODE);
    CLEAR_BIT(RCC->APB1ENR1, RCC_APB1ENR1_PWREN);

    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_4WS);

    RCC->CR |= RCC_CR_HSEON;

    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC | RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLR,
             RCC_PLLCFGR_PLLSRC_HSE | RCC_PLLCFGR_PLLM_0  | (85 << RCC_PLLCFGR_PLLN_Pos) | 0x00000000U);

    SET_BIT(RCC->CR, RCC_CR_PLLON);

    while(((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY));

    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLREN);

    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);

    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);

    while(((uint32_t)(READ_BIT(RCC->CFGR, RCC_CFGR_SWS)))!=RCC_CFGR_SWS_PLL);

    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV1);

    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV1);

    SysTick_Config(170000000/1000);

    SystemCoreClock = 170000000;
}

static void 
init_gpio(void){
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    GPIOC->MODER &= ~GPIO_MODER_MODER6;
	GPIOC->MODER |= GPIO_MODER_MODER6_0;

    GPIOC->OTYPER &= ~GPIO_OTYPER_OT_6;

    GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED6;

    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR6;

    GPIOC->BSRR |= GPIO_BSRR_BR_6;
}

//	set delay in miliseconds using sysTick timer
static void delay(uint32_t delay)
{
	delayTimerValue = delay;

	while(delayTimerValue);
}

int main(void){
    init_clock();

    init_gpio();
    while (1)
    {
        GPIOC->BSRR |= GPIO_BSRR_BS_6;
		delay(1000);

		GPIOC->BSRR |= GPIO_BSRR_BR_6;
		delay(1000);
    }
    
    return 0;
}