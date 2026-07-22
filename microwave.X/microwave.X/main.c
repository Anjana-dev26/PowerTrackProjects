/*
 * Name : Anjana V
 * Description :
 * Microwave Oven Simulation using PIC16F877A, 16×4 CLCD, Matrix Keypad, and PICSimLab.
 * The system provides a menu-driven interface through a 16×4 Character LCD (CLCD) 
 * and a 4×3 Matrix Keypad, allowing the user to select different cooking modes 
 * and control the cooking process
        Main Menu
            1 ? Micro
            2 ? Grill
            3 ? Convection
            4 ? Start
        Set Time Screen
            0?9 ? Enter cooking time.
            * ? Clear the entered time.
            # ? Confirm the time and proceed to the Run screen.
        Set Temperature Screen (Convection)
            0?9 ? Enter temperature (maximum 180°C).
            * ? Clear the entered temperature.
            # ? Confirm the temperature and proceed to the Set Time screen.
        Run Screen
            4 ? Start the timer if stopped, Pause if running, Resume if paused.
            5 ? Add 30 seconds to the remaining cooking time.
            6 ? Stop cooking and display the Time Up screen.
 */



#pragma config WDTE = OFF


#include "main.h"

/* Global Variables */
State state = POWER_ON_SCREEN;
unsigned char mode = 0;
unsigned char minute = 0;
unsigned char second = 0;
unsigned char temperature = 0;
unsigned char running = 0;
unsigned char paused = 0;
unsigned char one_sec_flag = 0;

void init_timer_config(void) {
    OPTION_REG = 0x04;      // Prescaler 
    TMR0 = 100;     // Preload value
    TMR0IF = 0;     // Clear interrupt flag
    TMR0IE = 1;     // Enable Timer0 interrupt
}
void __interrupt() isr(void)
{
    static unsigned int ms = 0;

    if(TMR0IF)
    {
        TMR0IF = 0;     // Clear interrupt flag
        TMR0 = 100;      // Reload Timer0
        delay_count++;
        ms++;
        // One second elapsed
        if(ms >= 1000)
        {
            ms = 0;
            one_sec_flag = 1;
        }
    }
}

/* Initialize all peripherals */
static void init_config(void) {
    init_clcd();
    init_matrix_keypad();
    init_timer_config();
    GIE = 1;     // Enable Global Interrupt
    PEIE = 1;   // Enable Peripheral Interrupt
}

void main(void) {
    unsigned char key;
    init_config();

    while(1)
    {
        // Read keypad
        key = read_matrix_keypad(STATE);
        // Execute current screen
        switch(state)
        {
            case POWER_ON_SCREEN:
                power_on_screen();
                break;

            case MENU_SCREEN:
                menu_screen(key);
                break;

            case MICRO_POWER_SCREEN:
                micro_power_screen();
                break;

            case SET_TIME_SCREEN:
                set_time_screen(key);
                break;

            case RUN_SCREEN:
                run_screen(key);
                break;

            case TIME_UP_SCREEN:
                time_up_screen();
                break;

            case SET_TEMP_SCREEN:
                set_temp_screen(key);
                break;

            case PREHEAT_SCREEN:
                preheat_screen();
                break;

            case REHEAT_SCREEN:
                reheat_screen(key);
                break;

            default:
                state = MENU_SCREEN;
                break;
        }
    }
    return;
}
