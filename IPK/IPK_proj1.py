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
clientSocket.listen()
while True:
    conn, addr = clientSocket.accept()
    #create new connection with client addr on socket conn
    BUFFER = conn.recv(SIZE).decode()
    if not BUFFER:
        conn.sendall(b'HTTP/1.1 500 Internal Server Error\r\n')
    else:
        REQUEST = re.search("GET", BUFFER)
        #GET
        if REQUEST:
            TEST = re.search("/resolve", BUFFER)
            if not TEST:
                conn.sendall(b'HTTP/1.1 400 Bad Request\r\n')
            else:
                GET = re.findall('name=.*&', BUFFER)
                if GET:
                    GET = GET[0][5:-1]
                    REQUEST = re.findall('type=A HTTP/1.1', BUFFER)
                    #A hostname -> IP
                    if REQUEST:
                        try:
                            ADDR = socket.getaddrinfo(GET, ServerPort)
                            ADDR = ADDR[0][4]
                            ADDR = GET + ":A=" + ADDR[0] + "\r\n\r\n"
                            BUFFER = 'HTTP/1.1 200 OK\r\n\r\n' + ADDR
                            conn.sendall(BUFFER.encode())
                        except:
                            conn.sendall(b'HTTP/1.1 500 Internal Server Error\r\n')
                    #PTR IP -> hostname
                    else:
                        REQUEST = re.findall('type=PTR HTTP/1.1', BUFFER)
                        if REQUEST:
                            sockAdr = (GET, 443)
                            try:
                                NAME = socket.getnameinfo(sockAdr, socket.NI_NOFQDN)[0]
                                NAME = GET + ":PTR=" + NAME + "\r\n\r\n"
                                BUFFER = 'HTTP/1.1 200 OK\r\n\r\n' + NAME
                                conn.sendall(BUFFER.encode())
                            except:
                                conn.sendall(b'HTTP/1.1 500 Internal Server Error\r\n')
                        else:
                            print("type not A or PTR")
                            conn.sendall(b'HTTP/1.1 400 Bad Request\r\n')
                else:
                    print("bad arg")
                    conn.sendall(b'HTTP/1.1 400 Bad Request\r\n')
        #POST or UNKNOWN
        else:
            REQUEST = re.search("POST", BUFFER)
            if REQUEST:
                TEST = re.search("/dns-query", BUFFER)
                if not TEST:
                    print("bad URL")
                    conn.sendall(b'HTTP/1.1 400 Bad Request\r\n')
                else:
                    #get all requests
                    BUFFER = re.findall('.*:(?:A|PTR)$', BUFFER, flags=re.MULTILINE)
                    if BUFFER:
                        Output = ""
                        for i in BUFFER:
                            mezikod = re.search('.*:PTR$', i, flags=re.MULTILINE)
                            mezikod2 = re.search('.*:A$', i, flags=re.MULTILINE)
                            try:
                                mezikod = mezikod.string
                                something = mezikod[:-4]
                                sockAdr = (something, 443)
                                neco = socket.getnameinfo(sockAdr, socket.NI_NOFQDN)[0]
                                if not neco:
                                    print("error")
                                    conn.sendall(b'HTTP/1.1 500 Internal Server Error\r\n')
                                Output += mezikod + "=" + neco + "\r\n\r\n"
                            except:    
                                Output += ""
                            try:
                                mezikod2 = mezikod2.string
                                something = mezikod2[:-2]
                                neco = socket.getaddrinfo(something, ServerPort)
                                if not neco:
                                    print("fail")
                                neco = neco[0][4]
                                neco = neco[0]
                                if not neco:
                                    print("error")
                                    conn.sendall(b'HTTP/1.1 500 Internal Server Error\r\n')
                                Output += mezikod2 + "=" + neco + "\r\n\r\n"
                            except:
                                Output += ""
                        BUFFER = 'HTTP/1.1 200 OK\r\n\r\n' + Output
                        conn.sendall(BUFFER.encode())
                    else:
                        print("bad post arg")
                        conn.sendall(b'HTTP/1.1 400 Bad Request\r\n')
            #not GET or POST
            else:
                print("unknown method")
                conn.send(b'HTTP/1.1 405 Method Not Allowed\r\n')
    conn.close()
