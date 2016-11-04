

// HLT control - a standard 1A pid regulator

// assumptions after first round of testing:
// the heat in the heating element can be assumed to be irrelevant
// when we are handling large amounts of water (>10 liters). Hence,
// we can use 100% heating until at 1 degree or so from the setpoint, 
// where we turn on the PID

#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
#include "config.h"

double HLTSetpoint, HLTInput, HLTOutput=0;
unsigned long  modelTime, serialTime;
 
unsigned long windowStartTime;
volatile long onTime = 0;
long oldOnTime=0;
// 30 liters autotune: [u'q', u'339.53', u'0.22', u'0.00', u'75.00', u'31.00']

// which mode we are in: no heat, coast to 0.5 degrees below setpoint, PID
enum HLTMode {H_OFF, H_TURBO, H_PID} hltmode = H_PID;

//double kp=679,ki=0,kd=0;
double kp=680,ki=0.5,kd=0;

byte ATuneModeRemember=2;
double aTuneStep=2000, aTuneNoise=1, aTuneStartValue=2000;
unsigned int aTuneLookBack=20;
boolean tuning = false;

PID myPID(&HLTInput, &HLTOutput, &HLTSetpoint, kp,ki,kd, DIRECT);
PID_ATune aTune(&HLTInput, &HLTOutput);

extern void print_regulator_settings();


void hlt_setup() {
  myPID.SetOutputLimits(0,HLTWindowSize);
    windowStartTime = millis();
  myPID.SetSampleTime(HLTSampleTime);
  HLTSetpoint = 10.0;
  writesetpoints();
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, HIGH);

  print_regulator_settings();      
  
}

// Code from Adafruit sous vide -  https://learn.adafruit.com/sous-vide-powered-by-arduino-the-sous-viduino?view=all

// ************************************************
// Called by ISR every 15ms to drive the output
// ************************************************
void DriveOutput()
{  
  long now = millis();
  // Set the output
  // "on time" is proportional to the PID output
  if(now - windowStartTime>HLTWindowSize)
  { //time to shift the Relay Window
     windowStartTime += HLTWindowSize;
  }
  if((onTime > 10) && (onTime > (now - windowStartTime)))
  {
     digitalWrite(RelayPin,LOW);
//     Serial.println("d HLT ON");
  hltheatdisplay("ON", onTime);
  }
  else
  {
  hltheatdisplay("OFF", onTime);
     digitalWrite(RelayPin,HIGH);
//     Serial.println("d HLT OFF");

  }
}

unsigned char x=10;


void AutoTuneHelper(boolean start)
{
  if(start)
    ATuneModeRemember = myPID.GetMode();
  else
    myPID.SetMode(ATuneModeRemember);
}

void changeAutoTune()
{
 if(!tuning)
  {
    //Set the output to the desired starting frequency.
    HLTOutput=aTuneStartValue;
    aTune.SetNoiseBand(aTuneNoise);
    aTune.SetOutputStep(aTuneStep);
    aTune.SetLookbackSec((int)aTuneLookBack);
    AutoTuneHelper(true);
    tuning = true;
    Serial.println("d autotune on");
  }
  else
  { //cancel autotune
    aTune.Cancel();
    tuning = false;
    AutoTuneHelper(false);
    Serial.println("d autotune off");

  }
}

void hlt_pid() {
    if(tuning)
  {
    byte val = (aTune.Runtime());
    if (val!=0)
    {
      tuning = false;
    }
    if(!tuning)
    { //we're done, set the tuning parameters
      kp = aTune.GetKp();
      ki = aTune.GetKi();
      kd = aTune.GetKd();
      myPID.SetTunings(kp,ki,kd);
      print_regulator_settings();
      AutoTuneHelper(false);
    }
  } else
    myPID.Compute();

}

void hlt_turbo_mode() {

  // go to turbo heating mode if we are sufficiently far below the setpoint
  
      if (( HLTInput < (HLTSetpoint - 0.5))) {
        hltmode = H_TURBO;
        myPID.SetMode(MANUAL);
                HLTOutput = HLTWindowSize; // 100% output
        Serial.println("d turbo mode");
      }
  
}

void hlt_control() {

  unsigned long now = millis();

  HLTInput = HLTTemp;

  // check the temperatures. if we are outside the range where we PID, go to turbo (and vice versa)
  switch(hltmode) {
    case H_OFF:
      HLTOutput = 0;
      break;
    case H_PID:  

      break;  
    case H_TURBO:
      if (HLTInput >= (HLTSetpoint)) {
        hltmode = H_PID;
            HLTOutput = 0; // start clean
        
        myPID.SetMode(AUTOMATIC);
        Serial.println("d pid mode");
      }

      break;
  }

   hlt_pid();

  if(x++ > 10) { // print  temperature once per second  
    Serial.print("th ");
    Serial.print(HLTTemp);
    Serial.println("");  
    x=0;
  }


  onTime = HLTOutput;
  if(onTime!=oldOnTime) {
    Serial.print("ph ");
    Serial.println(onTime/(HLTWindowSize/100));
    oldOnTime=onTime;
  }
  DriveOutput();
}









