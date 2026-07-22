/* 
 * File:   microwave.h
 * Author: HP
 *
 * Created on 27 June, 2026, 10:45 AM
 */

#ifndef MICROWAVE_H
#define	MICROWAVE_H

/* Menu options */
#define MICRO          0
#define GRILL          1
#define CONVECTION     2
#define REHEAT         3

/* Global Variables */
extern unsigned char menu_pos;
extern unsigned char mode;
extern unsigned char minute;
extern unsigned char second;
extern unsigned char temperature;
extern unsigned char running;
extern unsigned char paused;
extern unsigned int delay_count;

/* Functions */
void power_on_screen(void);
void menu_screen(unsigned char key);
void micro_power_screen(void);
void set_time_screen(unsigned char key);
void run_screen(unsigned char key);
void time_up_screen(void);
void set_time_screen(unsigned char key);
void preheat_screen(void);
void set_temp_screen(unsigned char key);
void reheat_screen(unsigned char key);


#endif	/* MICROWAVE_H */

