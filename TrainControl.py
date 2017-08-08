#!/usr/bin/python3

import serial






class ArdComm:
	def __init__( self ):
		PORT = "/dev/cu.usbserial-A9007OOs"
		TIMEOUT = 1
		BAUDRATE = 38400
		self.ardCommPort = serial.Serial( PORT, baudrate=BAUDRATE, timeout=TIMEOUT )
		self.ardCommPort.flushInput()
		self.ardCommPort.flushOutput()

	def sendCommand( self, cmdString ):
		self.ardCommPort.write( cmdString )
		
	def getResponse( self ):
		return self.ardCommPort.readline().strip()
	


