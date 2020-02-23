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
        print("got nothing")
        conn.sendall(b'HTTP/1.1 500 Internal Server Error\n')
    else:
        REQUEST = re.search("GET", BUFFER)
        #GET
        if REQUEST:
            TEST = re.search("/resolve", BUFFER)
            if not TEST:
                print("bad URL")
                conn.sendall(b'HTTP/1.1 400 Bad Request\n')
            else:
                GET = re.findall('name=.*&', BUFFER)
                if GET:
                    GET = GET[0][5:-1]
                    REQUEST = re.findall('type=A HTTP/1.1', BUFFER)
                    #A hostname -> IP
                    if REQUEST:
                        ADDR = socket.gethostbyname(GET)
                        BUFFER = 'HTTP/1.1 200 OK\n' + GET + ":A=" + ADDR + "\n"
                        conn.sendall(BUFFER.encode())
                    #PTR IP -> hostname
                    else:
                        REQUEST = re.findall('type=PTR HTTP/1.1', BUFFER)
                        if REQUEST:
                            NAME = socket.gethostbyaddr(GET)
                            BUFFER = 'HTTP/1.1 200 OK\n' + GET + ":PTR=" + NAME[0] + "\n"
                            conn.sendall(BUFFER.encode())
                        else:
                            print("type not A or PTR")
                            conn.sendall(b'HTTP/1.1 400 Bad Request\n')
                else:
                    print("bad arg")
                    conn.sendall(b'HTTP/1.1 400 Bad Request\n')
        #POST or UNKNOWN
        else:
            REQUEST = re.search("POST", BUFFER)
            if REQUEST:
                TEST = re.search("/dns-query", BUFFER)
                if not TEST:
                    print("bad URL")
                    conn.sendall(b'HTTP/1.1 400 Bad Request\n')
                else:
                    #get all requests
                    BUFFER = re.findall('.*:(?:A|PTR)$', BUFFER, flags=re.MULTILINE)
                    if BUFFER:
                        Output = ""
                        for i in BUFFER:
                            mezikod = re.search('.*:PTR$', i, flags=re.MULTILINE)
                            mezikod2 = re.search('.*:A$', i, flags=re.MULTILINE)
                            if mezikod:
                                mezikod = mezikod.string
                                something = mezikod[:-4]
                                neco = socket.gethostbyaddr(something)
                                if not neco:
                                    print("error")
                                    conn.sendall(b'HTTP/1.1 500 Internal Server Error\n')
                                Output += mezikod + "=" + neco[0] + "\n"
                            if mezikod2:
                                mezikod2 = mezikod2.string
                                something = mezikod2[:-2]
                                neco = socket.gethostbyname(something)
                                if not neco:
                                    print("error")
                                    conn.sendall(b'HTTP/1.1 500 Internal Server Error\n')
                                Output += mezikod2 + "=" + neco + "\n"
                        print(Output)
                        BUFFER = 'HTTP/1.1 200 OK\n' + Output
                        conn.sendall(BUFFER.encode())
                    else:
                        print("bad post arg")
                        conn.sendall(b'HTTP/1.1 400 Bad Request\n')
            #not GET or POST
            else:
                print("unknown method")
                conn.send(b'HTTP/1.1 405 Method Not Allowed\n')
    conn.close()
