#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/power.h>

#define SHUTDOWN_SPEED 60000

void initialize(void);
void aux_check(void);
void postError(uint8_t error_code);
bool checkPower(uint8_t pwrpin);

void performSleep(rtc_data_s *rtc_data, surya_data_s *surya_data, gps_data_s *gps_data);
void awakenSleep(void);
