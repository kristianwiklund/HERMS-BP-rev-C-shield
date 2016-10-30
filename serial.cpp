#include "config.h"
#include "Arduino.h"

extern void changeAutoTune();
extern double HLTSetpoint;
extern void print_pid_settings();
extern void writesetpoints();

void serial_setup() {
    Serial.begin(9600);
}

void readandsettemp() {

  char vessel;
  double temperature;
  
  if(!Serial.available())
    return;

  vessel = Serial.read();
  temperature = Serial.parseFloat();

  if(vessel=='h') {
    HLTSetpoint = temperature;
    print_pid_settings();
  }
  writesetpoints();
}

void poll_serial() {
  char command;
  
  if(!Serial.available())
    return;

  command=Serial.read();

  switch(command) {
      case 'a':
        changeAutoTune();
        break;
      case 't':
        readandsettemp();
        break;
        
  }
} 
