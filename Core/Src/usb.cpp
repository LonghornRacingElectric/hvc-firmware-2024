#include "usb.h"
#include <cstring>

static uint8_t buffer[1024];

void println(std::string& line) {
  if(line.size() > 1000) {
    return;
  }
  strcpy(reinterpret_cast<char *>(buffer), line.c_str());
  size_t s = line.size();
  buffer[s++] = '\n';
  buffer[s] = '\0';
  HAL_UART_Transmit(&huart4, buffer, s, 100);
}