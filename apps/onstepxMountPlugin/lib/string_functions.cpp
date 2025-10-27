#include <string.h>

#include "string_functions.h"

// compares 2 string
bool compare(char *commandIn, char *commandStr) {
  if (commandIn == nullptr || commandStr == nullptr) {
    return false;
  }
  if (strlen(commandIn) > strlen(commandStr)) {
    return false;
  }
  return (memcmp(commandIn, commandStr, strlen(commandStr)) == 0);
}

// copies part of a string from source to destination
void substring(char *source, int start_index, int length, char *destination) {
  if (source == nullptr || destination == nullptr) {
    return;
  }
  strncpy(destination, source + start_index, length);
}

void replace_char(char *source, char search_chr, char replace_chr) {
    if (source == nullptr) {
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
  return ((buffer == nullptr || strlen(buffer) <= 0) ? true : false); 
}

