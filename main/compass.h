#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

#define COMPASS_M_ERROR 5

typedef struct compass_data_s
{
    int16_t heading;

} compass_data_s;

void init_compass(void);
void read_compass(compass_data_s *data);
