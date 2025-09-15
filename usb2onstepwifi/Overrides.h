#pragma once

#define DEFAULT_LOCATION_SIZE 20
#define DEFAULT_DATE_TIME_SIZE 10

class Overrides {
public:
    // Public members (accessible from outside the class)
    void init(); // Constructor
    bool check_override(char *bufferIn);

private:
    // latitude and longitude defaults holder
    // +xx:yyy and +zzz:abc
    char latitude[DEFAULT_LOCATION_SIZE];
    char longitude[DEFAULT_LOCATION_SIZE];
    // offset 
    int utcoffset;
    // date and time stuff
    char date_str[DEFAULT_DATE_TIME_SIZE];
    char local_time_str[DEFAULT_DATE_TIME_SIZE];
    char time_str[DEFAULT_DATE_TIME_SIZE];
};

extern Overrides overrides;