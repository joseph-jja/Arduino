#pragma once

#define DATE_TIME_SIZE 10

// store time in 12 hour format with AM/PM
typedef struct {
    long hour;
    long minutes;
    long seconds;
    char meridian[3]; // Stores "AM" or "PM"
} TimeInfo; 

// store date split out
typedef struct {
    long year;
    long month;
    long day;
} DateInfo;

class Datetime {
public:
  // Public members (accessible from outside the class)
  void init();  // Constructor
  
  bool parse_date(char* dateStr, DateInfo* dateInfo);
  bool parse_time(char* timeStr, TimeInfo* timeInfo);
  
  void date_toString(DateInfo dateInfo, char buffer[], int bufferSize);
  void time_12h_toString(TimeInfo timeInfo, char buffer[], int bufferSize);
  void time_24h_toString(TimeInfo timeInfo, char buffer[], int bufferSize);

  // date and time objects
  DateInfo date_part;
  TimeInfo local_time_part;
  TimeInfo sidereal_time_part;

//private:

};

extern Datetime datetime;
