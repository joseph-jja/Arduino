#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "datetime_functions.h"
#include "string_functions.h"

#define ONE_DAY 84000
#define DEFAULT_DATE "25/09/18"
#define DEFAULT_TIME "12:00:00"

static char compile_time[] = __TIME__;

static int DAYS_IN_MONTH[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

long search_string(char* dateStr, int start_index, int length) {

    char buffer[3];
    memset(buffer, '\0', 3);

    substring(dateStr, start_index, length, buffer);
        
    return strtol(buffer, nullptr, 10);
};

bool parse_date(char* dateStr, DateInfo* dateInfo) {

    // Basic validation
    if (dateStr == nullptr || strlen(dateStr) < 8) {
        return false;
    }
    
    dateInfo->month = search_string(dateStr, 0, 2);
    dateInfo->day = search_string(dateStr, 3, 2);
    dateInfo->year = search_string(dateStr, 6, 2);
    
    return true;
};  

bool parse_time(char* timeStr, TimeInfo* timeInfo) {

    // Basic validation
    if (timeStr == nullptr || strlen(timeStr) < 8) {
        return false;
    }

    timeInfo->hour = search_string(timeStr, 0, 2);
    timeInfo->minutes = search_string(timeStr, 3, 2);
    timeInfo->seconds = search_string(timeStr, 6, 2);
    
    if (timeInfo->hour >= 12) {
        sprintf(timeInfo->meridian, "PM");
    } else {
        sprintf(timeInfo->meridian, "AM");
    }
    
    return true;
};  

void date_toString(DateInfo dateInfo, char buffer[], int bufferSize) {
    memset(buffer, '\0', bufferSize);
    sprintf(buffer, "%02ld/%02ld/%02ld", dateInfo.month, dateInfo.day, dateInfo.year);
};

void time_12h_toString(TimeInfo timeInfo, char buffer[], int bufferSize) {
    memset(buffer, '\0', bufferSize);
    sprintf(buffer, "%02ld:%02ld:%02ld", timeInfo.hour, timeInfo.minutes, timeInfo.seconds);
};

void time_24h_toString(TimeInfo timeInfo, char buffer[], int bufferSize) {
    memset(buffer, '\0', bufferSize);
    long hour = timeInfo.hour;
    if (strcmp(timeInfo.meridian, "PM") == 0 && timeInfo.hour < 12) {
        hour += 12;
    } else if (strcmp(timeInfo.meridian, "AM") == 0 && timeInfo.hour == 12) {
        hour = 0;
    }
    sprintf(buffer, "%02ld:%02ld:%02ld", hour, timeInfo.minutes, timeInfo.seconds);
};

