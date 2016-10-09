#include <BrewpiBuzzer.h>
#include <BrewpiLCD.h>
#include <BrewpiPins.h>
#include <BrewpiPrint.h>
#include "config.h"

SpiLcd lcd;
char buf[10];

// ftoa from skumlerud, http://forum.arduino.cc/index.php?topic=44262.0
char *ftoa(char *a, double f, int precision)
{
 long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
 
 char *ret = a;
 long heiltal = (long)f;
 itoa(heiltal, a, 10);
 while (*a != '\0') a++;
 if(precision>0) {
   *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
 } else
    *a++=0; 
 return ret;
}


void hltheatdisplay(const char *s, double n){
  lcd.setCursor(10,1);
  lcd.write(s);
  lcd.write(" ");
  lcd.setCursor(14,1);
  lcd.write(ftoa(buf,n/(HLTWindowSize/100),0));
  lcd.write("%");
  lcd.printSpacesToRestOfLine();
}

void mltcircdisplay(const char *s){
  lcd.setCursor(12,3);
  lcd.write(s);
  lcd.printSpacesToRestOfLine();
}

void writetemps() {
  lcd.setCursor(4,0);
  lcd.write(ftoa(buf,HLTTemp,1));
   lcd.write((char)223);
   lcd.write(" ");
  lcd.setCursor(4,2);
  lcd.write(ftoa(buf,MLTTemp,1));
   lcd.write((char)223);
   lcd.write(" ");
}

void writesetpoints() {
  lcd.setCursor(15,0);
  lcd.write(ftoa(buf,HLTSetpoint,1));
  lcd.setCursor(15,2);
  lcd.write(ftoa(buf,MLTSetpoint,1));
}
void writefixedtext() {
  lcd.setCursor(0,0);
  lcd.write("HLT:");
 
  lcd.setCursor(11,0);
  lcd.write(" S: ");

  lcd.setCursor(0,2);
  lcd.write("MLT:");

  lcd.setCursor(11,2);
  lcd.write(" S: ");

  lcd.setCursor(0,1);
  lcd.write("HLT Heat: ");

  lcd.setCursor(0,3);
  lcd.write("MLT Recirc: ");
  
}

void display_setup() {
     // display
 lcd.init();
 lcd.begin(20,4);
 lcd.home();
 lcd.write("Starting...");

  lcd.clear();
  writefixedtext();
  writetemps();
  writesetpoints();

}

void update_display() {
  writetemps();
}

