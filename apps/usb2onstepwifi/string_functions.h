#pragma once

bool compare(char *commandIn, char *commandStr);

void substring(char *source, int start_index, int length, char *destination);

void replace_char(char *source, char *search_chr, char replace_chr);

bool has_reply(char *messageIn);

bool boolean_reply(char *commandIn);

void ends_with(char *messageIn, char return_buffer[2]);

bool isNull(char *buffer);
