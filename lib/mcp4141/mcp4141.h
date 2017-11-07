#ifndef mcp4141_h
#define mcp4141_h

#include <Arduino.h>
#include <SPI.h>

class MCP4141
{
public:
  MCP4141(uint8_t);
  MCP4141();
  uint8_t set(uint8_t);
  uint8_t setPercent(uint8_t);
  uint8_t increment();
  uint8_t decrement();
private:
  uint8_t chip_select;
  uint8_t position;
};

#endif
