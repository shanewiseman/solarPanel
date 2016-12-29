#include "main.h"
gps_data_s gps_data;
surya_data_s surya_data;
rtc_data_s   rtc_data;

void setup(void)
{
    Serial.begin(9600);

    initialize();

    Serial.println("Starting GPS");
    init_gps();
    read_gps(&gps_data, &rtc_data);
    gps_sleep();

    Serial.println("Starting COMPASS");
    init_compass();

    Serial.println("Starting Accelerameter");
    init_accel();

    Serial.println("Starting RTC");
    init_rtc();
    set_ctime(&rtc_data);

    Serial.println("INIT Orientation");
    init_azimuth();
    init_zenith();
    
    
    Serial.println("SET UP");
    Serial.println("");
  
}

void loop(void)
{
    get_ctime(&rtc_data);

    Serial.print(rtc_data.hour);
    Serial.print(":");
    Serial.print(rtc_data.minutes);
    Serial.print(":");
    Serial.println(rtc_data.seconds);

    getSuryaData(&gps_data, &rtc_data, &surya_data);
  
    if( surya_data.zenith > SLEEP_ZENITH && 0){

        Serial.println("SLEEP CODE");

        performSleep(&rtc_data, &surya_data, &gps_data);

    } else {

        orientationCorrection(&surya_data);
        delay(1000);
    
    }
}


    // what i imagine is, if we were to lose equal or more by not tracking then sleep then sleep, we can do this foreach iteration.
    // ie. a cloud is overhead, we're not going to gain by expending energy tracking, take the slight hit but save by staying still, sleep for a minute



/*
    TODO

    1: we should read gps then transfer time data to rtc, we already have our coordinates, no sense in keeping
        radio on when we dont need too,
        use another pin and transitor to turn off gps but not sensors,
        when we sleep we'll turn them all off.

    1: should we put ourselves to sleep during the day?
        2.1 at some scenario tracking does not return the energy we're investing
        2.1.a the lower the intensity the more we should allow the panel to be scanned rather than tracked
        2.1.a.1 say at 80% we all allow 20 degree scan, 60% 40 degrees 
        2.1.a.2 how do we sample the panel?
        2.1.a.3 we would still need to change zenith, just scan azimuth

   2: sample the solarpanels 18.8V. can divide the voltage twice to give a max of 4.7V
        a. can give an idea of how efficient the panels are collecting light
        b. give logic conditions on how and what we perform, adjusting our usage with what we're producing.
        c 3 -> 1 resistor ratio


*/


