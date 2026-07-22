/*
 * File:   mkp.c
 * Author: HP
 *
 * Created on 21 June, 2026, 3:35 PM
 */


#include <xc.h>
#include "main.h"
#pragma config WDTE = OFF

void init_matrix_keypad(void)
{
    /* Setting the Columns as Inputs (RB2 - RB0)*/
    //TRISB = TRISB | 0x07
    MATRIX_KEYPAD_COLUMN_PORT_DDR = MATRIX_KEYPAD_COLUMN_PORT_DDR | 0x07;  
   
    
    /* Setting the Rows as Output (RD3 - RD0)*/
    MATRIX_KEYPAD_ROW_PORT_DDR = MATRIX_KEYPAD_ROW_PORT_DDR & 0xF0; // TRISD = TRISD & 0xF0 -> (TRISD0 to TRISD3)
    
    /* Enabling PORTB Pullups */
    nRBPU = 0;
    
    /* Making all the Rows HIGH to start with */
    ROW1 = HI;
    ROW2 = HI;
    ROW3 = HI;
    ROW4 = HI;
}

static unsigned char scan_keypad(void)
{
    int i;
    /* Scan Row 1 */
    ROW1 = LOW;
    ROW2 = HI;
    ROW3 = HI;
    ROW4 = HI;   
    
    for (i = 100; i--; );
    
    if (COL1 == LOW)
    {
        return 1;
    }
    else if (COL2 == LOW)
    {
        return 2;
    }
    else if (COL3 == LOW)
    {
        return 3;
    }
    /* Scan Row 2 */
    ROW1 = HI;
    ROW2 = LOW;
    ROW3 = HI;
    ROW4 = HI;   
    
    for (i = 100; i--; );
    
    if (COL1 == LOW)
    {
        return 4;
    }
    else if (COL2 == LOW)
    {
        return 5;
    }
    else if (COL3 == LOW)
    {
        return 6;
    }
    /* Scan Row 3 */
    ROW1 = HI;
    ROW2 = HI;
    ROW3 = LOW;
    ROW4 = HI;   

    for (i = 100; i--; );
    
    if (COL1 == LOW)
    {
        return 7;
    }
    else if (COL2 == LOW)
    {
        return 8;
    }
    else if (COL3 == LOW)
    {
        return 9;
    }
    /* Scan Row 4 */
    ROW1 = HI;
    ROW2 = HI;
    ROW3 = HI;
    ROW4 = LOW;   

    for (i = 100; i--; );
    
    if (COL1 == LOW)
    {
        return '*';
    }
    else if (COL2 == LOW)
    {
        return 0;
    }
    else if (COL3 == LOW)
    {
        return '#';
    }
    
    return ALL_RELEASED;
}

unsigned char read_matrix_keypad(unsigned char mode)
{
    static unsigned char once = 1;
    unsigned char key;
    /* Scan keypad */
    key = scan_keypad(); // 0 1 2 3 4 5 6 7 8 9 '*' '#'  ALL_RELEASED
    // Continuous key detection
    if (mode == LEVEL)
    {
        return key;
    }
    else
    {
        // Detect key only once
        if ((key != ALL_RELEASED) && once)
        {
            once = 0;
             return key;
        }
        // Wait until key is released 
        else if (key == ALL_RELEASED)
        {
            once = 1;
        }
    }
    
    return ALL_RELEASED;
}

