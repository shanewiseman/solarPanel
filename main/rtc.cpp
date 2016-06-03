#include "rtc.h"

void init_rtc(){
    Wire.begin();
    remove_alarm();
}

void set_ctime(rtc_data_s *rtc_data ){
    Wire.beginTransmission(I2C_ADDRESS);
    
    Wire.write((uint8_t) TIME_REG ); 

/*  --------------------------------------------------------------------------------------------  */
// seconds
    Wire.write(bin2bcd(rtc_data->seconds) & 0x7F);

/*  --------------------------------------------------------------------------------------------  */
// minutes 
    Wire.write( bin2bcd(rtc_data->minutes) & 0x7F);

/*  --------------------------------------------------------------------------------------------  */
// hour
    if( rtc_data->hour / 20 ){
        Wire.write( ( 0x60 | ( (rtc_data->hour % 20) & 0x0F ) ) & 0x7F );
    } else if( rtc_data->hour / 10 ){
        Wire.write( ( 0x50 | ( (rtc_data->hour % 10) & 0x0F ) ) & 0x7F );
    } else {
        Wire.write( ( 0x40 | ( rtc_data->hour & 0x0F ) ) & 0x7F );
    }

/*  --------------------------------------------------------------------------------------------  */
// day of week
    Wire.write(0x01 & 0x07);

/*  --------------------------------------------------------------------------------------------  */
// day of month 
    Wire.write(bin2bcd( rtc_data->day ) & 0x3F );

/*  --------------------------------------------------------------------------------------------  */
// month
    Wire.write( bin2bcd( rtc_data->month ) & 0x1F );

/*  --------------------------------------------------------------------------------------------  */
//  year
    Wire.write( bin2bcd( ( rtc_data->year ) ) & 0xFF );

/*  --------------------------------------------------------------------------------------------  */

    Wire.endTransmission();
}
uint8_t bin2bcd(uint8_t data){
    
    return ( (uint8_t)( data / 10 ) << 4 ) | ( (uint8_t)( data % 10 ) & 0x0F ); 

}
uint8_t bcd2bin(uint8_t data){
    return (uint8_t)( (uint8_t)(( data & 0xF0 ) >> 4 ) * 10 ) + (uint8_t)(data & 0x0F);
}
void get_ctime(rtc_data_s *rtc_data){
    
    rtc_data->seconds = getSeconds();
    rtc_data->minutes = getMinutes();
    rtc_data->hour    = getHour();
    rtc_data->day     = getDayMonth();
    rtc_data->month   = getMonth();
    rtc_data->year    = getYear();

}
uint8_t getSeconds(){

    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.requestFrom(I2C_ADDRESS, 1 );
    
    if (! Wire.available()) Serial.println("ERROR");

    return (uint8_t) (bcd2bin( Wire.read() & 0xFF ));

}
uint8_t getMinutes(){

    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x01);
    Wire.endTransmission();

    Wire.requestFrom(I2C_ADDRESS, 1 );
    
    return (uint8_t) (bcd2bin( Wire.read() & 0xFF ));

}
uint8_t getHour(){

    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x02);
    Wire.endTransmission();

    Wire.requestFrom(I2C_ADDRESS, 1 );
    
    uint8_t buffer = 0x00;

    buffer = Wire.read();

    if( buffer & 0x20 ){
        buffer = 20 + ( (uint8_t)(buffer & 0x0F) );
    } else if( buffer & 0x10 ){
        buffer = 10 + ( (uint8_t)(buffer & 0x0F) );
    } else {
        buffer =  ( (uint8_t)(buffer & 0x0F) );
    }

    return buffer;
}
uint8_t getDayWeek(){

    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x03);
    Wire.endTransmission();

    Wire.requestFrom(I2C_ADDRESS, 1 );
    
    return (uint8_t) (Wire.read() & 0x03);

}
uint8_t getDayMonth(){

    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x04);
    Wire.endTransmission();

    Wire.requestFrom(I2C_ADDRESS, 1 );
    
    return (uint8_t) (bcd2bin( Wire.read() & 0x3F ));

}
uint8_t getMonth(){

    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x05);
    Wire.endTransmission();

    Wire.requestFrom(I2C_ADDRESS, 1 );
    
    return (uint8_t) (bcd2bin( Wire.read() & 0x1F ));

}
uint8_t getYear(){

    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x06);
    Wire.endTransmission();

    Wire.requestFrom(I2C_ADDRESS, 1 );
    
    return (uint8_t) (bcd2bin( Wire.read() & 0xFF ));

}
void set_alarm(rtc_data_s *rtc_data, uint8_t dayoffset){
    
    Wire.beginTransmission(I2C_ADDRESS);
    
    Wire.write((uint8_t) ALARM_REG ); 

/*  --------------------------------------------------------------------------------------------  */
// seconds
    Wire.write( ( bin2bcd(rtc_data->seconds) | 0x00 ) & 0xFF);
/*  --------------------------------------------------------------------------------------------  */
// minutes 
    Wire.write( ( bin2bcd( rtc_data->minutes ) | 0x00 ) & 0xFF);

/*  --------------------------------------------------------------------------------------------  */
// hour
    if( rtc_data->hour / 20 ){
        //          12/24 + 20h                                 DY/DT                            
        Wire.write( ( 0x60 | ( (rtc_data->hour % 20) & 0x0F ) | 0x00 ) & 0xFF );
    } else if( rtc_data->hour / 10 ){
        Wire.write( ( 0x50 | ( (rtc_data->hour % 10) & 0x0F ) | 0x00 ) & 0xFF );
    } else {
        Wire.write( ( 0x40 | ( rtc_data->hour & 0x0F ) | 0x00 ) & 0xFF );
    }
    

    Wire.endTransmission();
/*  --------------------------------------------------------------------------------------------  */
//day
    uint8_t dayofweek = getDayWeek();

    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write((uint8_t) ALARM_REG + 3 ); 

    //increments to next day, sets DOW flag
    Wire.write( ( ( ( ( dayofweek + dayoffset ) & 0x0F ) | 0x00 ) | 0x40 ) & 0xCF );

    Wire.endTransmission();
/*  --------------------------------------------------------------------------------------------  */

    Wire.endTransmission();

    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write((uint8_t) CNTRL_REG );
    Wire.write( 0x1D );
    Wire.endTransmission();

}
void remove_alarm(){

    Wire.beginTransmission(I2C_ADDRESS);
    
    Wire.write((uint8_t) ALARM_REG ); 

    for( uint16_t i = 7; i <= 10; i++ ){
        Wire.write(0x00);
    }

    Wire.endTransmission();

    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write((uint8_t) CNTRL_REG );
    Wire.write( 0x1C );
    Wire.endTransmission();
    
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write((uint8_t) STATS_REG );
    Wire.write( 0x88 );
    Wire.endTransmission();
       

}
