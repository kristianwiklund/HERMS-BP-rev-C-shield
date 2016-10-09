#include "config.h"


// the MLT control code works by controlling the recirculation.
// if the MLT is too cold, the recirculation flow goes through the HLT
// if the MLT is too warm, the recirculation flow does not go through the HLT
// the pins have to be inverted, if valveherms is on, valverecirc need to be off

float MLTTemp=33.0;
double MLTSetpoint=31.0;

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

  switch(state) {

    case MLT_OFF:
    case MLT_COOL:
      if(MLTTemp<MLTSetpoint) {
        state=MLT_HEAT;
        herms_on();
      }
        break;
    case MLT_HEAT:
      if(MLTTemp>MLTSetpoint) {
        state=MLT_COOL;
        herms_off();
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

