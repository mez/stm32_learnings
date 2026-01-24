#include "LCDio.h"


void project_lcd() {

    initialize_LCD_pins();
    initialize_LCD_display();

    send_text_to_LCD("Hello LCDio.h!");
        
    while (1) {
        // Main loop
    };
}