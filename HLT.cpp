

// HLT control - a standard 1A pid regulator
// to be added: Autotune

#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
#include "config.h"

double HLTSetpoint, Input, HLTOutput=0;
unsigned long windowStartTime;
volatile long onTime = 0;

PID myPID(&Input, &HLTOutput, &HLTSetpoint, 2,5,1, DIRECT);

void hlt_setup() {

  myPID.SetOutputLimits(0,HLTWindowSize);
  myPID.SetMode(AUTOMATIC);
  windowStartTime = millis();
  myPID.SetSampleTime(HLTSampleTime);
  HLTSetpoint = 30.0;
  writesetpoints();
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, HIGH);
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
  if((onTime > 100) && (onTime > (now - windowStartTime)))
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

void hlt_control() {
  if(x++ > 10) { // print  temperature once per second  
    Serial.print("th ");
    Serial.print(HLTTemp);
    Serial.println("");
    x=0;
  }
  
  Input = HLTTemp;
  myPID.Compute();
//  Serial.print("d HLT Window: ");
//  Serial.println(HLTOutput);
  onTime = HLTOutput;
  DriveOutput();
}









