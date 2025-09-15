#include "string_functions.h"

#include <string.h>

#include "Constants.h"

// compares 2 string
bool compare(char *commandIn, char *commandStr) {
  return (memcmp(commandIn, commandStr, strlen(commandStr)) == 0);
}

// copies part of a string from source to destination
void substring(char *source, int start_index, int length, char *destination) {
  strncpy(destination, source + start_index, length);
}

bool has_reply(char *messageIn) {

  bool hasResponse = true;
  // we know that :S is setting something and reply is 0 or 1
  // we know that :G is getting something so these have a reply message
  if (compare(messageIn, ":S") || compare(messageIn, ":G")) {
    hasResponse = true;
  } else {
    int i = 0;
    while (i < NUMBER_OF_STRINGS && hasResponse) {
      if (compare(messageIn, COMMANDS_WITH_NO_REPLY[i])) {
        hasResponse = false;
      }
      i++;
    }
  }

  return hasResponse;
}
