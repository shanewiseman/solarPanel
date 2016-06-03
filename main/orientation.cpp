#include "orientation.h"

uint8_t azimuth_motor_p[] = {
    5,
    6,
    7,
    8
};
uint8_t zenith_motor_p[] = {
    9,
    10,
    11,
    12
};


CustomStepper azimuth_step( 
        azimuth_motor_p[0],
        azimuth_motor_p[1],
        azimuth_motor_p[2],
        azimuth_motor_p[3],
        (byte[]) {4,B1100,B0110,B0011,B1001}, 2037.88641975, 2, CW
    );
CustomStepper zenith_step( 
        zenith_motor_p[0],
        zenith_motor_p[1],
        zenith_motor_p[2],
        zenith_motor_p[3],
        (byte[]) {4,B1100,B0110,B0011,B1001}, 2037.88641975, 2, CW
    );

void init_azimuth()
{
    //while heading is 5 degrees off north, make your adjustment
    azimuthCorrection(0); 
}
void init_zenith()
{
    //while not perpendicular to ground (5 degrees) , make your adjusment
    zenithCorrection(0);
}
void orientationCorrection(surya_data_s *surya_data)
{
    
    azimuthCorrection(surya_data->azimuth);
    zenithCorrection(surya_data->zenith);
}
void azimuthCorrection(uint16_t requested)
{
    //gert compass reading, perform calc and handoff to perform
    compass_data_s compass_data;
    read_compass(&compass_data);

    //determine direction
    uint16_t distance = 0; 

    if( compass_data.heading < requested ){
        if( (  (uint16_t)( compass_data.heading + requested + 0.5 ) % 360 ) < 180 ){
            azimuth_step.setDirection(CCW);
            distance = 360 - abs(compass_data.heading - requested);
            //Serial.println("CCW");
        } else {
            azimuth_step.setDirection(CW);
            distance = abs(compass_data.heading - requested);
            //Serial.println("CW");
        }

    } else {

        if( ( (uint16_t)( compass_data.heading + requested + 0.5) % 360 ) < 180 ){
            azimuth_step.setDirection(CW);
            distance = 360 - abs(compass_data.heading - requested);
            //Serial.println("CW");
        } else { 
            azimuth_step.setDirection(CCW);
            distance = abs(compass_data.heading - requested);
            //Serial.println("CCW");
        }
    }
    
    Serial.println("");
    Serial.println("Heading: " + String(compass_data.heading));
    Serial.println("Requested: " + String(requested));
    //Serial.println("Distance: " + String(distance));

    //perform
    //if( distance > COMPASS_M_ERROR )
        //performAzimuthAdjustment( distance );

    //turnoff motors
    for(int i = 0; i < sizeof(azimuth_motor_p); i++)
        digitalWrite(azimuth_motor_p[i], LOW);
}
void zenithCorrection(uint16_t requested)
{

    //get accelerometer reading, perform calc and handoff to perform
    accel_data_s accel_data;    
    read_accel(&accel_data);
    //determine direction
    
    uint16_t distance = 0;

    if(accel_data.verticle < requested ){
        zenith_step.setDirection(CCW);
    } else {
        zenith_step.setDirection(CW);
    }

    distance = abs( accel_data.verticle - requested );
    
    Serial.println("");
    Serial.println("Verticle Raw: " + String(accel_data.verticle) );
    Serial.println("Requested: " + String(requested) );

    //perform
    //performZenithAdjustment( distance );

    //turn off motors
    for(int i = 0; i < sizeof(zenith_motor_p); i++)
        digitalWrite(zenith_motor_p[i], LOW);
}
void performAzimuthAdjustment(uint16_t adjustment)
{
    azimuth_step.rotateDegrees( adjustment ); 
    while( ! azimuth_step.isDone() ){
        azimuth_step.run();
    }

}
void performZenithAdjustment(uint16_t adjustment)
{
    /* Huge Problem, unline azimuth, we may not be able to get to our requested zenith
     * because th platfrom may/proobably wont be exactly level. how do we account?
     * 
     * We can try to loop every 1 degree move and compare the last, if we didnt move then we're stuck
     *
    */
    zenith_step.rotateDegrees( adjustment ); 
    while( ! zenith_step.isDone() ){
        zenith_step.run();
    }
}

