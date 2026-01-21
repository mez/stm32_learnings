#include "stm32f446xx.h"
#include "stm32f4xx_hal.h"

void project_led_toggle(void)
{
  // we need to enable port A pin 5
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable GPIOA clock

  // mode register to put is 01 for output
  GPIOA->MODER |= GPIO_MODER_MODE5_0; // Set PA5 to output mode
  GPIOA->MODER &= ~GPIO_MODER_MODE5_1;

  // output type register
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT5; // Set PA5 to push-pull

  // output speed register Low speed
  GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR5_0|GPIO_OSPEEDER_OSPEEDR5_1); // Clear speed bits

  // pull-up/pull-down register
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5_0; // No pull-up, no pull-down
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5_1;

  // lets turn on the LED
  while (1)
  {
    HAL_Delay(1000);
    GPIOA->ODR ^= GPIO_ODR_OD5; // toggle PA5
  }
}