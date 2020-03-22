#!/usr/bin/env python3

import getopt
import sys
import re
import xml.etree.ElementTree as ET


help_str = '--help - this help menu\n--source=file - source file with XML source code of IPPCODE20\n--input=file - file with described inputs\n*****************************************************************\n Additional info: At least 1 of input/source needs to be described, if only 1 is described, then stdin is used as input for second not described option\n'

XML_source = ''
inputs = ''
global_frame = []
lf_cnt = -1
local_frame = []
temporary_frame = []
stack = []


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
        print(instr.opcode)

        #exec
        instr_exec(instr)


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

class promenna:
    def __init__(self, arg):
        self.name = arg.split('@')[1]
    def change_val(self, value):
        self.value = value
    def change_type(self, val_type):
        self.val_type = val_type

#ret 0 if ok, else 1
def instr_exec(instr):

    global local_frame
    global temporary_frame
    global global_frame
    global lf_cnt

    print(instr.opcode)
    if instr.opcode.upper == "MOVE":
        #type of arg2
        blabla = is_symb(instr.type2)
        
        #var
        if is_symb(instr.type1) != 1:
            exit(32)
        elif blabla == 2:
            exit(32)
        var_type = var_exist(instr.arg1)
        #variable must exist
        if var_type == 3:
            exit(54)
        # MOVE GF
        if var_type == 0:
            # MOVE GF var
            if blabla == 1:
                var_type2 = var_exist(instr.arg2)
                #must exist
                if var_type2 == 3:
                    exit(54)
                # MOVE GF GF
                if var_type2 == 0:
                    print("neco")
                # MOVE GF LF
                if var_type2 == 1:
                    print("neco")
                # MOVE GF TF
                if var_type2 == 2:
                    print("neco")


        # MOVE LF
        elif var_type == 1:
            # MOVE LF var
            if blabla == 1:
                var_type2 = var_exist(instr.arg2)
                #must exist
                if var_type2 == 3:
                    exit(54)
                # MOVE LF GF
                if var_type2 == 0:
                    print("neco")
                # MOVE LF LF
                if var_type2 == 1:
                    print("neco")
                # MOVE LF TF
                if var_type2 == 2:
                    print("neco")

        # MOVE TF
        elif var_type == 2:
            # MOVE TF var
            if blabla == 1:
                var_type2 = var_exist(instr.arg2)
                #must exist
                if var_type2 == 3:
                    exit(54)
                # MOVE TF GF
                if var_type2 == 0:
                    print("neco")
                # MOVE TF LF
                if var_type2 == 1:
                    print("neco")
                # MOVE TF TF
                if var_type2 == 2:
                    print("neco")

        
    elif instr.opcode.upper() == "CREATEFRAME":
        temporary_frame.clear

    elif instr.opcode.upper == "PUSHFRAME":
        lf_cnt += 1
        local_frame[lf_cnt].append(temporary_frame)
        temporary_frame.clear()

    elif instr.opcode.upper == "POPFRAME":
        temporary_frame = local_frame[lf_cnt]
        if lf_cnt < 0:
            exit(55)
        local_frame[lf_cnt].clear()
        lf_cnt -= 1

    elif instr.opcode.upper == "DEFVAR":
        #var
        if is_symb(instr.type1) != 1:
            exit(32)
        #redefinition of var
        if var_exist(instr.arg1) != 3:
            exit(52)
        frame = is_frame(instr.arg1)
        #gf
        if frame == 0:
            global_frame.append(promenna(instr.arg1))
        #lf
        elif frame == 1:
            local_frame[lf_cnt].append(promenna(instr.arg1))
        #tf
        elif frame == 2:
            temporary_frame.append(promenna(instr.arg1))

    #elif instr.opcode.upper == "CALL":

    #elif instr.opcode.upper == "RETURN":

    #elif instr.opcode.upper == "PUSHS":

    #elif instr.opcode.upper == "POPS":

    #elif instr.opcode.upper == "ADD":

    #elif instr.opcode.upper == "SUB":

    #elif instr.opcode.upper == "MUL":

    #elif instr.opcode.upper == "IDIV":

    #elif instr.opcode.upper == "LT" or instr.opcode.upper == "GT" or instr.opcode.upper == "EQ":

    #elif instr.opcode.upper == "AND" or instr.opcode.upper == "OR" or instr.opcode.upper == "NOT":

    #elif instr.opcode.upper == "INT2CHAR":

    #elif instr.opcode.upper == "STRI2INT":

    #elif instr.opcode.upper == "READ":

    #elif instr.opcode.upper == "WRITE":

    #elif instr.opcode.upper == "CONCAT":

    #elif instr.opcode.upper == "STRLEN":

    #elif instr.opcode.upper == "GETCHAR":

    #elif instr.opcode.upper == "SETCHAR":

    #elif instr.opcode.upper == "TYPE":

    #elif instr.opcode.upper == "LABEL":

    #elif instr.opcode.upper == "JUMP":

    #elif instr.opcode.upper == "JUMPIFEQ":

    #elif instr.opcode.upper == "JUMPIFNEQ":

    #elif instr.opcode.upper == "EXIT":

    #elif instr.opcode.upper == "DPRINT":

    #elif instr.opcode.upper == "BREAK":

    else:
        return 1

#is par symb ? @ret if symb 0, 1 if var, else 2
def is_symb(par):
    if par == "int" or par == "bool" or par == "string" or par == "nil":
        return 0
    if par == "var":
        return 1
    return 2

#is par frame ? @ret if global 0, if local 1, if temp 2, else 3
def is_frame(par):
    #for case insensitive
    par = par.lower()
    frame = par.split('@')[0]
    if frame.lower() != "gf":
        if frame.lower() != "lf":
            if frame.lower() != "tf":
                return 3
            return 2
        return 1
    return 0


#Does var exist ? @ret 0 if global, 1 if local, 2 if temporary, 3 if doesnt exist, else 4
def var_exist(par):

    global global_frame
    global local_frame
    global temporary_frame
    global lf_cnt

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
        delka = len(local_frame[lf_cnt])
        while cnt < delka:
            if local_frame[lf_cnt][cnt] == par:
                return 1
            cnt += 1
    elif frame == 2:
        if par == temporary_frame:
            return 2
    return 4
def close_them():
    if inputs:
        close(inputs)
        print("closed inputs")

main()
