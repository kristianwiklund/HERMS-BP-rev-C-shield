#include "BrewpiPins.h"
#include <DallasTemperature.h>
#define RelayPin actuatorPin2
#define valverecirc actuatorPin1
#define valveherms actuatorPin4

extern void mlt_control();
extern void hlt_control();
extern void poll_serial();
extern void update_display();
extern void thermo_loop();

extern void display_setup();
extern void hlt_setup();
extern void serial_setup();
extern void thermo_setup();
extern void mlt_setup();

#define TEMPERATURE_PRECISION 9

extern float HLTTemp,MLTTemp;
extern double HLTSetpoint, HLTOutput;
extern double MLTSetpoint;

#define HLTWindowSize 5000.0 // 5 seconds PID window for the relay
#define HLTSampleTime 500.0 // 0.5 seconds sampling time for the PID

extern void hltheatdisplay(const char *, double);
extern void writesetpoints();
extern void mltcircdisplay(const char *);
