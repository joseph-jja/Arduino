// clang-format off
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../Config.h"
#include "Location.h"
#include "string_functions.h"

void SiteLocation::init() {

    parse_location(DEFAULT_LATITUDE, &latitude);
    parse_location(DEFAULT_LONGITUDE, &longitude);
    utc_offset.sign = DEFAULT_UTC_OFFSET[0];
    utc_offset.hours = strtol(&DEFAULT_UTC_OFFSET[1], nullptr, 10);
}

void SiteLocation::set_location(Location* loc, char sign, long hours, long minutes) {
    loc->sign = sign;
    loc->hours = hours;
    loc->minutes = minutes;
}

void SiteLocation::location_toString(Location loc, char buffer[], int bufferSize) {
    memset(buffer, '\0', bufferSize);
    sprintf(buffer, "%c%ld:%02ld", loc.sign, loc.hours, loc.minutes);
}

bool SiteLocation::parse_location(const char* locStr, Location* loc) {
    // Basic validation
    if (locStr == nullptr || strlen(locStr) < 4) {
        return false;
    }

    // Check sign
    if (locStr[0] != '+' && locStr[0] != '-') {
        return false;
    }
    loc->sign = locStr[0];

    // in the event we get a 38.44 or 38*44 format, convert to 38:44
    replace_char((char*)locStr, '.', ':');
    replace_char((char*)locStr, '*', ':');

    // Find the colon separator
    const char* colonPos = strchr(locStr, ':');
    if (colonPos == nullptr) {
        return false;
    }

    // Extract hours
    char hoursStr[4] = {0}; // Max 3 digits for hours + null terminator
    size_t hoursLen = colonPos - (locStr + 1);
    if (hoursLen == 0 || hoursLen >= sizeof(hoursStr)) {
        return false;
    }
    strncpy(hoursStr, locStr + 1, hoursLen);
    loc->hours = strtol(hoursStr, nullptr, 10);

    // Extract minutes
    char minutesStr[3] = {0}; // Max 2 digits for minutes + null terminator
    strncpy(minutesStr, colonPos + 1, 2);
    loc->minutes = strtol(minutesStr, nullptr, 10);

    // Validate ranges
    if (loc->hours < 0 || loc->hours > 90 || loc->minutes < 0 || loc->minutes >= 60) {
        return false;
    }

    return true;
}

bool SiteLocation::parse_offset(const char* offsetStr, Offset* offset) {

    // Basic validation
    if (offsetStr == nullptr || strlen(offsetStr) < 2) {
        return false;
    }

    // Check sign
    if (offsetStr[0] != '+' && offsetStr[0] != '-') {
        return false;
    }
    offset->sign = offsetStr[0];
    
    char hoursStr[3] = {0};
    for ( int i  = 1; i < strlen(offsetStr); i++ ) {
        hoursStr[i - 1] = offsetStr[i];
    }
    offset->hours = strtol(hoursStr, nullptr, 10);
    
    return true;
}

void SiteLocation::offset_toString(Offset offset, char buffer[], int bufferSize) {

    memset(buffer, '\0', bufferSize);
    sprintf(buffer, "%c%ld", offset.sign, offset.hours);
}

SiteLocation siteLocation;

