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

void replace_char(char *source, char search_str, char replace_str) {

    char *n = strstr(source, &search_str);
    if (n != NULL) {
        
    }
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

bool boolean_reply(char *commandIn) {
  bool has_0_or_1_reply = compare(commandIn, ":S");

  if (!has_0_or_1_reply) {
    int i = 0;
    while (i < NUMBER_OF_0_or_1_STRINGS && !has_0_or_1_reply) {
      if (compare(commandIn, COMMANDS_WITH_0_or_1_REPLY[i])) {
        has_0_or_1_reply = true;
      }
      i++;
    }
  }
  return has_0_or_1_reply;
}

void ends_with(char *messageIn, char return_buffer[]) {

  memset(return_buffer, '\0', 2);
  if (compare(messageIn, ":Gd#") || compare(messageIn, ":GR#")
      || compare(messageIn, ":GD#") || compare(messageIn, ":GZ#")
      || compare(messageIn, ":GA#")) {
    return_buffer[0] = '*';
  } else if (compare(messageIn, ":G") || compare(messageIn, ":F")) {
    return_buffer[0] = '#';
  } else if (!compare(messageIn, ":MS#")
             && !compare(messageIn, ":MA#")
             && !compare(messageIn, ":A2#")
             && !compare(messageIn, ":ERESET#")
             && !compare(messageIn, ":ENVRESET#")) {
    return_buffer[0] = '#';
  }
}
