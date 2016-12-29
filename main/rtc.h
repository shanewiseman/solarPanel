#include<Arduino.h>
#include <Wire.h>
#define I2C_ADDRESS 0x68 
#define TIME_REG    0x00
#define ALARM_REG   0x07
#define CNTRL_REG   0x0E
#define STATS_REG   0x0F


#ifndef RTC_H
#define RTC_H
typedef struct rtc_data_s 
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint16_t year;

} rtc_data_s;

void init_rtc(void);
void set_ctime(rtc_data_s *rtc_data);
void get_ctime(rtc_data_s *rtc_data);
void set_alarm(rtc_data_s *rtc_data, uint8_t dayoffset = 1);
void remove_alarm(void);

uint8_t bin2bcd(uint8_t data);
uint8_t bcd2bin(uint8_t data);


uint8_t getSeconds(void);
uint8_t getMinutes(void);
uint8_t getHour(void);
uint8_t getDayWeek(void);
uint8_t getDayMonth(void);
uint8_t getMonth(void);
uint8_t getYear(void);


//see https://cdn-shop.adafruit.com/product-files/3013/DS3231.pdf for chip documentation
#endif
