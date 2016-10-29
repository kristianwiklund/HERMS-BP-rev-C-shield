#include "config.h"
#include "Arduino.h"

extern void changeAutoTune();

void serial_setup() {
    Serial.begin(9600);
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
  }
} 
