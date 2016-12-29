#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include "rtc.h"

#ifndef GPS_H
#define GPS_H

typedef struct gps_data_s
{
    float latdegrees;
    float londegrees;
    
    uint16_t altitude;

} gps_data_s;

void init_gps(void);
void read_gps(gps_data_s *gps_data, rtc_data_s *rtc_data);
void gps_sleep(void);
void gps_wake(void);
void verify_gps_start(void);


#endif 
