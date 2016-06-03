#include "gps.h"
SoftwareSerial mySerial(4, 3);

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

void read_gps(gps_data_s *data)
{
    char c;
    do{
        while(! gpsObj->newNMEAreceived() ){
            c = gpsObj->read();
        }
    } while( ! gpsObj->newNMEAreceived() || !gpsObj->parse(gpsObj->lastNMEA()) );

    data->latdegrees   = gpsObj->latitudeDegrees;
    data->londegrees   = gpsObj->longitudeDegrees;
    data->hour         = gpsObj->hour;
    data->minute       = gpsObj->minute;
    data->seconds      = gpsObj->seconds;
    data->day          = gpsObj->day;
    data->month        = gpsObj->month;
    data->year         = gpsObj->year;
    data->altitude     = gpsObj->altitude;
    
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

    read_gps(&gps_data);

    while( gps_data->latdegrees < 1 ){
        Serial.println("Still Waiting...");
        delay(1000);
        read_gps(&gps_data);
    }

}
