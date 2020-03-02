#!/usr/bin/env python3

import getopt
import sys
import re

help_str = '--help - this help menu\n--source=file - source file with XML source code of IPPCODE20\n--input=file - file with described inputs\n*****************************************************************\n Additional info: At least 1 of input/source needs to be described, if only 1 is described, then stdin is used as input for second not described option\n'

XML_source = ''
inputs = ''

def main():
    #GET options from args
    try:
        options, values = getopt.getopt(sys.argv[1:],'',['help', 'source=', 'input='])
    except getopt.GetoptError as error:
        print(error)
        exit(10)

    #work based on arguments
    for opt, arg in options:
        if opt in '--help':
            print(help_str)
            close_them()
            exit(0)
        elif opt in '--source':
            try:
                XML_source = open(arg, "r")
            except:
                print("unexpected problem with opening file:", arg)
                close_them()
                exit(11)
        elif opt == '--input':
            try:
                inputs = open(arg, "r")
            except:
                print("unexpected problem with opening file:", arg)
                close_them()
                exit(11)
        else:
            print("bad argument")
            close_them()
            exit(10)
    
    if XML_source:
        #inputs na stdin
        if not inputs:
            print("not inputs")
        else:
            print("everything")
    #XML_source on stdin
    else:
        print("not XML_source")
        

def close_them():
    if XML_source:
        close(XML_source)
        print("closed XML")
    if inputs:
        close(inputs)
        print("closed inputs")

main()
