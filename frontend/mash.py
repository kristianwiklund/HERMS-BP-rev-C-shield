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



