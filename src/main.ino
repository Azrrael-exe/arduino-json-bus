#include <Arduino.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#include <EEPROMVar.h>
#include <EEPROMex.h>

#include <mcp4141.h>
#include <SparkFun_MiniGen.h>

#include "vco_controller.h"
#include "definitions.h"

SoftwareSerial outSerial(2, 3);
EEPROMClassEx nonV = EEPROMClassEx();
VCO_CONTROLLER controller;

MCP4141 amplitude_pot = MCP4141(cs_a);
MCP4141 noise_pot = MCP4141(cs_n);
MCP4141 offset_pot = MCP4141(cs_o);

MiniGen low_freq_gen = MiniGen(cs_gl);
MiniGen high_freq_gen = MiniGen(cs_gh);

void setup() {
  Serial.begin(9600);
  outSerial.begin(9600);

  // ::: Restore VCO Config :::
  readController(nonV, controller);

  // ::: Signal Generators Config :::
  low_freq_gen.reset();
  high_freq_gen.reset();
  low_freq_gen.setMode(MiniGen::TRIANGLE);
  high_freq_gen.setMode(MiniGen::TRIANGLE);
  low_freq_gen.setFreqAdjustMode(MiniGen::FULL);
  high_freq_gen.setFreqAdjustMode(MiniGen::FULL);
  float frequency = 1000.0;
  unsigned long freqReg = low_freq_gen.freqCalc(frequency);
  low_freq_gen.adjustFreq(MiniGen::FREQ0, freqReg);
  frequency = 30000.0;
  freqReg = high_freq_gen.freqCalc(frequency);
  high_freq_gen.adjustFreq(MiniGen::FREQ0, freqReg);

}

void loop() {
  if(Serial.available() > 0){
    StaticJsonBuffer<200> jsonBuffer;
    String inp = Serial.readStringUntil('\n');
    JsonObject& req = jsonBuffer.parseObject(inp);
    if(req.success()){
      if(req.containsKey("function")){
        char* function;
        function = req["function"];
        // --- set device id ---
        if(String(function) == "set_id"){
          JsonArray& config = jsonBuffer.parseArray(req.get<String>("config"));
          controller.device = config.get<String>(0)[0];
          config.remove(0);
          req["config"] = config;
          saveDeviceId(nonV, controller);
        }
        // --- Get devices ids ---
        if(String(function) == "get_id"){
          // --- If is not the firts device in the line ---
          if(req.containsKey("config")){
            JsonArray& config = jsonBuffer.parseArray(req.get<String>("config"));
            config.add(String(controller.device));
            req["config"] = config;
          }
          // --- If is the firts device in the line ---
          else {
            JsonArray& config = jsonBuffer.createArray();
            config.add(String(controller.device));
            req["config"] = config;
          }
        }
      }
      // --- Get relevant information for the device ---
      if(req.containsKey(String(controller.device))){
        controller.amplitude = req[String(controller.device)]["A"];
        controller.noise = req[String(controller.device)]["N"];
        controller.offset = req[String(controller.device)]["O"];

        amplitude_pot.setPercent(controller.amplitude);
        noise_pot.setPercent(controller.noise);
        offset_pot.setPercent(controller.offset);

        // ::: Set config as Default :::
        if(req[String(controller.device)]["default"]){
          saveController(nonV, controller);
        }

        // ::: Make the response :::
        if(req.containsKey("res")){
          JsonObject& response = jsonBuffer.parseObject(req.get<String>("res"));
          response[String(controller.device)] = "ok";
          req["res"] = response;
        }
        else{
          JsonObject& response = jsonBuffer.createObject();
          response[String(controller.device)] = "ok";
          req["res"] = response;
        }
      // --- Clear the information of the device ---
        req.remove(String(controller.device));
      }
      String output;
      req.printTo(output);
      outSerial.println(output);
    }
  }
}

void saveDeviceId(EEPROMClassEx& mem, VCO_CONTROLLER& contr){
  mem.writeByte(nonv_device, contr.device);
}

void saveController(EEPROMClassEx& mem, VCO_CONTROLLER& contr){
  mem.writeByte(nonv_device, contr.amplitude);
  mem.writeByte(nonv_device, contr.noise);
  mem.writeByte(nonv_device, contr.offset);
}

void readController(EEPROMClassEx& mem, VCO_CONTROLLER& contr){
  contr.device = char(mem.readByte(nonv_device));
  contr.amplitude = int(mem.readByte(nonv_amplitude));
  contr.noise = int(mem.readByte(nonv_noise));
  contr.offset = int(mem.readByte(nonv_offset));
}
