#!/usr/bin/env python

import socket
import sys

portNumber = int(sys.argv[1])
inputFileName = sys.argv[2]

#Open the input file
try:
	inputFile = open(inputFileName, "r")

except Exception as e:
	print(e)
	exit(1)

#Create client socket
try:
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect(('127.0.0.1', portNumber))

#send transactions to server
	for line in inputFile:
		s.send(line)

	s.close()

except Exception as e:
	print(e)
	exit(1)

