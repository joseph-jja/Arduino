#pragma once

#define DATE_TIME_SIZE 10

// store time in 12 hour format with AM/PM
typedef struct {
    int hour;
    int minute;
    int second;
    char meridian[3]; // Stores "AM" or "PM"
} TimeInfo;​

// store date split out
typedef struct {
    int year;
    int month;
    int day;
} DateInfo;​

class Datetime {
public:
  // Public members (accessible from outside the class)
  void init(unsigned long ms);  // Constructor
  bool set_date(char date[]);
  bool set_local_time(char time[]);     // always uses 24 hour format
  bool set_sidereal_time(char time[]);  // always uses 24 hour format

  void get_date(char date[], unsigned long ms);
  void get_local_time(char time[], unsigned long ms, bool is_twenty_four_hour);
  void get_sidereal_time(char time[], unsigned long ms);

private:

  long update_time(unsigned long ms);
  long update_time_value(char time[], unsigned long ms, long time_in, bool is_twenty_four_hour);

  TimeInfo convert_24h_to_12hr(int hour_24, int minute, int second);
  TimeInfo convert_12h_to_24hr(int hour_12, int minute, int second, const char* meridian);

  long start_time;
  long delta_time;
  long days_changed;

  // date and time stuff
  DateInfo date_part;
  TimeInfo local_time_part;
  TimeInfo sidereal_time_part;
};

extern Datetime datetime;
