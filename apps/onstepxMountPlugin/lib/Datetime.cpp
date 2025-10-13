#include <string.h>
#include <stdio.h>

#include "Datetime.h"

#define ONE_DAY 84000
#define DEFAULT_DATE "25/09/18"
#define DEFAULT_TIME "12:00:00"

static char compile_time[] = __TIME__;

static int DAYS_IN_MONTH[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

void Datetime::init() { // Constructor

    parse_date(DEFAULT_DATE, &date_part);
    parse_time(DEFAULT_TIME, &local_time_part);
    parse_time(DEFAULT_TIME, &sidereal_time_part);
};

long search_string(char* dateStr, char search_for, int start_index) {

    char buffer[3];
    memset(buffer, '\0', 3);
    int j = 0;
    int i = start_index;
    bool found = false;

    // year
    while (!found && i < strlen(dateStr)) {
       if (dateStr[i] == '/') {
           found = true;
       } 
       if (!found) {
           buffer[j] = dateStr[i]; 
           i++;
           j++;
       }
    }
    
    if (found) {
        return strtol(buffer, nullptr, 10);
    }
    return -1;
}

bool Datetime::parse_date(char* dateStr, DateInfo* dateInfo) {

    // Basic validation
    if (dateStr == nullptr || strlen(dateStr) < 8) {
        return false;
    }
    
    dateInfo->year = search_string(dateStr, '/', 0);
    dateInfo->month = search_string(dateStr, '/', 2);
    dateInfo->day = search_string(dateStr, '/', 5);
    
    return true;
};  

bool Datetime::parse_time(char* timeStr, TimeInfo* timeInfo) {

    // Basic validation
    if (timeStr == nullptr || strlen(timeStr) < 8) {
        return false;
    }

    timeInfo->hour = search_string(timeStr, ':', 0);
    timeInfo->minutes = search_string(timeStr, ':', 2);
    timeInfo->seconds = search_string(timeStr, ':', 5);
    
    if (timeInfo->hour >= 12) {
        sprintf(timeInfo->meridian, "PM");
    } else {
        sprintf(timeInfo->meridian, "AM");
    }
    
    return true;
};  

Datetime datetime;
