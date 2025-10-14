#pragma once

//#define USB_DEBUG_ENABLED 1
//#define MOCK_CLIENT_ENABLED 1
#define ESP32_I2C_ADDRESS 24
#define WIFI_CLIENT_READ_TIMOUT 5000
#define USB_READ_TIMOUT 2500

const int NUMBER_OF_STRINGS = 56;
const int MAX_STRING_LENGTH = 10;

static char COMMANDS_WITH_NO_REPLY[NUMBER_OF_STRINGS][MAX_STRING_LENGTH] = {
  ":W0#",
  ":W1#",
  ":W2#",
  ":W3#",
  ":Q#",
  ":Me#",
  ":Mw#",
  ":Mn#",
  ":Ms#",
  ":Qe#",
  ":Qw#",
  ":Qn#",
  ":Qs#",
  ":Mg",
  ":RG#",
  ":RC#",
  ":RM#",
  ":RS#",
  ":Rn#",
  ":TQ#",
  ":TR#",
  ":T+#",
  ":T-#",
  ":TS#",
  ":TL#",
  ":TK#",
  ":CS#",
  ":LB#",
  ":LN#",
  ":LC",
  ":LD#",
  ":LL#",
  ":L!#",
  ":$QZ+#",
  ":$QZ-#",
  ":$QZZ#",
  ":$QZ/#",
  ":$QZ!#",
  ":hF#",
  ":hC#",
  ":FQ#",
  ":FF#",
  ":FS#",
  ":F1#",
  ":F2#",
  ":F3#",
  ":F4#",
  ":F+#",
  ":F-#",
  ":FR",
  ":FZ#",
  ":FH#",
  ":Fh#",
  ":B+#",
  ":B-#",
  ":U#"
};

const int NUMBER_OF_0_or_1_STRINGS = 27;

static char COMMANDS_WITH_0_or_1_REPLY[NUMBER_OF_0_or_1_STRINGS][MAX_STRING_LENGTH] = {
  ":S",
  ":Te#",
  ":Td#",
  ":Tr#",
  ":Tn#",
  ":Lo",
  ":LW",
  ":$BR",
  ":$BD",
  ":WR",
  ":AW#",
  ":A1#",
  ":A+#",
  ":hQ#",
  ":hP#",
  ":hR#",
  ":FA#",
  ":fA#",
  ":FA",
  ":Fa#",
  ":FB",
  ":FC",
  ":Fc#",
  ":Fc",
  ":FD",
  ":FP",
  ":FS"
};
