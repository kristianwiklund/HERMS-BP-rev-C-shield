#include <BrewpiBuzzer.h>
#include <BrewpiLCD.h>
#include <BrewpiPins.h>
#include <BrewpiPrint.h>
#include "config.h"

// this uses global variables, hacks, solutions. Be afraid.


// Plan: 
// We need two different softwares: one is a simple PID regulator (done), the other is a bit-banger that will keep the valves going
// The components needed are the PID library + autotuner, 
// I was leaning towards Firmata for serial command thingy but it is too complex so lets use a standard 1A serial readline thing
// We need the following commands:
// x - stop everything and disable all outputs. This disables heating and pumps and sets the valves in "recirculation, no heat"
// s - start
// t<vessel> <celsius> - temperature setpoint
// p <p>, i <i>, d <d> - regulator parameters
// q - read the PID parameters
// a - initiate autotune

// return values:
// "t<h/m> <celsius>" - actual temperature in the thing
// "q <p> <i> <d> <htl setpoint> <mlt setpoint>" - sent either when receiving a q command OR a t command or (unimplemented) when changing the setpoint with the rotary encoded
// "e <error message>"
// "d <debug message>"
// "p<h/m>" which output power the thing is set to

Buzzer buzzer;


void setup() {
 serial_setup();
  display_setup();
  hlt_setup();
  thermo_setup();
  mlt_setup();
  
 // buzzer
  buzzer.init();
  buzzer.beep(3,100);


}

void loop() {

  delay(100); // sleep 1s for debug purposes
  thermo_loop();
  poll_serial();
  hlt_control();
  mlt_control();
  update_display();
 

}
