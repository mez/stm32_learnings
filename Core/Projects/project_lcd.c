#include "stm32f446xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <stdint.h>


/**
 5V tolerant pins:

    PA0, PA1, PA15, PB7 , PA4, PA5, PA6, PA7

    PA0: done - brown
    PA1: done - red
    PA15: done - yellow
    PB7: done - white
    PA4: done - blue
    PA5: done - gray
    PA6: done - dark blue
    PA7: done - purple

*/

// define the LCD digital pins to microcontroller pins
#define LCDD0_PIN   GPIO_PIN_0  // PA0
#define LCDD0_PORT  GPIOA
#define LCDD1_PIN   GPIO_PIN_1  // PA1
#define LCDD1_PORT  GPIOA
#define LCDD2_PIN   GPIO_PIN_15  // PA15
#define LCDD2_PORT  GPIOA
#define LCDD3_PIN   GPIO_PIN_7  // PB7
#define LCDD3_PORT  GPIOB
#define LCDD4_PIN   GPIO_PIN_4  // PA4  
#define LCDD4_PORT  GPIOA
#define LCDD5_PIN   GPIO_PIN_5  // PA5
#define LCDD5_PORT  GPIOA
#define LCDD6_PIN   GPIO_PIN_6  // PA6
#define LCDD6_PORT  GPIOA
#define LCDD7_PIN   GPIO_PIN_7  // PA7
#define LCDD7_PORT  GPIOA

#define LCDRegisterSelect_PIN   GPIO_PIN_4  // PB4
#define LCDRegisterSelect_PORT  GPIOB
#define LCDReadWrite_PIN   GPIO_PIN_5  // PB5
#define LCDReadWrite_PORT  GPIOB
#define LCDEnable_PIN   GPIO_PIN_3  // PB3
#define LCDEnable_PORT  GPIOB

// define instruction codes
#define LCD_FUNCTION_SET_8BIT_2LINE 0b00111000
#define LCD_DISPLAY_ON_CURSOR_ON 0b00001110
#define LCD_ENTRY_MODE_SET 0b00000110
#define LCD_CLEAR_DISPLAY 0b00000001
#define LCD_SET_CGRAM_ADDRESS_SLOT0 0b01000000
#define LCD_SET_DDRAM_ADDRESS 0b10000000

// based on the bit state set the corresponding GPIO pin
void send_bit_to_GPIO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t bitState) {
    if (bitState) {
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
    }
}

//send the bits of a character to the corresponding GPIO pins using the defined mapping
void send_byte_to_LCD(char character) {
    send_bit_to_GPIO(LCDD0_PORT, LCDD0_PIN, character & 0b00000001);
    send_bit_to_GPIO(LCDD1_PORT, LCDD1_PIN, character & 0b00000010);
    send_bit_to_GPIO(LCDD2_PORT, LCDD2_PIN, character & 0b00000100);
    send_bit_to_GPIO(LCDD3_PORT, LCDD3_PIN, character & 0b00001000);
    send_bit_to_GPIO(LCDD4_PORT, LCDD4_PIN, character & 0b00010000);
    send_bit_to_GPIO(LCDD5_PORT, LCDD5_PIN, character & 0b00100000);
    send_bit_to_GPIO(LCDD6_PORT, LCDD6_PIN, character & 0b01000000);
    send_bit_to_GPIO(LCDD7_PORT, LCDD7_PIN, character & 0b10000000);

    // wait for a short period to allow LCD to read data
    HAL_Delay(1); // 1 millisecond delay

    // we have to reset enable pin to low after sending data
    HAL_GPIO_WritePin(LCDEnable_PORT, LCDEnable_PIN, GPIO_PIN_RESET);
}

void enable_LCD() {
 HAL_GPIO_WritePin(LCDEnable_PORT, LCDEnable_PIN, GPIO_PIN_SET);
 HAL_Delay(1);
}

void disable_LCD() {
 HAL_GPIO_WritePin(LCDEnable_PORT, LCDEnable_PIN, GPIO_PIN_RESET);
 HAL_Delay(1);
}

void set_LCD_to_write_mode() {
 HAL_GPIO_WritePin(LCDReadWrite_PORT, LCDReadWrite_PIN, GPIO_PIN_RESET);
}

void set_LCD_to_read_mode() {
 HAL_GPIO_WritePin(LCDReadWrite_PORT, LCDReadWrite_PIN, GPIO_PIN_SET);
 enable_LCD();
}

void set_LCD_to_instruction_mode() {
 HAL_GPIO_WritePin(LCDRegisterSelect_PORT, LCDRegisterSelect_PIN, GPIO_PIN_RESET);
}

void set_LCD_to_character_mode() {
 HAL_GPIO_WritePin(LCDRegisterSelect_PORT, LCDRegisterSelect_PIN, GPIO_PIN_SET);
}

void config_port_and_pin_for_output(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    // wow using the HAL is so much easier than direct register manipulation!!
    if (GPIOx == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (GPIOx == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (GPIOx == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (GPIOx == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    } else if (GPIOx == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    } else if (GPIOx == GPIOF) {
        __HAL_RCC_GPIOF_CLK_ENABLE();
    } else if (GPIOx == GPIOG) {
        __HAL_RCC_GPIOG_CLK_ENABLE();
    } else if (GPIOx == GPIOH) {
        __HAL_RCC_GPIOH_CLK_ENABLE();
    } 

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}


void send_character_to_LCD(char character) {
    set_LCD_to_write_mode();
    set_LCD_to_character_mode();
    enable_LCD();
    send_byte_to_LCD(character);
}

void send_instruction_to_LCD(char character) {
    set_LCD_to_write_mode();
    set_LCD_to_instruction_mode();
    enable_LCD();
    send_byte_to_LCD(character);
}

void send_text_to_LCD(const char* text) {
    while (*text) {
        send_character_to_LCD(*text++);
    }
}


void project_lcd() {

    // Configure gpio pins as output
    config_port_and_pin_for_output(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_15 |
                          GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
   
    config_port_and_pin_for_output(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7);

    // first we set to 8-bit mode.
    send_instruction_to_LCD(LCD_FUNCTION_SET_8BIT_2LINE);
    send_instruction_to_LCD(LCD_DISPLAY_ON_CURSOR_ON);
    send_instruction_to_LCD(LCD_ENTRY_MODE_SET);
    send_instruction_to_LCD(LCD_CLEAR_DISPLAY);


   send_text_to_LCD("Hello World!");
   
        
    while (1) {
        // Main loop
    };
}