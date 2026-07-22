/* 
 * File:   main.h
 * Author: HP
 *
 * Created on 27 June, 2026, 10:32 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include "clcd.h"
#include "mkp.h"
#include "microwave.h"

#define DISP_ON_AND_CURSOR_ON           0x0E
#define DISP_ON_AND_CURSOR_BLINK        0x0F
#define CURSOR_HOME                     0x02

/* States */
typedef enum
{
    POWER_ON_SCREEN = 0,
    MENU_SCREEN,

    MICRO_POWER_SCREEN,

    SET_TIME_SCREEN,

    RUN_SCREEN,

    TIME_UP_SCREEN,

    SET_TEMP_SCREEN,

    PREHEAT_SCREEN,

    REHEAT_SCREEN

}State;


/* Global Variables */
extern State state;
extern unsigned char one_sec_flag;

/* Menu */
#define MK_SW1     1      // Move Up
#define MK_SW2     2      // Move Down

/* Run Mode */
#define MK_SW4     4      // Start / Resume
#define MK_SW5     5      // Pause
#define MK_SW6     6      // Stop

/* Special Keys */
#define MK_STAR    '*'
#define MK_HASH    '#'

#endif	/* MAIN_H */

