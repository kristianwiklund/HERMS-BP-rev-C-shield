import serial
import threading

class HBPSerial:

	def parse(self,s):
		# mechanisms to update data structures
		arr = s.split()
		print(arr)
		
		if arr[0]=='th':
			self.d["h"]["temp"] = arr[1]
	#		print("set temp "+arr[0]+" to "+arr[1])
			
	def read_from_port(self):
		print("starting serial reader on port "+self.port)
		while True:
			s = self.ser.readline().decode()
			self.parse(s)

	def __init__(self,port):
		self.port = port
		self.ser = serial.Serial(port=self.port,baudrate=9600)
		self.d={}
		self.d["m"]={}
		self.d["h"]={}

		self.d["m"]["setpoint"] = 0
		self.d["h"]["setpoint"] = 0
		self.d["m"]["temp"] = 0
		self.d["h"]["temp"] = 0
		self.thread = threading.Thread(target=self.read_from_port)
		self.thread.start()
		
		
	def command(self,s):
		self.ser.write(s)
	
	def autotune(self):
		self.command("a")

	def getTemp(self,tunid):
		return self.d[tunid]["temp"]

    # get the setpoint of a tun
	def getSetpoint(self,tunid):
		return 30.0

    # get the heating power of a tun
	def getHeatpwr(self,tunid):
		pass
		
	def getProgramStep(self):
		pass

	def getFullStatus(self):
		pass
		
	def stopAlarm(self):
		pass
		
	def advStep(self):
		pass
		
	def getProgram(self, progid):
		pass

