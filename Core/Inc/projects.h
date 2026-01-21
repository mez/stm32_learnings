//  projects.h - header file for project selection and declarations
// I got tired of creating a new codebase for each tiny project and learnings. This
// file is just to switch what code is running on the mcu.
#pragma once


#define PROJECT_LED_TOGGLE 1
#define PROJECT_BUTTON_LED 2

// set this to one of the above project defines and it will run that project.
#define ACTIVE_PROJECT PROJECT_LED_TOGGLE

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


static inline void run_active_project(void) {
#if ACTIVE_PROJECT == PROJECT_LED_TOGGLE
    project_led_toggle();
#elif ACTIVE_PROJECT == PROJECT_BUTTON_LED
    project_button_led();
#else
    #error "No valid project selected"
#endif
}