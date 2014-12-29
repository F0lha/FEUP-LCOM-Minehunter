#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <machine/int86.h>
#include <stdlib.h>
#include <stdio.h>
//#include "ModRTC.h"

/*


unsigned long regA;
unsigned long regB;
unsigned long regC;

int scan_rtc(unsigned long index,  unsigned long *dado){
        if(sys_outb(RTC_ADDR_REG, index) != OK)
                return 1;
        if(sys_inb(RTC_DATA_REG, dado) != OK)
                return 1;
        return 0;
}

int write_rtc(unsigned long index, unsigned long dado){
        if(sys_outb(RTC_ADDR_REG, index) != OK)
                return 1;
        if(sys_outb(RTC_DATA_REG, dado) != OK)
                return 1;
        return 0;
}

void disable(){

        long unsigned int reg_b = scan_rtc(REG_B_RTC,&reg_b);
        reg_b |= BIT(7);
        write_rtc(REG_B_RTC, reg_b);

}


void enable(){
        long unsigned int reg_b = scan_rtc(REG_B_RTC,&reg_b);
        reg_b &= ~BIT(7);
        write_rtc(REG_B_RTC, reg_b);
}

void wait_valid_rtc(void) {
        unsigned long regA = 0;
        do {
                disable();
                sys_outb(RTC_ADDR_REG, REG_A_RTC);
                sys_inb(DATA_REG_RTC, &regA);
                enable();
        } while ( regA & RTC_UIP);
}

unsigned char bcd_to_binary(unsigned char bcdvalue)
{
        char buffer[32];
        sprintf(buffer, "%x", bcdvalue);
        sscanf(buffer, "%d", &bcdvalue);
        return bcdvalue;
}
unsigned char binary_to_bcd(unsigned char binaryvalue)
{
        char buffer[32];
        sprintf(buffer, "%d", binaryvalue);
        sscanf(buffer, "%x", &binaryvalue);
        return binaryvalue;
}

int rtc_date(){
        date data;

        disable();

        scan_rtc(REG_A_RTC, &regA);
        if(regA & RTC_UIP){
                wait_valid_rtc();
        }

        scan_rtc(RTC_SEC, &(sec));
        scan_rtc(RTC_MIN, &(min));
        scan_rtc(RTC_HOUR, &(hour));
        scan_rtc(RTC_DAY_MONTH, &(day_month));
        scan_rtc(RTC_DAY_WEEK, &(day_week));
        scan_rtc(RTC_MONTH, &(month));
        scan_rtc(RTC_YEAR, &(year));




        scan_rtc(REG_B_RTC, &regB);


        if(!(regB & RTC_DATA_MODE)) {

        	day_week = bcd_to_binary(day_week);
        	day_month = bcd_to_binary(day_month);
        	year = bcd_to_binary(year);
        	month = bcd_to_binary(month);


        	if(!(regB & BIT(1))){
        		if(hour & BIT(7)){
        			hour &= ~BIT(7);

        		}
        	}
        }

        enable();
        return 0;
}
*/
