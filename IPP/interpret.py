#!/usr/bin/env python3

import getopt
import sys
import re
import xml.etree.ElementTree as ET


help_str = '--help - this help menu\n--source=file - source file with XML source code of IPPCODE20\n--input=file - file with described inputs\n*****************************************************************\n Additional info: At least 1 of input/source needs to be described, if only 1 is described, then stdin is used as input for second not described option\n'

XML_source = ''
inputs = ''
global_frame = []
local_frame = []
temporary_frame = ''



def main():
    global XML_source
    global inputs
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
            #check, that xml file can be opened
            try:
                f = open(arg)
            except:
                print("file:", arg,"doesnt exists, or cant be opened")
                exit(11)
            finally:
                f.close()
            XML_source = arg;
        elif opt == '--input':
            try:
                f = open(arg, "r")
                inputs = arg
            except:
                print("unexpected problem with opening file:", arg)
                exit(11)
            finally:
                f.close
        else:
            print("bad argument")
            close_them()
            exit(10)

    #
    if not XML_source:
        if not inputs:
            print("--source or --input must be specified")
            exit(10)
        else:
            XML_source = sys.stdin
    if not inputs:
        inputs = sys.stdin

    #XML_parse
    try:
        tree = ET.parse(XML_source)
        root = tree.getroot()
    except:
        print("problem with xml file structure")
        exit()
    #<program>
    if root.tag != 'program':
        print("element program missing, got:", root.tag)
        exit(32)
    my_instructions = []
    for child in root:
        #<instruction>
        if not child.tag or child.tag != "instruction":
            print("missing element instruction")
            exit(32)
        try:
            order = child.attrib["order"]
        except:
            print("missing attribute order")
            exit(32)
        try:
            opcode = child.attrib["opcode"]
        except:
            print("missing attribute opcode")
            exit(32)
        type1 = ''
        arg1 = ''
        type2 = ''
        arg2 = ''
        type3 = ''
        arg3 = ''
        #<arg_>
        for child2 in child:
            #arg1
            if child2.tag == "arg1":
                try:
                    type1 = child2.attrib["type"]
                except:
                    print("arg1 missing attribute type")
                    exit(32)
                try:
                    arg1 = child2.text
                except:
                    print("arg1 missing attribute type")
                    exit(32)
            #arg2
            elif child2.tag == "arg2":
                try:
                    type2 = child2.attrib["type"]
                except:
                    print("arg2 missing attribute type")
                    exit(32)
                try:
                    arg2 = child2.text
                except:
                    print("arg2 missing attribute type")
                    exit(32)
            #arg3
            elif child2.tag == "arg3":
                try:
                    type3 = child2.attrib["type"]
                except:
                    print("arg3 missing attribute type")
                    exit(32)
                try:
                    arg3 = child2.text
                except:
                    print("arg3 missing attribute type")
                    exit(32)
            #wrong attribute
            else:
                print("wrong attribute:", child.tag)
                exit(32)
        try:
            my_instructions.append(instrukce(order, opcode, type1, arg1, type2, arg2, type3, arg3))
        except:
            print("error when tried to append array of instructions, instruction:", opcode)
            exit(99)
    #get count of instructions in array
    count = len(my_instructions)
    #go through all instructions in array
    while count > 0:
        cnt = 1
        min_instr = 0
        #search for lowest order value in array
        while cnt < count:
            if my_instructions[cnt].order < my_instructions[min_instr].order:
                min_instr = cnt
            cnt += 1
        instr = my_instructions[min_instr]
        print(instr.order)
        #remove used instruction from array and decrease count
        my_instructions.remove(instr)
        count -= 1
    
class instrukce:
    def __init__(self, order, opcode, type1, arg1, type2, arg2, type3, arg3):
        self.order = order
        self.opcode = opcode
        self.type1 = type1
        self.arg1 = arg1
        self.type2 = type2
        self.arg2 = arg2
        self.type3 = type3
        self.arg3 = arg3

def instr_exec(instr):
    if instr.opcode == "MOVE":
        if is_symb(instr.type1) != 1:
            exit(32)
        elif is_frame(instr.arg1)
    elif instr.opcode == "CREATEFRAME":

    elif instr.opcode == "PUSHFRAME":

    elif instr.opcode == "POPFRAME":

    elif instr.opcode == "DEFVAR":

    elif instr.opcode == "CALL":

    elif instr.opcode == "RETURN":

    elif instr.opcode == "PUSHS":

    elif instr.opcode == "POPS":

    elif instr.opcode == "ADD":

    elif instr.opcode == "SUB":

    elif instr.opcode == "MUL":

    elif instr.opcode == "IDIV":

    elif instr.opcode == "LT" or instr.opcode == "GT" or instr.opcode == "EQ":

    elif instr.opcode == "AND" or instr.opcode == "OR" or instr.opcode == "NOT":

    elif instr.opcode == "INT2CHAR":

    elif instr.opcode == "STRI2INT":

    elif instr.opcode == "READ":

    elif instr.opcode == "WRITE":

    elif instr.opcode == "CONCAT":

    elif instr.opcode == "STRLEN":

    elif instr.opcode == "GETCHAR":

    elif instr.opcode == "SETCHAR":

    elif instr.opcode == "TYPE":

    elif instr.opcode == "LABEL":

    elif instr.opcode == "JUMP":

    elif instr.opcode == "JUMPIFEQ":

    elif instr.opcode == "JUMPIFNEQ":

    elif instr.opcode == "EXIT":

    elif instr.opcode == "DPRINT":

    elif instr.opcode == "BREAK":

#is par symb ? @ret if symb 0, 1 if var, else 2
def is_symb(par):
    if par == "int" or par == "bool" or par == "string" or par == "nil":
        return 0
    if par == "var":
        return 1
    return 2

#is par frame ? @ret if global 0, if local 1, if temp 2, else 3
def is_frame(par):
    frame = re.match( '^gf@', par)
    if not frame:
        frame = re.match( '^lf@', par)
        if not frame:
            frame = re.match( '^tf@', par)
            if not frame:
                return 3
            return 2
        return 1
    return 0
#Does frame exist ? @ret 0 if global, 1 if local, 2 if temporary, else 3
def frame_exist(par):

    global global_frame
    global local_frame
    global temporary_frame


    frame = is_frame(par)
    if frame == 3:
        return 3
    elif frame == 0:
        delka = len(global_frame)
        cnt = 0
        while cnt < delka:
            if global_frame[cnt] == par:
                return 0
            cnt += 1
    elif frame == 1:
        cnt = 0
        delka = len(local_frame)
        while cnt < delka:
            if local_frame[cnt] == par:
                return 1
            cnt += 1
    elif frame == 2:
        if par == temporary_frame:
            return 2

def close_them():
    if inputs:
        close(inputs)
        print("closed inputs")

main()
