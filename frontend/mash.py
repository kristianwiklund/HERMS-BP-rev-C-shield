#import curses
import time
from serial import *

# keep track of a liquid tun
class Tun:
    temperature=-3222
    power=-223
    setpoint=-443
    # to store data over time
    xdata = []
    ydata = []


    def __init__(self,bt,myid):
        self.id = myid
        self.bt = bt

    def update(self):
        self.newtemperature=float(self.bt.getTemp(self.id))
        self.newsetpoint=float(self.bt.getSetpoint(self.id))
        self.newpower=self.bt.getHeatpwr(self.id)

# keep track of the HLT recirculation
# bt assumptions:
# bt configured to four outputs, only one is used for heating.
# continous recirculation of the HLT
# mash pump turned on manually (or by linux) when the MLT is filled

# pin assignments:
 #define VALVE1_PIN 20 //OUT3 - HLT recirc
 #define VALVE2_PIN 19 //OUT4 - Fill MLT
 #define VALVE3_PIN 18 //OUT5 - MLT recirc
 #define VALVE4_PIN 21 // OUT2 - Fill Boil
 #define HLTHEAT_PIN 22 //OUT1


class HLTRecirc:
    state=-1234 # 0 - valve 0, 1 - valve 1 
    newstate=0

    def update(self):
        # here is code to check the state of the valves
        # change "newstate"
        self.newstate=self.state

    def __init__(self,bt):
        self.bt = bt
        HLTRecirc.update(self)
        
