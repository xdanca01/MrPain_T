#!/usr/bin/env python3

import socket
import sys
import string

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
Server = '127.0.0.1'
clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientSocket.bind((Server, ServerPort))
clientSocket.listen()
conn, addr = clientSocket.accept()
