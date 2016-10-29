#include <OneWire.h>
#include <DallasTemperature.h>
#include "config.h"

// contains code from the DallasTemperature library examples.

OneWire oneWire(oneWirePin);
DallasTemperature sensors(&oneWire);

DeviceAddress HLTThermo = {0x28, 0x8B, 0x1B, 0x6E, 0x05, 0x00, 0x00, 0xB0 };
float HLTTemp=100.0;
DeviceAddress MLTThermo = {0x28, 0x8B, 0x1B, 0x6E, 0x05, 0x00, 0x00, 0xB0 };



// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("d Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
}

// function to print a device's resolution
void printResolution(DeviceAddress deviceAddress)
{
  Serial.print("d Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();    
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  Serial.print("d Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}

void thermo_setup() {
  sensors.begin(); // dallas init

  
  // locate devices on the bus
  Serial.print("d Locating devices...");
  Serial.print("d Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

 
  // Search for devices on the bus and assign based on an index. Ideally,
  // you would do this to initially discover addresses on the bus and then 
  // use those addresses and manually assign them (see above) once you know 
  // the devices on your bus (and assuming they don't change).
  // 
  // method 1: by index
  if (!sensors.getAddress(HLTThermo, 0)) Serial.println("d Unable to find address for Device 0"); 
  if (!sensors.getAddress(MLTThermo, 1)) Serial.println("d Unable to find address for Device 1"); 


  // show the addresses we found on the bus
  Serial.print("d Device 0 Address: ");
  printAddress(HLTThermo);
  Serial.println();

  // show the addresses we found on the bus
  Serial.print("d Device 1 Address: ");
  printAddress(MLTThermo);
  Serial.println();



  // set the resolution to 9 bit per device
  sensors.setResolution(HLTThermo, TEMPERATURE_PRECISION);
  sensors.setResolution(MLTThermo, TEMPERATURE_PRECISION);

    sensors.requestTemperatures();

}

void thermo_loop() {
 if (sensors.isConversionAvailable(0))
  {
    HLTTemp = sensors.getTempC(HLTThermo);     
    MLTTemp = sensors.getTempC(MLTThermo);
    sensors.requestTemperatures();
  }

}

