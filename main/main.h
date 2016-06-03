#include "gps.h"
#include "surya.h"
#include "rtc.h"
#include "orientation.h"
#include "utils.h"

#define SLEEP_ZENITH (uint8_t) 80 

bool checkPower(uint8_t);
void postError(uint8_t);
void aux_check(void);
