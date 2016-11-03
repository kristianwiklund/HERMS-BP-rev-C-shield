#include "config.h"
#include "Arduino.h"
#include "BrewpiBuzzer.h"
#include <PID_v1.h>

extern PID myPID;

extern Buzzer buzzer;

extern void changeAutoTune();
extern double HLTSetpoint, HLTSetpoint;

extern double kp,ki,kd;

extern void writesetpoints();

void print_regulator_settings() {
      Serial.print("q ");
      Serial.print(kp);
      Serial.print(" ");
      Serial.print(ki);
      Serial.print(" ");
      Serial.print(kd);
      Serial.print(" ");
      Serial.print(HLTSetpoint);
      Serial.print(" ");
      Serial.println(MLTSetpoint);
}

void serial_setup() {
    Serial.begin(9600);
}

void readandsetpid() {
  char param;
  double value;
  
  if(!Serial.available())
    return;

  buzzer.beep(4,100);

  param = Serial.read();
  value = Serial.parseFloat();

  switch(param) {

    case 'p':
      kp = value;
      break;
    case'i':
      ki = value;
      break;
    case 'd':
      kd = value;
      break;  
  }

    myPID.SetTunings(kp,ki,kd);
    print_regulator_settings();


}

void readandsettemp() {

  char vessel;
  double temperature;
  
  if(!Serial.available())
    return;

  buzzer.beep(1,100);

  vessel = Serial.read();
  temperature = Serial.parseFloat();

  switch(vessel) {
    case 'h':
      HLTSetpoint = temperature;
      break;
    case 'm': 
  	  MLTSetpoint = temperature;
      break;
  }

  print_regulator_settings();

  writesetpoints();
}

void poll_serial() {
  char command;
  
  if(!Serial.available())
    return;

  command=Serial.read();

  switch(command) {
      case 'a':
        buzzer.beep(2,100);
        changeAutoTune();
        break;
      case 't':
        readandsettemp();
        break;
      case 'k':
        readandsetpid();
        break;  
        
  }
} 
