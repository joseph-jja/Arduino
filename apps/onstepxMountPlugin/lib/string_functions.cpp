#include <string.h>

#include "string_functions.h"

// compares 2 string
bool compare(char *commandIn, char *commandStr) {
  if (commandIn == NULL || commandStr == NULL) {
    return false;
  }
  if (strlen(commandIn) > strlen(commandStr)) {
    return false;
  }
  return (memcmp(commandIn, commandStr, strlen(commandStr)) == 0);
}

// copies part of a string from source to destination
void substring(char *source, int start_index, int length, char *destination) {
  if (source == NULL || destination == NULL) {
    return;
  }
  strncpy(destination, source + start_index, length);
}

void replace_char(char *source, char search_chr, char replace_chr) {
    if (source == NULL) {
        return;
    }
  
    int i = 0;
    while (source[i] != '\0') {
        if (source[i] == search_chr) {
            source[i] = replace_chr;
        }
        i++;
    }
}

bool isNull(char *buffer) {
  return ((buffer == NULL || strlen(buffer) <= 0) ? true : false); 
}

