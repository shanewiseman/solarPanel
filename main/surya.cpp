#include "surya.h"

Helios h = Helios();

void getSuryaData(gps_data_s *gpsdata, surya_data_s *suryadata ){
    h.calcSunPos( gpsdata->year,
            gpsdata->month,
            gpsdata->day,
            ( gpsdata->hour ),
            gpsdata->minute,
            gpsdata->seconds,
            gpsdata->londegrees,
            gpsdata->latdegrees
       );

    suryadata->zenith  = (uint16_t) h.dZenithAngle;
    suryadata->azimuth = (uint16_t) h.dAzimuth;
}

void searchSuryaPosition(gps_data_s *gps_data, uint8_t angle ){
    
    uint8_t hour = 7;
    uint8_t minute = 0;
    uint8_t second = 0;

    do {
        h.calcSunPos( gps_data->year,
            gps_data->month,
            gps_data->day,
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
        h.calcSunPos( gps_data->year,
            gps_data->month,
            gps_data->day,
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
        h.calcSunPos( gps_data->year,
            gps_data->month,
            gps_data->day,
            hour,
            minute,
            second,
            gps_data->londegrees,
            gps_data->latdegrees
        );
        second++;
    } while( h.dZenithAngle > angle );
    second -= 2;
    
    gps_data->hour    = hour;
    gps_data->minute  = minute;
    gps_data->seconds = second;

}

