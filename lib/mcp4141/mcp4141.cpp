#include "mcp4141.h"

MCP4141::MCP4141(){
  ;;
}

MCP4141::MCP4141(uint8_t pin){
  SPI.begin();
  chip_select = pin;
  position = 0;
  pinMode(chip_select, OUTPUT);
  set(127);
}

uint8_t MCP4141::set(uint8_t val){
  digitalWrite(chip_select, LOW);
  SPI.transfer(0);
  SPI.transfer(val);
  digitalWrite(chip_select, HIGH);
  digitalWrite(chip_select, LOW);
  SPI.transfer(0);
  SPI.transfer(val);
  digitalWrite(chip_select, HIGH);
  position = val;
  return position;
}

uint8_t MCP4141::setPercent(uint8_t val){
    uint8_t cal_val = uint8_t(128*((100-val)/100.0));
    set(cal_val);
    return position;
}

uint8_t MCP4141::increment(){
  if(position < 127){
    digitalWrite(chip_select, LOW);
    SPI.transfer(4);
    digitalWrite(chip_select, HIGH);
    position+=1;
  }
  return position;
}

uint8_t MCP4141::decrement(){
  if(position > 0){
    digitalWrite(chip_select, LOW);
    SPI.transfer(8);
    digitalWrite(chip_select, HIGH);
    position-=1;
  }
  return position;
}
