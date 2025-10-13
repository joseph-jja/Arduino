// USB2I2C plugin
#pragma once



class OnStepXMountPlugin {
public:
  // the initialization method must be present and named: void init();
  void init();

  void loop();

  void command(char reply[], char command[], char parameter[], bool *supressFrame, bool *numericReply, CommandError *commandError);

private:

};

extern OnStepXMountPlugin onstepxMountPlugin;
