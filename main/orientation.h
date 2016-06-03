
#include "surya.h"
#include "./CustomStepper/CustomStepper.h"

#define R_STEPS_MOTOR 64
#define R_STEPS_OUTER 64 * 64
#define STEPS_P_DEGREE 11

void init_azimuth(void);
void init_zenith(void);

void orientationCorrection(surya_data_s *surya_data);

void azimuthCorrection(uint16_t requested);
void zenithCorrection(uint16_t requested); 

void performAzimuthAdjustment(uint16_t adjustment);
void performZenithAdjustment(uint16_t adjustment);


