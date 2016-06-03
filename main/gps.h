#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

#ifndef GPS_H
#define GPS_H

uint8_t p_SNSR_PWR = A1;

typedef struct gps_data_s
{

    uint8_t hour;
    uint8_t minute;
    uint8_t seconds;

    uint8_t day;
    uint8_t month;
    uint8_t year;

    float latdegrees;
    float londegrees;
    
    uint16_t altitude;


} gps_data_s;

void init_gps(void);
void read_gps(gps_data_s *data);
void gps_sleep(void);
void gps_wake(void);
void verify_gps_start(void);


#endif 
