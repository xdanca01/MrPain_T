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
#MAXSIZEOFBUFFER
SIZE = 4096
#localhost server
Server = '127.0.0.1'
clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientSocket.bind((Server, ServerPort))
clientSocket.listen(1)
while True:
    conn, addr = clientSocket.accept()
    #create new connection with client addr on socket conn
    BUFFER = conn.recv(SIZE).decode()
    if not BUFFER:
        print("unknown method")
        conn.send(b'HTTP/1.1 405 Method Not Allowed\n')
    else:
        REQUEST = re.search("GET", BUFFER)
        if REQUEST:
            print(BUFFER)
            GET = re.findall('=.*&', BUFFER)
            GET = GET[0][1:-1]
            REQUEST = re.findall('=A HTTP/1.1', BUFFER)
            #A hostname -> IP
            if REQUEST:
                ADDR = socket.gethostbyname(GET)
                BUFFER = 'HTTP/1.1 200 OK\n' + GET + ":A=" + ADDR + "\n"
                conn.sendall(BUFFER.encode())
            #PTR IP -> hostname
            else:
                NAME = socket.gethostbyaddr(GET)
                BUFFER = 'HTTP/1.1 200 OK\n' + GET + ":PTR=" + NAME[0] + "\n"
                conn.sendall(BUFFER.encode())
        else:
            REQUEST = re.search("POST", BUFFER)
            if REQUEST:
                print("BUFFER:",BUFFER)
                #get all requests
                BUFFER = re.findall('.*:(?:A|PTR)$', BUFFER, flags=re.MULTILINE)
                HOSTNAMES = ""
                IPADDR = ""
                for i in BUFFER:
                    mezikod = re.search('.*:PTR$', i, flags=re.MULTILINE)
                    mezikod2 = re.search('.*:A$', i, flags=re.MULTILINE)
                    if mezikod:
                        mezikod = mezikod.string
                        HOSTNAMES = HOSTNAMES + "\n" + mezikod
                    if mezikod2:
                        mezikod2 = mezikod2.string
                        IPADDR = IPADDR + "\n" + mezikod2
                    print("HOSTNAMES:", HOSTNAMES)
                    print("IP:", IPADDR)
                print("BUFFER:",BUFFER)
                print("POST")
            else:
                print("NOTHING")
    conn.close()
