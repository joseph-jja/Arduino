// USB2I2C plugin
#pragma once

#define DEFAULT_LOCATION_SIZE 20

class OnStepXMountPlugin {

public:
  // the initialization method must be present and named: void init();
  void init();

  void loop();

  bool command(char reply[], char command[], char parameter[], bool *supressFrame, bool *numericReply, CommandError *commandError);

private:

};

extern OnStepXMountPlugin onstepxMountPlugin;
