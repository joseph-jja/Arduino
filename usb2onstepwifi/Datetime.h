#pragma once

#define DATE_TIME_SIZE 10

class Datetime {
public:
  // Public members (accessible from outside the class)
  void init(unsigned long ms);  // Constructor
  bool set_date(char date[]);
  bool set_time(char time[]);
  void get_date(char date[]);
  void get_time(char time[]);

private:

  long start_time;
  long delta_time;

  // date and time stuff
  char date_str[DATE_TIME_SIZE];
  char time_str[DATE_TIME_SIZE];
};

extern Datetime datetime;