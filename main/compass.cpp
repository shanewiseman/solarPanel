#include "compass.h"

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

void init_compass(){
    
    if( ! mag.begin() ){
        Serial.println("COMPASS ERROR");
    }
}
void read_compass(compass_data_s *data){
    float declinationAngle = 0.22;

    sensors_event_t event; 
    mag.getEvent(&event);

    float heading = atan2(event.magnetic.y, event.magnetic.x);

    heading += declinationAngle;
    // Correct for when signs are reversed.
    if(heading < 0)
        heading += 2*PI;
    
    // Check for wrap due to addition of declination.
    if(heading > 2*PI)
        heading -= 2*PI;
   
    // Convert radians to degrees for readability.
    data->heading = heading * 180/M_PI;
}
