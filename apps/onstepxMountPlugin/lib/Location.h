#pragma once

#define DEFAULT_LOCATION_SIZE 10

typedef struct {
    char sign; // '+' or '-'
    long hours;
    long minutes;
} Location;

typedef struct {
    char sign; // '+' or '-'
    long hours;
} Offset;

class SiteLocation {
public:

    void init();

    // fills in a location structure
    void set_location(Location* loc, char sign, long hours, long minutes);

    // converts location structure to string
    void location_toString(Location loc, char buffer[], int bufferSize);

    // parses location string into structure
    bool parse_location(const char* locStr, Location* loc);

    bool parse_offset(const char* offsetStr, Offset* offset);

    // converts location structure to string
    void offset_toString(Offset loc, char buffer[], int bufferSize);
    
    Location latitude;
    Location longitude;
    Offset utc_offset;

    char location_buffer[DEFAULT_LOCATION_SIZE];
};

extern SiteLocation siteLocation;
