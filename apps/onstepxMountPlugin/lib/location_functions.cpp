// clang-format off
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#include "../Config.h"
#include "location_functions.h"
#include "string_functions.h"

/*
  Notes on improvements:
  - avoid comparing 'char' to NULL; test for '+'/'-' explicitly.
  - do not cast away const and mutate input strings; copy into a temporary buffer before modifications.
  - use snprintf to avoid buffer overruns.
  - use strtol with endptr and errno checks to validate numeric conversions.
  - validate length and digits for hours/minutes to avoid overflow and malformed inputs.
  - consistently use nullptr checks and defensive programming.
*/

void set_location(Location* loc, char sign, long hours, long minutes) {
    if (loc == nullptr) {
        return;
    }
    // Only accept '+' or '-' as valid sign; default to '+'
    if (sign != '+' && sign != '-') {
        loc->sign = '+';
    } else {
        loc->sign = sign;
    }
    loc->hours = hours;
    loc->minutes = minutes;
}

void location_toString(Location loc, char buffer[], int bufferSize) {
    if (buffer == nullptr || bufferSize <= 0) {
        return;
    }
    // Ensure we don't overflow the provided buffer
    // Format: "+HH*MM" (original code used '*' as separator in output)
    // Use snprintf which guarantees null-termination (when bufferSize > 0)
    snprintf(buffer, (size_t)bufferSize, "%c%ld*%02ld", loc.sign, loc.hours, loc.minutes);
}

bool parse_location(const char* locStr, Location* loc, int max) {
    if (loc == nullptr || locStr == nullptr) {
        return false;
    }

    size_t len = strlen(locStr);
    // reject obviously too short or ridiculously long strings
    if (len < 2 || len > 128) {
        return false;
    }

    // Copy the input to a mutable buffer so replace_char can be used safely
    char* tmp = (char*)malloc(len + 1);
    if (tmp == nullptr) {
        return false;
    }
    memcpy(tmp, locStr, len + 1);

    // Replace common separators with ':'
    replace_char(tmp, '.', ':');
    replace_char(tmp, '*', ':');

    // Trim leading/trailing whitespace
    // (simple trim: remove leading spaces)
    char* start = tmp;
    while (*start && isspace((unsigned char)*start)) start++;
    // remove trailing spaces
    char* endptr_trim = tmp + strlen(tmp) - 1;
    while (endptr_trim >= start && isspace((unsigned char)*endptr_trim)) {
        *endptr_trim = '\0';
        endptr_trim--;
    }

    // Determine sign
    bool hasSign = false;
    if (*start == '+' || *start == '-') {
        loc->sign = *start;
        hasSign = true;
        start++;
    } else {
        loc->sign = '+';
    }

    // Find colon separator
    char* colonPos = strchr(start, ':');
    if (colonPos == nullptr) {
        free(tmp);
        return false;
    }

    // Extract hours substring and validate
    size_t hoursLen = (size_t)(colonPos - start);
    if (hoursLen == 0 || hoursLen > 6) { // limit to reasonable length
        free(tmp);
        return false;
    }
    // Ensure all chars in hours are digits
    for (size_t i = 0; i < hoursLen; ++i) {
        if (!isdigit((unsigned char)start[i])) {
            free(tmp);
            return false;
        }
    }

    // Convert hours using strtol with endptr check
    errno = 0;
    char* endptr = nullptr;
    long hours = strtol(start, &endptr, 10);
    if (errno != 0 || endptr != colonPos) {
        free(tmp);
        return false;
    }

    // Parse minutes: accept 1 or 2 digits, if more digits present consider them up to 2 as minutes
    char* minutesStart = colonPos + 1;
    if (*minutesStart == '\0') {
        free(tmp);
        return false;
    }

    // Only accept up to 2 digits for minutes; if more, fail
    size_t minutesLen = strlen(minutesStart);
    if (minutesLen == 0 || minutesLen > 2) {
        // allow input like "38:4" -> 4 minutes (len==1) but reject >2
        if (minutesLen > 2) {
            free(tmp);
            return false;
        }
    }
    for (size_t i = 0; i < minutesLen; ++i) {
        if (!isdigit((unsigned char)minutesStart[i])) {
            free(tmp);
            return false;
        }
    }

    errno = 0;
    long minutes = strtol(minutesStart, &endptr, 10);
    if (errno != 0 || endptr == minutesStart) {
        free(tmp);
        return false;
    }

    // Validate ranges
    if (hours < 0 || hours > max || minutes < 0 || minutes >= 60) {
        free(tmp);
        return false;
    }

    // Populate loc
    loc->hours = hours;
    loc->minutes = minutes;

    free(tmp);
    return true;
}

bool parse_offset(const char* offsetStr, Offset* offset) {
    if (offset == nullptr || offsetStr == nullptr) {
        return false;
    }

    size_t len = strlen(offsetStr);
    if (len == 0 || len > 16) {
        return false;
    }

    // copy to mutable buffer
    char tmp[32];
    if (len >= sizeof(tmp)) {
        return false;
    }
    memcpy(tmp, offsetStr, len + 1);

    // trim leading spaces
    char* start = tmp;
    while (*start && isspace((unsigned char)*start)) start++;
    // trim trailing spaces
    char* t = start + strlen(start) - 1;
    while (t >= start && isspace((unsigned char)*t)) {
        *t = '\0';
        t--;
    }

    // determine sign
    bool hasSign = false;
    if (*start == '+' || *start == '-') {
        offset->sign = *start;
        hasSign = true;
        start++;
    } else {
        offset->sign = '+';
    }

    // remaining should be hours number (one or two digits typically)
    if (*start == '\0') {
        return false;
    }
    // validate digits only (allow optional ":00" suffix or just digits)
    char* colon = strchr(start, ':');
    size_t hoursLen = 0;
    if (colon) {
        hoursLen = (size_t)(colon - start);
        // don't allow extra data after : other than optional "00" or digits
        char* after = colon + 1;
        if (*after != '\0') {
            // accept ":00" or ":0" or empty, but if there are other non-digits, reject
            for (char* p = after; *p; ++p) {
                if (!isdigit((unsigned char)*p)) return false;
            }
        }
    } else {
        hoursLen = strlen(start);
    }

    if (hoursLen == 0 || hoursLen > 3) {
        return false;
    }
    for (size_t i = 0; i < hoursLen; ++i) {
        if (!isdigit((unsigned char)start[i])) {
            return false;
        }
    }

    // parse hours
    errno = 0;
    char* endptr = nullptr;
    long hours = strtol(start, &endptr, 10);
    if (errno != 0) {
        return false;
    }

    // simple reasonable validation: offsets are commonly between 0 and 14 (timezones),
    // but we won't hardcode that here — caller can validate if needed.
    if (hours < 0 || hours > 1000) { // arbitrary large upper bound
        return false;
    }

    offset->hours = hours;
    return true;
}

void offset_toString(Offset offset, char buffer[], int bufferSize) {
    if (buffer == nullptr || bufferSize <= 0) {
        return;
    }
    // Format as "+HH:00" (keeps original behavior)
    snprintf(buffer, (size_t)bufferSize, "%c%02ld:00", offset.sign, offset.hours);
}
