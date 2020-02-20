#!/usr/bin/env python3

import socket
import sys
import string
import os
import re

argc = len(sys.argv)
if argc != 2:
    print("bad count of arguments. need:2 got:",argc)
    print("please provide PORT in argument")
    exit(1)

if not sys.argv[1].isdigit():
    print("argument is not made of numbers only:",sys.argv[1])
    exit(2)

#PORT
ServerPort = int(sys.argv[1])
#localhost
SIZE = 1024
Server = '127.0.0.1'
clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientSocket.bind((Server, ServerPort))
clientSocket.listen()
while True:
    conn, addr = clientSocket.accept()
    commID = os.fork()
    #child leave
    if commID == 0:
        break
#create new connection with client addr on socket conn
print("connection with client",addr,"ESTABLISHED")
BUFFER = conn.recv(SIZE)
BUFFER = BUFFER.decode('utf-8')
if not BUFFER:
    print("nothing received")
REQUEST = re.search("GET", BUFFER)
print(REQUEST)



print("konec")
conn.close()
exit(0)
