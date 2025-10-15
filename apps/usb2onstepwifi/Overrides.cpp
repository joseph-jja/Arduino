#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Config.h"
#include "Overrides.h"
#include "string_functions.h"

#define DEFAULT_BUFFER_SIZE 128

/* TODO fix these errors
2025-10-01T20:55:24: [ERROR] Error setting site longitude coordinates 
2025-10-01T20:55:19: [ERROR] Error setting UTC Offset. 
2025-10-01T20:55:14: [ERROR] Error setting site longitude coordinates 
2025-10-01T20:55:09: [WARNING] Error on response to rotator check (:GX98#) CHECK CONNECTION 
2025-10-01T20:55:09: [WARNING] Error: -4 
2025-10-01T20:55:09: [INFO] OnStep/OnStepX version could not be detected 
2025-10-01T20:54:49: [WARNING] Could not process mount date and time: T00:00:00 
2025-10-01T20:54:34: [WARNING] Failed to get site latitude from device. 
Failed to get tracking frequency from device. 
2025-10-01T20:54:24: [ERROR] Failed to get site name from device 
2025-10-01T20:54:19: [ERROR] Failed to retrieve time format from device. 
2025-10-01T20:54:03: [INFO] OnStepX Focuser is online. 
*/
bool Overrides::check_override(char *bufferIn, char buffer[], int buffer_size, unsigned long ms) {

  bool override = false;

  if (isNull(bufferIn)) {
    return override;
  }

  memset(buffer, '\0', buffer_size);
  int bufferInLen = strlen(bufferIn);

  if (bufferInLen == 0 || bufferInLen > buffer_size - 1) {
    return override;
  }

  char date_time_buffer[DEFAULT_BUFFER_SIZE];
  memset(date_time_buffer, '\0', DEFAULT_BUFFER_SIZE);

  // commands to skip
  if (compare(bufferIn, USB_RESET_COMMAND)) {
    override = true;
    sprintf(buffer, "%s#", USB_RESET_REPLY);
  } else if (compare(bufferIn, ACK_COMMAND_IN)) {
    // get mount type
    override = true;
    sprintf(buffer, "%s", MOUNT_MODE);
  }

  return override;
}

Overrides overrides;
