//  projects.h - header file for project selection and declarations
// I got tired of creating a new codebase for each tiny project and learnings. This
// file is just to switch what code is running on the mcu.
#pragma once


#define PROJECT_LED_TOGGLE 1
#define PROJECT_BUTTON_LED 2
#define PROJECT_LCD 3
#define PROJECT_ADC 4

// set this to one of the above project defines and it will run that project.
#define ACTIVE_PROJECT PROJECT_ADC

/**
    Project 1: Hello World aka LED Toggle
    The goal is to set the gpio registers directly to toggle an led.
*/
void project_led_toggle(void);

/**
    Project 2: Button controlled LED
    The goal is to read a button state and control an led based on that.
*/
void project_button_led(void);

/**
    Project 3: LCD Display
    The goal is to initialize and display something on an LCD.
*/
void project_lcd(void);

/**
    Project 4: ADC Tutorial
    The goal is to read the internal temperature sensor using the ADC.
*/
void project_adc(void);

static inline void run_active_project(void) {
#if ACTIVE_PROJECT == PROJECT_LED_TOGGLE
    project_led_toggle();
#elif ACTIVE_PROJECT == PROJECT_BUTTON_LED
    project_button_led();
#elif ACTIVE_PROJECT == PROJECT_LCD
    project_lcd();
#elif ACTIVE_PROJECT == PROJECT_ADC
    project_adc();  
#else
    #error "No valid project selected"
#endif
}