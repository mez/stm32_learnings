#include "LCDio.h"
#include "stm32f446xx.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>

// factory calibration values (from datasheet)
#define TS_CAL1  (*(uint16_t*)0x1FFF7A2C)  // ADC value at 30°C, VDDA=3.3V
#define TS_CAL2  (*(uint16_t*)0x1FFF7A2E)  // ADC value at 110°C, VDDA=3.3V

char* get_temp_string(uint16_t adc_value) {
    static char temp_string[20];
    // linear interpolation using factory calibration
    float temperature = 30.0f + (80.0f * (float)(adc_value - TS_CAL1)) 
                                       / (float)(TS_CAL2 - TS_CAL1);
    snprintf(temp_string, sizeof(temp_string), "Temp: %.1f C", temperature);
    return temp_string;
}

void project_adc() {

    initialize_LCD_pins();
    initialize_LCD_display();

    send_text_to_LCD("ADC Intro !");

    // ADC calibration is not needed for STM32F4 series

    // select a clock source for the ADC? (need to learn more about clocks)
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // enable ADC1 clock
    // ADC enable
    //since system clock is 84MHz, set prescaler to divide by 4 to get 21MHz ADC clock
    ADC->CCR |= ADC_CCR_ADCPRE_0;
    //enable temperature sensor and Vrefint
    ADC->CCR |= ADC_CCR_TSVREFE;
    //disable Vbat channel
    ADC->CCR &= ~ADC_CCR_VBATE;
    // set sample time for channel 18 to 480 cycles (23us) because minimum sample time for temperature sensor is 10us
    ADC->CCR |= ADC_SMPR1_SMP18;

    //select channel 18 (temperature sensor) for ADC1
    ADC1->SQR3 = 18; // channel 18 is 10010
    ADC1->SQR1 = 0; // we tell ADC we have only 1 conversion in the regular sequence. st does a +1 to this value

    ADC1->CR2 |= ADC_CR2_ADON;
    HAL_Delay(1);
    // Start ADC conversion
        
    while (1) {
        // Main loop
        ADC1->CR2 |= ADC_CR2_SWSTART; // start conversion

        // now we wait for cycles to complete conversion
        while ( !(ADC1->SR & ADC_SR_EOC)) {}
        send_instruction_to_LCD(LCD_CLEAR_DISPLAY);

        uint16_t adc_value = ADC1->DR; // read converted value
        send_text_to_LCD(get_temp_string(adc_value));
        HAL_Delay(1000); // wait 1 second before next reading
        send_instruction_to_LCD(LCD_CLEAR_DISPLAY);
    };
}