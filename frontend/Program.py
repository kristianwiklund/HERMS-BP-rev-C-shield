from PyQt4.QtCore import *		# Qt core
from PyQt4.QtGui import *		# Qt GUI interface
from PyQt4.uic import *			# ui files realizer
from PyQt4 import QtGui, uic
from operator import methodcaller
import threading
import time
#
# simple machine for running a mash program
#
# the machine consumes a text file with commands
#
# tm <temp> set mash setpoint to temperature <temp>. Will cause error if the HLT is colder than <temp>
# th <temp> set HLT setpoint to temperature <temp>
# wait_time <seconds> wait for <seconds> seconds, then continue
# stepname <text> displays the <text> as step name in the gui
# wait_tm <temp> wait until mash temp is <temp>
# wait_th <temp> wait until hlt temp is <temp>
# beep sound the alarm
# requester <text> show a requester with text and wait for the user to click OK


class MyTableModel(QAbstractTableModel):
	def __init__(self, datain, parent=None, *args):
		QAbstractTableModel.__init__(self, parent, *args)
		self.arraydata = datain

	def rowCount(self, parent):
		return len(self.arraydata)

	def columnCount(self, parent):
		return len(self.arraydata[0])

	def data(self, index, role):
		#print(index)
		if not index.isValid():
			return QVariant()
		elif role != Qt.DisplayRole:
			return QVariant()
		return QVariant(self.arraydata[index.row()][index.column()])


class Program:

	oldstep=-1
	step=-1
	running = False
	runprogram = True
	waittime=time.time()+10
	waittemp=False
	
	xdata = {}
	ydata = {}
	xdata["HLT"] = []
	ydata["HLT"] = []
	xdata["MLT"] = []
	ydata["MLT"] = []

	def execute_program(self):
		print("start program thread")
		while self.runprogram:
			time.sleep(0.1)	
			if self.running and self.bt.serialAvailable():
				
				if self.waittime>time.time():
					continue
				
				if self.waittemp:
					if self.bt.getTemp(self.waittemp[0][1])<self.waittemp[1]:
						continue
					else:
						self.waittemp=False
						print "done waiting for temp"
				
				print(self.nextstep())
				command = self.program[self.step][0]
				argument = self.program[self.step][1]
				
				if command in {"th","tm"}:
					print("setpoint "+command)
					self.bt.setSetpoint(command[1],argument)
				elif command in  {"wait_th","wait_tm"}:
					print("waiting "+command)
					self.waittemp=[command.split("_")[1],argument]
				elif command=="wait_time":
					self.waittime = time.time()+int(argument)
				elif command=="autotune":
					print ("Autotuning - exiting program control")
					self.bt.autotune()
					self.runprogram=False
				else:
					print("fel error in program")
					self.runprogram=False
				
	def __init__(self, w, bt,plot,table):

		self.plot = plot
		self.bt = bt
		self.program = list()
		self.tableWidget = table
		self.thread = threading.Thread(target=self.execute_program)
		self.thread.start()
		
		
	def load(self,filename):
		with open(filename) as f:
			self.program = map(methodcaller("split", " "), f.read().splitlines())
			#print(self.program)
	  		self.tablemodel = MyTableModel(self.program)
			self.tableWidget.setModel(self.tablemodel)
			#self.tableWidget.layoutChanged.emit()
			self.step = -1
			self.oldstep = -1
			self.tableWidget.setDisabled(1)
			
	def stopalarm(self):
	  # code to change the alarm indicator back to inactive
	  self.bt.stopAlarm()

	def nextstep(self):
		self.step = self.step + 1;
		
		if(self.step>=len(self.program)):
			self.step=-1
			self.running=False
			return list()

		else:
			self.tableWidget.selectRow(self.step)
			return self.program[self.step]

	def update(self):
	
		# need to update the progress bars and display which step is active
		# change this
		fullstatus = self.bt.getFullStatus()
		#               print ("step" + str(brewstep))


		self.xdata["HLT"].append(fullstatus["timestamp"])
		self.ydata["HLT"].append(float(fullstatus["HLT"]["temp"]))

		self.xdata["MLT"].append(fullstatus["timestamp"])
		self.ydata["MLT"].append(float(fullstatus["MLT"]["temp"]))
		#print self.ydata

		self.plot.update_plot(self.xdata, self.ydata)

	def run(self):
		self.running=True

