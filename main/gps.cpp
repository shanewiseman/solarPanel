#include "gps.h"
SoftwareSerial mySerial(4, 3);


uint8_t p_SNSR_PWR = A1;

Adafruit_GPS GPS(&mySerial);
Adafruit_GPS *gpsObj = &GPS;

void init_gps(){

    Adafruit_GPS GPS(&mySerial);
    Adafruit_GPS *gpsObj = &GPS;

    pinMode(p_SNSR_PWR, OUTPUT);
    digitalWrite(p_SNSR_PWR, HIGH);
    
    gpsObj->begin( 9600 );
    gpsObj->sendCommand( PGCMD_NOANTENNA );
    gpsObj->sendCommand( PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ );
    gpsObj->sendCommand( PMTK_SET_NMEA_OUTPUT_RMCONLY );

    verify_gps_start();
    
}

void read_gps(gps_data_s *gps_data, rtc_data_s *rtc_data)
{
    /*TODO Should I check to see if gps is on? could just have a boolean somewhere */

    char c;
    do{
        while(! gpsObj->newNMEAreceived() ){
            c = gpsObj->read();
        }
    } while( ! gpsObj->newNMEAreceived() || !gpsObj->parse(gpsObj->lastNMEA()) );

    gps_data->latdegrees   = gpsObj->latitudeDegrees;
    gps_data->londegrees   = gpsObj->longitudeDegrees;
    gps_data->altitude     = gpsObj->altitude;

    rtc_data->hour         = gpsObj->hour;
    rtc_data->minutes      = gpsObj->minute;
    rtc_data->seconds      = gpsObj->seconds;
    rtc_data->day          = gpsObj->day;
    rtc_data->month        = gpsObj->month;
    rtc_data->year         = gpsObj->year;
    
}
void gps_sleep()
{
    digitalWrite(p_SNSR_PWR, LOW);
}
void gps_wake()
{
    digitalWrite(p_SNSR_PWR, HIGH);
    verify_gps_start();
}

void verify_gps_start()
{
    Serial.println("Waiting for GPS Start");
    gps_data_s gps_data;
    rtc_data_s rtc_data;

    read_gps(&gps_data, &rtc_data);

    while( gps_data.latdegrees == 0.00 ){
        Serial.println("Still Waiting...");
        delay(10000);
        read_gps(&gps_data, &rtc_data);
    }

}
