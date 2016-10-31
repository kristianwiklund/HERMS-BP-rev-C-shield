import serial
import threading
import time

class HBPSerial:

	serialisup = False
	
	def serialAvailable(self):
		return self.serialisup;

	def parse(self,s):
		# mechanisms to update data structures
		arr = s.split()
		print(arr)
		
		if arr[0] in {"tm","th","ph","pm"}:
			self.d[arr[0]] = arr[1]
		elif arr[0]=="q":
			self.serialisup = True
			self.d["kp"] = arr[1]
			self.d["ki"] = arr[2]
			self.d["kd"] = arr[3]
			self.d["sh"] = arr[4]
			self.d["sm"] = arr[5]
		else:
			print "unimplemented response "+s
			
	def read_from_port(self):
		print("starting serial reader on port "+self.port)
		while True:
			s = self.ser.readline().decode()
			self.parse(s)

	def __init__(self,port):
		self.port = port
		self.start_time = time.time()
		self.ser = serial.Serial(port=self.port,baudrate=9600)
		self.d={}

		self.d["sm"] = 0
		self.d["sh"] = 0
		self.d["tm"] = 0
		self.d["th"] = 0
		self.thread = threading.Thread(target=self.read_from_port)
		self.thread.start()
		
		
	def command(self,s):
		self.ser.write(s)
	
	def autotune(self):
		self.command("a")

	def getTemp(self,tunid):
		return self.d["t"+tunid]

    # get the setpoint of a tun
	def getSetpoint(self,tunid):
		return self.d["s"+tunid]

	def setSetpoint(self,tunid,value):
		self.command("t"+tunid+" "+str(value))
	
    # get the heating power of a tun
	def getHeatpwr(self,tunid):
		if "p"+tunid in self.d:
			return float(self.d["p"+tunid])
		else:
			return 0.0
	
	def getProgramStep(self):
		return(255)

	def getFullStatus(self):
		s = {}
		s["timestamp"] = time.time() - self.start_time # seconds since script started
		s["HLT"] = {'temp':float(self.d["th"]),'setpoint':float(self.d["sh"])}
		s["MLT"] = {"temp":float(self.d["tm"]),"setpoint":float(self.d["sm"])}
		
		return(s)
		
	def stopAlarm(self):
		pass
		
	def advStep(self):
		pass
		
	def getProgram(self, progid):
		pass

