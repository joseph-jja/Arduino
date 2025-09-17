#pragma once

#define DEFAULT_LOCATION_SIZE 20

class Overrides {
public:
  // Public members (accessible from outside the class)
  void init(unsigned long ms);  // Constructor
  bool check_override(char *bufferIn, char returnOut[], int buffer_size, unsigned long ms);

private:

  // latitude and longitude defaults holder
  // +xx:yyy and +zzz:abc
  char latitude[DEFAULT_LOCATION_SIZE];
  char longitude[DEFAULT_LOCATION_SIZE];
  // offset
  int utcoffset;
};

extern Overrides overrides;