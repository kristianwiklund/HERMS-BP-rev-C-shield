#!/usr/bin/python


from PyQt4.QtCore import *		# Qt core
from PyQt4.QtGui import *		# Qt GUI interface
from PyQt4.uic import *			# ui files realizer
from PyQt4 import QtGui, uic
from HBPSerial import *
from mash import *
from functools import *
from Program import *
from matplotlib.backends import qt_compat
use_pyside = qt_compat.QT_API == qt_compat.QT_API_PYSIDE
if use_pyside:
    from PySide import QtGui, QtCore
else:
    from PyQt4 import QtGui, QtCore
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
import matplotlib.pyplot as plt

# for example / remove later
from numpy import arange, sin, pi
# for example / end


import sys				# system support

class TehFigure:
	def __init__(self, layoutwidget):
		self.figure = Figure(figsize=(5,4), dpi=100) # magic numbers
		self.axes1=self.figure.add_subplot(111) # more magic
		self.axes2=self.figure.add_subplot(111) # more magic
		self.canvas = FigureCanvas(self.figure)
		self.lines1, = self.axes1.plot([],[], '-')
		self.lines2, = self.axes2.plot([],[], '-')
		self.axes1.set_ylim(0, 100)        
		self.axes2.set_ylim(0, 100)        
		self.compute_initial_figure()
		self.canvas.updateGeometry()
		layoutwidget.addWidget(self.canvas)
        
	def compute_initial_figure(self):
		pass

	def update_plot(self, xdata, ydata):
		# we need to split this somehow.

		#Update data (with the new _and_ the old points)
		self.lines1.set_xdata(xdata["HLT"])
		self.lines1.set_ydata(ydata["HLT"])
		self.lines2.set_xdata(xdata["MLT"])
		self.lines2.set_ydata(ydata["MLT"])
		#Need both of these in order to rescale
		self.axes1.relim()
		self.axes1.autoscale_view()
		self.axes2.relim()
		self.axes2.autoscale_view()
		#We need to draw *and* flush
		self.figure.canvas.draw()
		self.figure.canvas.flush_events()

	def annotate(self, x, y, text):
		self.axes1.annotate(text, xy=(x,y),xytext=(x,50),arrowprops=dict(facecolor='black', arrowstyle="->"))


UI_FILE = 'lodda.ui'		# qt ui descriptor

class XTun(Tun):

	setpointsaved = True
	manualsetpoint = 0 

	def setSetpointManually(self, value):
		self.setpointsaved = False
		self.setPointWidget.display(value)
		self.setPointWidget.setStyleSheet("QLCDNumber{color:blue;}")
		self.manualsetpoint = value

	def setSetpointSave(self):
		if not self.setpointsaved:
			self.setPointWidget.setStyleSheet("QLCDNumber{color:yellow;}")
			self.setpointsaved = True
			self.bt.setSetpoint(self.id, self.manualsetpoint)
        
	def __init__(self, w, bt, myid, setpoint, temperature, setbutton, dial, pwrlabel):
		Tun.__init__(self,bt, myid)
		self.setPointWidget = setpoint
		self.dialWidget = dial
		self.temperatureWidget = temperature
		w.connect(dial,SIGNAL("valueChanged(int)"), partial(XTun.setSetpointManually,self))
		self.setbuttonWidget = setbutton
		setbutton.clicked.connect(partial(XTun.setSetpointSave,self))
		self.pwrLabel = pwrlabel
  
	def update(self):
		if self.setpointsaved:
			Tun.update(self)
			if self.newsetpoint != self.setpoint:
				self.setPointWidget.display(self.newsetpoint)
				self.setPointWidget.setStyleSheet("QLCDNumber{color:green;}")
				self.setpoint = self.newsetpoint
				self.manualsetpoint = self.setpoint
				#print("new setpoint "+str(self.setpoint))

			if (self.newtemperature < 200) and (self.newtemperature > -20): # disconnected onewire results in weird numbers.
				if self.newtemperature != self.temperature:
					self.temperatureWidget.setDecMode()
					self.temperatureWidget.display(self.newtemperature)
					self.temperatureWidget.setStyleSheet("QLCDNumber{color:red;}")
					self.temperature = self.newtemperature
				
			else:
				self.temperatureWidget.setHexMode()
				self.temperatureWidget.display(int("dead",16))
				
			if (self.newpower != self.power):
				self.power = self.newpower
				self.pwrLabel.setText(str(int(self.power))+"%")

                    

                    
          
class MainWin(QtGui.QMainWindow):
     def __init__(self,bt):
         QtGui.QMainWindow.__init__(self)
         self.bt = bt

         # Set up the user interface from Designer.
         self.ui = uic.loadUi(UI_FILE)
         self.ui.show()

         sc = TehFigure(self.ui.plotlayout)

         self.HLT = XTun(self.ui, bt, "h", self.ui.HLTSet, self.ui.HLTTemp, self.ui.toggleHLT, self.ui.HLTdial,self.ui.HLTPower)
         self.MLT = XTun(self.ui, bt, "m", self.ui.MLTSet, self.ui.MLTTemp, self.ui.toggleMLT, self.ui.MLTdial,self.ui.MLTPower)

         self.programstatus = Program(self.ui, bt,sc)



         # init callbacks


     

     # callback function

     def updateui(self):
          self.MLT.update()
          self.HLT.update()
          self.programstatus.update()


### main

bt = HBPSerial("COM5:")
app = QtGui.QApplication(sys.argv)
window = MainWin(bt)

# set a timer that update the status every  second
timer = QTimer()
timer.timeout.connect(window.updateui)
timer.start(1000)


sys.exit(app.exec_())

