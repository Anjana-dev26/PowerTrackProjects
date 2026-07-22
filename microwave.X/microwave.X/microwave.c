#include "main.h"


/* Global Variables */

unsigned char menu_pos = 0;
unsigned char mode = MICRO;
unsigned char minute = 0;
unsigned char second = 0;
unsigned char temperature = 0;
unsigned char running = 0;
unsigned char paused = 0;
unsigned int delay_count = 0;
unsigned int temp = 0;
unsigned char temp_digit = 0;

/*POWER ON SCREEN*/
void power_on_screen(void)
{
    static unsigned char once = 1;
    static unsigned char i = 0;

    // Display welcome screen only once
    if(once)
    {
        clcd_clear();

        clcd_print("Power ON", LINE2(4));
        clcd_print("Microwave Oven", LINE3(1));

        once = 0;
    }

    // Animate top and bottom borders
    if(delay_count >= 80)
    {
        delay_count = 0;

        if(i < 16)
        {
            clcd_putch(0xFF, LINE1(i));
            clcd_putch(0xFF, LINE4(i));
            i++;
        }
        else
        {
            // Animation complete
            i = 0;
            once = 1;
            state = MENU_SCREEN;
            clcd_clear();
        }
    }
}


void menu_screen(unsigned char key)
{
    static unsigned char once = 1;

    if(once)
    {
        clcd_clear();

        clcd_print("1.Micro", LINE1(0));
        clcd_print("2.Grill", LINE2(0));
        clcd_print("3.Conv",  LINE3(0));
        clcd_print("4.Start",LINE4(0));

        once = 0;
    }

    switch(key)
    {
        case 1:                 // Micro
            mode = MICRO;
            state = MICRO_POWER_SCREEN;
            once = 1;
            clcd_clear();
            break;

        case 2:                 // Grill
            mode = GRILL;
            state = SET_TIME_SCREEN;
            once = 1;
            clcd_clear();
            break;

        case 3:                 // Convection
            mode = CONVECTION;
            state = SET_TEMP_SCREEN;
            once = 1;
            clcd_clear();
            break;

        case 4:                 // Reheat
            mode = REHEAT;
            minute = 0;
            second = 30;
            state = RUN_SCREEN;
            once = 1;
            clcd_clear();
            break;
    }
}

void micro_power_screen(void)
{
    static unsigned char once = 1;
    static unsigned int time = 0;

    if(once)
    {
        clcd_clear();

        clcd_print(" MICRO MODE ", LINE2(2));
        clcd_print("POWER =1000W", LINE3(2));

        once = 0;
        time = 0;
    }

    if(delay_count >= 1)
    {
        delay_count = 0;

        if(++time >= 2000)      //2 seconds
        {
            once = 1;

            minute = 0;
            second = 0;

            state = SET_TIME_SCREEN;

            clcd_clear();
        }
    }
}

void set_time_screen(unsigned char key)
{
    static unsigned char once = 1;

    static unsigned char digit = 0;

    static unsigned char mm_t = 0;
    static unsigned char mm_u = 0;
    static unsigned char ss_t = 0;
    static unsigned char ss_u = 0;

    static unsigned char blink = 0;
    static unsigned int blink_time = 0;

    unsigned char value;

    if(once)
    {
        clcd_clear();

        clcd_print("SET TIME<MM:SS>",LINE1(0));
        clcd_print("TIME:00:00",LINE2(0));
        clcd_print("*:CLR   #:ENT",LINE4(0));

        digit = 0;

        mm_t = 0;
        mm_u = 0;
        ss_t = 0;
        ss_u = 0;

        blink = 1;

        once = 0;
    }

    /* blink */

    if(delay_count >= 1)
    {
        delay_count = 0;

        if(++blink_time >= 500)
        {
            blink_time = 0;

            blink ^= 1;
        }
    }

    /* display */

    clcd_putch(mm_t + '0',LINE2(5));
    clcd_putch(mm_u + '0',LINE2(6));

    clcd_putch(':',LINE2(7));

    clcd_putch(ss_t + '0',LINE2(8));
    clcd_putch(ss_u + '0',LINE2(9));

    if(!blink)
    {
        switch(digit)
        {
            case 0:
                clcd_putch(' ',LINE2(8));
                break;

            case 1:
                clcd_putch(' ',LINE2(9));
                break;

            case 2:
                clcd_putch(' ',LINE2(5));
                break;

            case 3:
                clcd_putch(' ',LINE2(6));
                break;
        }
    }

    /* clear */

    if(key == MK_STAR)
    {
        digit = 0;

        mm_t = 0;
        mm_u = 0;
        ss_t = 0;
        ss_u = 0;
    }

    /* numeric */

    if(key <= 9)
    {
        value = key;

        switch(digit)
        {
            case 0:
                ss_t = value;
                digit++;
                break;

            case 1:
                ss_u = value;
                digit++;
                break;

            case 2:
                mm_t = value;
                digit++;
                break;

            case 3:
                mm_u = value;
                digit = 4;
                break;
        }
    }

    /* enter */

    if((key == MK_HASH) && (digit == 4))
    {
        minute = (mm_t * 10) + mm_u;
        second = (ss_t * 10) + ss_u;

        running = 0;
        paused = 0;

        once = 1;

        state = RUN_SCREEN;

        clcd_clear();
    }
}

void run_screen(unsigned char key)
{
    static unsigned char once = 1;

    if(once)
    {
        clcd_clear();
        once = 0;
    }

    /* 4 : Start / Pause / Resume */
    if(key == 4)
    {
        if(running == 0)
        {
            /* Start or Resume */
            running = 1;
        }
        else
        {
            /* Pause */
            running = 0;
        }
    }

    /* 5 : Add 30 seconds */
    if(key == 5)
    {
        second += 30;

        if(second >= 60)
        {
            minute += second / 60;
            second %= 60;
        }
    }

    /* 6 : Stop */
    if(key == 6)
    {
        running = 0;
        once = 1;
        state = TIME_UP_SCREEN;
        clcd_clear();
        return;
    }

    /* Countdown */
    if(one_sec_flag)
    {
        one_sec_flag = 0;

        if(running)
        {
            if(second > 0)
            {
                second--;
            }
            else
            {
                if(minute > 0)
                {
                    minute--;
                    second = 59;
                }
                else
                {
                    running = 0;
                    once = 1;
                    state = TIME_UP_SCREEN;
                    clcd_clear();
                    return;
                }
            }
        }
    }

    /* Display */
    clcd_print("TIME=", LINE1(0));
    clcd_putch((minute / 10) + '0', LINE1(5));
    clcd_putch((minute % 10) + '0', LINE1(6));
    clcd_putch(':', LINE1(7));
    clcd_putch((second / 10) + '0', LINE1(8));
    clcd_putch((second % 10) + '0', LINE1(9));

    //if(running)
      //  clcd_print("4:PAUSE", LINE2(0));
    //else
        clcd_print("4.START/RESUME", LINE2(0));

    clcd_print("5.PAUSE", LINE3(0));
    clcd_print("6.STOP", LINE4(0));
}

void time_up_screen(void)
{
    static unsigned char once = 1;
    static unsigned int timer = 0;

    if(once)
    {
        clcd_clear();

        clcd_print("   TIME UP!!   ",LINE2(0));
        clcd_print("ENJOY YOUR MEAL",LINE3(0));

        timer = 0;
        once = 0;
    }

    if(one_sec_flag)
    {
        one_sec_flag = 0;

        timer++;

        if(timer >= 3)
        {
            once = 1;
            menu_pos = 0;

            state = MENU_SCREEN;

            clcd_clear();
        }
    }
}

void preheat_screen(void)
{
    /* Temporary implementation */

    clcd_clear();

    clcd_print("PREHEATING", LINE2(2));

    __delay_ms(1000);

    state = SET_TIME_SCREEN;
}

void reheat_screen(unsigned char key)
{
    minute = 0;
    second = 30;

    state = RUN_SCREEN;
}

void set_temp_screen(unsigned char key)
{
    static unsigned char once = 1;
    unsigned char value;

    if(once)
    {
        clcd_clear();
        temp = 0;
        temp_digit = 0;
        once = 0;
    }

    /* Display */

    clcd_print("SET TEMP <*C>", LINE1(0));

    clcd_print("TEMP = ", LINE2(0));

    clcd_putch((temp / 100) + '0', LINE2(7));
    clcd_putch(((temp / 10) % 10) + '0', LINE2(8));
    clcd_putch((temp % 10) + '0', LINE2(9));

    clcd_print("*:CLEAR", LINE4(0));
    clcd_print("#:ENTER", LINE4(9));

    /* CLEAR */

    if(key == '*')
    {
        temp = 0;
        temp_digit = 0;
    }

    /* Numeric key */

    if(key <= 9)
    {
        if(temp_digit < 3)
        {
            value = key;

            temp = temp * 10 + value;

            if(temp > 180)
            {
                temp = 180;
            }

            temp_digit++;
        }
    }

    /* ENTER */

    if(key == '#')
    {
        if(temp > 0)
        {
            once = 1;

            clcd_clear();

            state = SET_TIME_SCREEN;
        }
    }
}