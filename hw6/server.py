import socket
import sys
import threading

mut = threading.Lock()
portNumber = int(sys.argv[1])
accounts = {}
transactionArray = []

def acquireTransaction(conn):
	while(True):
		data = conn.recv(1)
		if not data:
			break
		transactionLine = ''
		while(data != '\n'):
			transactionLine += data
			data = conn.recv(1)
		print(transactionLine)

#Create server socket
try:
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind(('127.0.0.1', portNumber))
	while(1):
		s.listen(5)

		(conn, addr) = s.accept()
		t = threading.Thread(target=acquireTransaction, args=(conn,))
		t.start()
		t.join()

		# transactionArray = transactionLine.split()
		# print('Name: ' + transactionArray[0])
		# print('Type: ' + transactionArray[1])
		# print('Amount: ' + transactionArray[2] + '\n')

		# name = transactionArray[0]
		# transactionType = transactionArray[1]
		# amount = float(transactionArray[2][1:])


		# if name in accounts.keys():
		# 	if(transactionType == 'credit' or transactionType == 'Credit'):
		# 		accounts[name] += amount
		# 	elif(transactionType == 'debit' or transactionType == 'Debit'):
		# 		accounts[name] -= amount
		# else:
		# 	if(transactionType == 'credit' or transactionType == 'Credit'):
		# 		accounts[name] = amount
		# 	elif(transactionType == 'debit' or transactionType == 'Debit'):
		# 		accounts[name] = -amount

		# for key, value in accounts.items():
		# 	print key + ' : ' + str(value)

		conn.send('OK\n')
		conn.close()

	s.close()

except Exception as e:
	print(e)
	exit(1)


