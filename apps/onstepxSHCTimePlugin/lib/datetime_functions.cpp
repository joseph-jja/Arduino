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

    if (dateStr == nullptr ){
        return -1;
    }
    
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

    long month = search_string(dateStr, 0, 2);
    long day = search_string(dateStr, 3, 2);
    long year = search_string(dateStr, 6, 2);

    if (month == -1 || day == -1 || year == -1) {
        return false;
    }
    
    dateInfo->month = month;
    dateInfo->day = day;
    dateInfo->year = year;
    
    return true;
};  

bool parse_time(char* timeStr, TimeInfo* timeInfo) {

    // Basic validation
    if (timeStr == nullptr || strlen(timeStr) < 8) {
        return false;
    }

    long hour = search_string(timeStr, 0, 2);
    long minutes = search_string(timeStr, 3, 2);
    long seconds = search_string(timeStr, 6, 2);

    if (hour == -1 || minutes == -1 || seconds == -1) {
        return false;
    }

    timeInfo->hour = hour;
    timeInfo->minutes = minutes;
    timeInfo->seconds = seconds;

    if (timeInfo->hour >= 12) {
        snprintf(timeInfo->meridian, sizeof(timeInfo->meridian), "PM");
    } else {
        snprintf(timeInfo->meridian, sizeof(timeInfo->meridian), "AM");
    }
    
    return true;
};  

void date_toString(DateInfo dateInfo, char buffer[], int bufferSize) {
    memset(buffer, '\0', bufferSize);
    snprintf(buffer, sizoef(buffer), "%02ld/%02ld/%02ld", dateInfo.month, dateInfo.day, dateInfo.year);
};

void time_12h_toString(TimeInfo timeInfo, char buffer[], int bufferSize) {
    memset(buffer, '\0', bufferSize);
    snprintf(buffer, sizoef(buffer), "%02ld:%02ld:%02ld", timeInfo.hour, timeInfo.minutes, timeInfo.seconds);
};

void time_24h_toString(TimeInfo timeInfo, char buffer[], int bufferSize) {
    memset(buffer, '\0', bufferSize);
    long hour = timeInfo.hour;
    if (strcmp(timeInfo.meridian, "PM") == 0 && timeInfo.hour < 12) {
        hour += 12;
    } else if (strcmp(timeInfo.meridian, "AM") == 0 && timeInfo.hour == 12) {
        hour = 0;
    }
    snprintf(buffer, sizoef(buffer), "%02ld:%02ld:%02ld", hour, timeInfo.minutes, timeInfo.seconds);
};

