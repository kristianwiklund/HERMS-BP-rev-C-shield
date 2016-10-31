#include "config.h"


// the MLT control code works by controlling the recirculation.
// if the MLT is too cold, the recirculation flow goes through the HLT
// if the MLT is too warm, the recirculation flow does not go through the HLT
// the pins have to be inverted, if valveherms is on, valverecirc need to be off

float MLTTemp=33.0;
double MLTSetpoint=31.0;
unsigned char xx=10;

void herms_off() {
  digitalWrite(valveherms, HIGH); // signals are INVERTED with the rev C shield
  digitalWrite(valverecirc, LOW); // signals are INVERTED with the rev C shield
  mltcircdisplay("Bypass");
}

void herms_on() {
  digitalWrite(valveherms, LOW); // signals are INVERTED with the rev C shield
  digitalWrite(valverecirc, HIGH); // signals are INVERTED with the rev C shield
  mltcircdisplay("HERMS");
}


void mlt_setup() {
  pinMode(valveherms, OUTPUT);
  pinMode(valverecirc, OUTPUT);

  herms_off();
}

enum MLTState {MLT_OFF=0,MLT_HEAT,MLT_COOL,MLT_HPEAK, MLT_LPEAK} state=MLT_OFF;

void mlt_control() {

  if(xx++ > 10) { // print  temperature once per second  
    Serial.print("tm ");
    Serial.print(MLTTemp);
    Serial.println("");
    xx=0;
   }

  switch(state) {

    case MLT_OFF:
    case MLT_COOL:
      // only recirc through the coil of the HLT is hot enough
      if((MLTTemp<MLTSetpoint) && (HLTTemp>=MLTTemp)) {
        state=MLT_HEAT;
        herms_on();
        Serial.println("pm 100");
      }
        break;
    case MLT_HEAT:
      if(MLTTemp>MLTSetpoint) {
        state=MLT_COOL;
        herms_off();
        Serial.println("pm 0");
      }
      break;

   case MLT_HPEAK:
    // add code to do peak detection to be able to predict the overshoot
    break;

    
   case MLT_LPEAK:
    // add code to do peak detection to be able to predict the overshoot
    break;

  }
  
}

