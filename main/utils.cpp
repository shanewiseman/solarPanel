#include "main.h"

uint8_t p_PWR = A0;
uint8_t p_INT = 2;
uint8_t p_LED = 13;
uint8_t p_ERROR[] = {
    A2,
    A3,
    A4,
    A5
};

void initialize()
{

    //ERROR OUTPUT
    for( int i = 0; i < sizeof(p_ERROR); i++ ){
        pinMode( p_ERROR[i], OUTPUT );
        digitalWrite( p_ERROR[i], LOW);
    }

    //LED
    pinMode(p_LED, OUTPUT);
    digitalWrite(p_LED,LOW);

    //POWER MONITOR
    pinMode(p_PWR, INPUT);
    if( !( checkPower(p_PWR) ) ){
        int error = 1;
        postError(error);
    }

    //INTERRUPT PIN
    pinMode(p_INT, INPUT);
}
void postError( uint8_t error_code )
{
    if( error_code & 1 ){
        digitalWrite( p_ERROR[0], HIGH ); 
    }
    if( error_code & 2 ){
        digitalWrite( p_ERROR[1], HIGH ); 
    }
    if( error_code & 4 ){
        digitalWrite( p_ERROR[2], HIGH ); 
    }
    if( error_code & 8 ){
        digitalWrite( p_ERROR[3], HIGH ); 
    }

    while( 1 ){
        digitalWrite(p_LED, HIGH);
        delay(100);
        digitalWrite(p_LED, LOW);
        delay(100);
    }
}

bool checkPower(uint8_t pwrpin)
{   
    //Checks for 4.85V
    int VOLT = 993;
    if( analogRead(pwrpin) < VOLT ){
        //return 0;
        return 1;
    }
    return 1;
}

void performSleep(rtc_data_s *rtc_data, surya_data_s *surya_data, gps_data_s *gps_data ){

//http://donalmorrissey.blogspot.com/2010/04/putting-arduino-diecimila-to-sleep-part.html

    rtc_data->seconds = gps_data->seconds;
    rtc_data->minutes = gps_data->minute;
    rtc_data->hour    = gps_data->hour;
    rtc_data->day     = gps_data->day;
    rtc_data->month   = gps_data->month;
    rtc_data->year    = gps_data->year;

    set_ctime(rtc_data);

    Serial.println(rtc_data->hour);

    searchSuryaPosition(gps_data, SLEEP_ZENITH );

    getSuryaData(gps_data, surya_data);

    orientationCorrection(surya_data);

    rtc_data->seconds = gps_data->seconds;
    rtc_data->minutes = gps_data->minute;
    rtc_data->hour    = gps_data->hour;

   
    set_alarm(rtc_data, 1);
 
/*
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write((uint8_t) ALARM_REG );
    Wire.endTransmission();
    Wire.requestFrom(I2C_ADDRESS,4);
        
    for(int i = 0; i < 4 ; i++ ){
        Serial.println( Wire.read(), HEX);
    }
*/  
    gps_sleep();

    attachInterrupt(digitalPinToInterrupt( p_INT ) , &awakenSleep,  FALLING);
    delay(100);
    
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      
    sleep_enable();
        
    sleep_mode();
          
    /* The program will continue from here. */
          
    /* First thing to do is disable sleep. */
    sleep_disable();
/*
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write((uint8_t) STATS_REG );
    Wire.endTransmission();
    Wire.requestFrom(I2C_ADDRESS,1);
    Serial.println( Wire.read(), HEX );
*/
    remove_alarm();
    gps_wake();
    Serial.println("AWAKE!");


}
void awakenSleep(){
    detachInterrupt(digitalPinToInterrupt( p_INT ));
}









