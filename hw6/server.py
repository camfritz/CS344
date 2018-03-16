#!/usr/bin/env python

import socket
import sys
import threading
import signal

#create global server socket, mutex, port number, accounts dictionary, and threads array
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
mut = threading.Lock()
portNumber = int(sys.argv[1])
accounts = {}
threads = []

#Handles the interrupt SIGINT signal, join all threads, write to file and close server socket
def handleSignal(signal, arg):
	for thread in threads:
		thread.join()
	file = open('log.txt', 'w')
	for key, value in sorted(accounts.items()):
		file.writelines('Name: ' + key +', ' + 'Balance: ' + str(value) + '\n')
	file.close()
	s.close()
	exit(1)

#Parse each line and handle the transaction accordingly
def handleTransaction(line):
	transactionArray = []
	transactionArray = line.split()

	name = transactionArray[0]
	transactionType = transactionArray[1]

	if(transactionArray[2][0] == '$'):
		amount = float(transactionArray[2][1:])
	else:
		amount = float(transactionArray[2])

#Acquire the mutex, modify global data dictionary, release mutex when finished
	mut.acquire()
	if name in accounts.keys():
		if(transactionType == 'credit' or transactionType == 'Credit'):
			accounts[name] += amount
		elif(transactionType == 'debit' or transactionType == 'Debit'):
			accounts[name] -= amount
	else:
		if(transactionType == 'credit' or transactionType == 'Credit'):
			accounts[name] = amount
		elif(transactionType == 'debit' or transactionType == 'Debit'):
			accounts[name] = -amount
	mut.release()

#Recieve each character of a line until a newline character is reached. Call the transaction handler, close connection at the end of file
def acquireTransaction(conn):
	while(True):
		data = conn.recv(1)
		if not data:
			break
		transactionLine = ''
		while(data != '\n'):
			transactionLine += data
			data = conn.recv(1)
		handleTransaction(transactionLine)
	print('OK\n')
	conn.close()

#Create server socket
signal.signal(signal.SIGINT, handleSignal)
try:
	s.bind(('127.0.0.1', portNumber))
	while(1):
		s.listen(5)

#create threads for each client
		(conn, addr) = s.accept()
		t = threading.Thread(target=acquireTransaction, args=(conn,))
		threads.append(t)
		t.start()

except Exception as e:
	print(e)
	exit(1)


