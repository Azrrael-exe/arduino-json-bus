#ifndef vco_controller_h
#define vco_controller_h

#include <Arduino.h>

struct VCO_CONTROLLER {
  char device;
  uint8_t amplitude;
  uint8_t noise;
  uint8_t offset;
};

#endif
