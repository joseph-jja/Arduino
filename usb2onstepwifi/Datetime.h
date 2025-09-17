#pragma once

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

  long start_time;
  long delta_time;
  long days_changed = 0;

  // date and time stuff
  long date_part = 0;
  long local_time_part = 0;
  long sidereal_time_part = 0;
};

extern Datetime datetime;
