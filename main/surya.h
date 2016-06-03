#ifndef SURYA_H
#define SURYA_H

#include "gps.h"
#include "compass.h"
#include "accelerometer.h"
#include "./Helios/Helios.h"

typedef struct surya_data_s
{

    uint16_t zenith;
    uint16_t azimuth;

}  surya_data_s;


void getSuryaData(gps_data_s *gps_data, surya_data_s *surya_data);
void searchSuryaPosition(gps_data_s *gps_data, uint8_t angle );

#endif
