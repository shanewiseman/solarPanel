#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MMA8451.h>

typedef struct accel_data_s
{
    //we should make note these could be negative, but... given the use case it will only be positive here,
    // if we need to handle negative numbers start changes here
    uint8_t verticle;
    uint8_t horizontal;

} accel_data_s;

void init_accel(void);
void read_accel(accel_data_s *accel_data); 
