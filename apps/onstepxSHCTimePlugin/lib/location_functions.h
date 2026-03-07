#pragma once

#define DEFAULT_LOCATION_SIZE 10

#define MAX_LATITUDE 90
#define MAX_LONGITUDE 180

typedef struct {
    char sign; // '+' or '-'
    long hours;
    long minutes;
} Location;

typedef struct {
    char sign; // '+' or '-'
    long hours;
} Offset;

// fills in a location structure
void set_location(Location* loc, char sign, long hours, long minutes);

// converts location structure to string
void location_toString(Location loc, char buffer[], int bufferSize);

// parses location string into structure
bool parse_location(const char* locStr, Location* loc, int max);

bool parse_offset(const char* offsetStr, Offset* offset);

// converts location structure to string
void offset_toString(Offset loc, char buffer[], int bufferSize);
