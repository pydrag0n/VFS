#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/vfs.h"


char *_get_datetime()
{
    int date_size = 256;
    char *datetime = malloc(date_size);
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    sprintf(datetime, "%02d-%02d-%04d %02d:%02d:%02d",
                    timeinfo->tm_mday,
                    timeinfo->tm_mon+1,
                    timeinfo->tm_year+1900,
                    timeinfo->tm_hour,
                    timeinfo->tm_min,
                    timeinfo->tm_sec
                    );
    return datetime;
}