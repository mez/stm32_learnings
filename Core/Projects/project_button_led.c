#include "stm32f446xx.h"
#include "stm32f4xx_hal.h"

void project_button_led() {
    // Enable clock for GPIOA and GPIOC
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable clock for GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // enable clock for GPIOC

    // setup control register for led output on my board its PA5
    // Mode register
    GPIOA->MODER |= GPIO_MODER_MODE0_0; // set PA5 to output mode
    GPIOA->MODER &= ~GPIO_MODER_MODE0_1;

    // Output type register
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT0; // set PA5 to push-pull
   
    // Output speed register Low speed since its an led.
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR0; // Clear speed bits
    // Pull-up/pull-down register
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0; // No pull-up, no pull-down



    // setup control registers for the button input on my board its PC13
    //Mode register for input
    GPIOC->MODER &= ~GPIO_MODER_MODE13; // set PC13 to input mode
    
    //we set to 00 for no pull-up pull-down since the board already has a pull-up resistor.
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13; 

    // variables for debouncing.
    volatile char buttonPressed = 0;
    volatile int buttonPressedConfidenceLevel = 0;
    volatile int buttonReleasedConfidenceLevel = 0;
    volatile const int confidenceThreshold = 500;
    volatile char ledState = 0;

    while (1) {
        // read button state by using the IDR register (input data register)
        if ( !(GPIOC->IDR & GPIO_IDR_ID13) ) {

            if (buttonPressed == 0) {
                if (buttonPressedConfidenceLevel >= confidenceThreshold) {
                    if (ledState == 0) {
                        ledState = 1;
                        GPIOA->BSRR = GPIO_BSRR_BS0; // turn on led
                    } else {
                        ledState = 0;
                        GPIOA->BSRR = GPIO_BSRR_BR_0; // turn off led
                    }
                    buttonPressed = 1;
                } else {
                    buttonPressedConfidenceLevel++;
                    buttonReleasedConfidenceLevel= 0;
                }
            }
            // button pressed, means it was reading LOW
        } else {
            
            if (buttonPressed == 1) {

                if (buttonReleasedConfidenceLevel >= confidenceThreshold) {
                    buttonPressed = 0;
                } else {
                    buttonReleasedConfidenceLevel++;
                    buttonPressedConfidenceLevel=0;
                }
            }
            // GPIOA->BSRR = GPIO_BSRR_BR_0; // turn off led
        }
    }
}