#include "accelerometer.h"

Adafruit_MMA8451 accel = Adafruit_MMA8451();

void init_accel()
{
    if( ! accel.begin() ){
        //Error Logic
        Serial.println("ACCEL ERROR");
        while(1);
    }

    accel.setRange(MMA8451_RANGE_2_G);
}
void read_accel(accel_data_s *accel_data)
{
    accel.read();

    //front to back
    accel_data->verticle = (uint8_t)abs( ( (float)accel.x / 4096 ) * 90 );
    
    //side to side gangster
    accel_data->horizontal = (uint8_t) abs( ( (float)accel.y / 4096 ) * 90 );
    
}
