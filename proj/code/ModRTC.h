#ifndef __MODRTC_H
#define __MODRTC_H


#define BIT(n)                  (0x01<<(n))

#define RTC_ADDR_REG 	0x70
#define RTC_DATA_REG 	0x71
#define RTC_SEC 		0
#define RTC_MIN 		2
#define RTC_HOUR 		4
#define RTC_DAY_WEEK 	6
#define RTC_DAY_MONTH 	7
#define RTC_MONTH 		8
#define RTC_YEAR 		9
#define RTC_REG_A		10
#define RTC_REG_B		11
#define RTC_REG_C		12
#define RTC_REG_D		13
#define RTC_UIP                 BIT(7)
#define RTC_DATA_MODE   BIT(2)


unsigned long day_month;
unsigned long day_week;
unsigned long month;
unsigned long year;
unsigned long hour;
unsigned long min;
unsigned long sec;


int scan_rtc (unsigned long index, unsigned long *dado);

int write_rtc(unsigned long index, unsigned long dado);

















#endif
