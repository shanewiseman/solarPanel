#include "surya.h"

Helios h = Helios();

void getSuryaData(gps_data_s *gpsdata, rtc_data_s *rtc_data, surya_data_s *suryadata ){
    h.calcSunPos( rtc_data->year,
            rtc_data->month,
            rtc_data->day,
            rtc_data->hour,
            rtc_data->minutes,
            rtc_data->seconds,
            gpsdata->londegrees,
            gpsdata->latdegrees
       );

    suryadata->zenith  = (uint16_t) h.dZenithAngle;
    suryadata->azimuth = (uint16_t) h.dAzimuth;
}

void searchSuryaPosition(gps_data_s *gps_data, rtc_data_s *rtc_data, uint8_t angle ){
    
    uint8_t hour = 7;
    uint8_t minute = 0;
    uint8_t second = 0;

    do {
        h.calcSunPos( rtc_data->year,
            rtc_data->month,
            rtc_data->day,
            hour,
            minute,
            second,
            gps_data->londegrees,
            gps_data->latdegrees
        );
        hour++;
    } while( h.dZenithAngle > angle );
    hour -= 2;

    do {
        h.calcSunPos( rtc_data->year,
            rtc_data->month,
            rtc_data->day,
            hour,
            minute,
            second,
            gps_data->londegrees,
            gps_data->latdegrees
        );
        minute++;
    } while( h.dZenithAngle > angle );
    minute -= 2;

    do {
        h.calcSunPos( rtc_data->year,
            rtc_data->month,
            rtc_data->day,
            hour,
            minute,
            second,
            gps_data->londegrees,
            gps_data->latdegrees
        );
        second++;
    } while( h.dZenithAngle > angle );
    second -= 2;
    
    rtc_data->hour    = hour;
    rtc_data->minutes  = minute;
    rtc_data->seconds = second;

}

