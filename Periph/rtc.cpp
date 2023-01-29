#include "rtc.h"

 time_t seconds;

int8_t RTC_init()
{
     seconds = time(NULL);
    return 0;
};

void RTC_setTime(uint32_t timestamp)
{
    set_time(timestamp);

};
uint32_t RTC_getTime()
{
   seconds = time(NULL);
   return seconds;
};