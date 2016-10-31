#
# simple machine for running a mash program
#
# the machine consumes a text file with commands
#
# tm <temp> set mash setpoint to temperature <temp>. Will cause error if the HLT is colder than <temp>
# th <temp> set HLT setpoint to temperature <temp>
# wait_time <minutes> wait for <minutes> minutes, then continue
# stepname <text> displays the <text> as step name in the gui
# wait_tm <temp> wait until mash temp is <temp>
# wait_th <temp> wait until hlt temp is <temp>
# beep sound the alarm
# requester <text> show a requester with text and wait for the user to click OK

class Program:

	oldstep=255
	xdata = {}
	ydata = {}
	xdata["HLT"] = []
	ydata["HLT"] = []
	xdata["MLT"] = []
	ydata["MLT"] = []


	def __init__(self, w, bt,stepWidgets,nextwidget,stopalarmwidget,plot):
	  self.stepWidgets = stepWidgets
	  nextwidget.clicked.connect(self.nextstep)
	  stopalarmwidget.clicked.connect(self.stopalarm)
	  self.plot = plot
	  self.bt = bt

	def stopalarm(self):
	  # code to change the alarm indicator back to inactive
	  self.bt.stopAlarm()

	def nextstep(self):
	  self.bt.advStep()

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

