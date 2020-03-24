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
        self.opcode = opcode.upper()
        self.type1 = type1.lower()
        self.arg1 = arg1
        self.type2 = type2.lower()
        self.arg2 = arg2
        self.type3 = type3.lower()
        self.arg3 = arg3

class promenna:
    value = ''
    val_type = ''
    def __init__(self, arg):
        self.name = arg.split('@')[1]

class zas_item:
    def __init__(self, value, value_type):
        self.value = value
        self.val_type = value_type

#ret 0 if ok, else 1
def instr_exec(instr):

    global local_frame
    global temporary_frame
    global global_frame

    if instr.opcode == "MOVE":
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
            index = find_index("gf", instr.arg1.split('@')[1])
            # MOVE GF var
            if blabla == 1:
                var_type2 = var_exist(instr.arg2)
                #must exist
                if var_type2 == 3:
                    exit(54)
                index2 = find_index(instr.arg2.split('@')[0],instr.arg2.split('@')[1])
                # MOVE GF GF
                if var_type2 == 0:
                    global_frame.top()[index].value = global_frame[index2].value
                    global_frame[index].val_type = global_frame[index2].val_type
                # MOVE GF LF
                elif var_type2 == 1:
                    global_frame[index].value = local_frame.top()[index2].value
                    global_frame[index].val_type = local_frame.top()[index2].val_type
                # MOVE GF TF
                elif var_type2 == 2:
                    global_frame[index].value = temporary_frame[index2].value
                    global_frame[index].val_type = temporary_frame[index2].val_type
                else:
                    exit(99)
            #MOVE GF konst
            elif blabla == 0:
                global_frame[index].value = instr.arg2
                global_frame[index].val_type = instr.type2



        # MOVE LF
        elif var_type == 1:
            index = find_index("lf", instr.arg1.split('@')[1])
            # MOVE LF var
            if blabla == 1:
                var_type2 = var_exist(instr.arg2)
                #must exist
                if var_type2 == 3:
                    exit(54)
                index2 = find_index(instr.arg2.split('@')[0],instr.arg2.split('@')[1])
                # MOVE LF GF
                if var_type2 == 0:
                    local_frame.top()[index].value = global_frame[index2].value
                    local_frame.top()[index].val_type = global_frame[index2].val_type
                # MOVE LF LF
                elif var_type2 == 1:
                    local_frame.top()[index].value = local_frame.top()[index2].value
                    local_frame.top()[index].val_type = local_frame.top()[index2].val_type
                # MOVE LF TF
                elif var_type2 == 2:
                    local_frame.top()[index].value = temporary_frame[index2].value
                    local_frame.top()[index].val_type = temporary_frame[index2].val_type
                else:
                    exit(99)
            # MOVE LF konst
            elif blabla == 0:
                local_frame.top()[index].value = instr.arg2
                local_frame.top()[index].val_type = instr.type2

        # MOVE TF
        elif var_type == 2:
            index = find_index("tf", instr.arg1.split('@')[1])
            # MOVE TF var
            if blabla == 1:
                var_type2 = var_exist(instr.arg2)
                #must exist
                if var_type2 == 3:
                    exit(54)
                index2 = find_index(instr.arg2.split('@')[0],instr.arg2.split('@')[1])
                # MOVE TF GF
                if var_type2 == 0:
                    temporary_frame[index].value = global_frame[index2].value
                    temporary_frame[index].val_type = global_frame[index2].val_type
                # MOVE TF LF
                elif var_type2 == 1:
                    temporary_frame[index].value = local_frame.top()[index2].value
                    temporary_frame[index].val_type = local_frame.top()[index2].val_type
                # MOVE TF TF
                elif var_type2 == 2:
                    temporary_frame[index].value = temporary_frame[index2].value
                    temporary_frame[index].val_type = temporary_frame[index2].val_type
                else:
                    exit(99)
            # MOVE TF konst
            elif blabla == 0:
                temporary_frame[index].value = instr.arg2
                temporary_frame[index].val_type = instr.type2
        else:
            exit(99)
        
    elif instr.opcode == "CREATEFRAME":
        temporary_frame.clear

    elif instr.opcode == "PUSHFRAME":
        delka = len(temporary_frame)
        #not defined temp_frame
        if delka <= 0:
            exit(55)
        local_frame.push(temporary_frame)
        temporary_frame.clear()

    elif instr.opcode == "POPFRAME":
        if len(local_frame) <= 0:
            exit(55)
        temporary_frame = local_frame.pop()

    elif instr.opcode == "DEFVAR":
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
            local_frame.top().append(promenna(instr.arg1))
        #tf
        elif frame == 2:
            temporary_frame.append(promenna(instr.arg1))
    #elif instr.opcode == "CALL":

    #elif instr.opcode == "RETURN":

    elif instr.opcode == "PUSHS":
        typ = is_symb(instr.type1)
        #var
        if typ == 1:
            index = find_index(instr.arg1.split('@')[0], instr.arg1.split('@')[1])
            var_typ = var_exist(instr.arg1)
            #PUSHS GF
            if var_typ == 0:
                var = global_frame[index]
                stack.push(zas_item(var.value, var.val_type))
            #PUSHS LF
            elif var_typ == 1:
                var = local_frame.top()[index]
                stack.push(zas_item(var.value, var.val_type))
            #PUSHS TF
            elif var_typ == 2:
                var = temporary_frame[index]
                stack.push(zas_item(var.value, var.val_type))
        elif typ == 0:
            stack.push(zas_item(instr.arg1,instr.type1))
        else:
            exit(99)

    elif instr.opcode == "POPS":
        typ = is_symb(instr.type1)
        #var
        if typ == 1:
            #stack empty
            if len(stack) <= 0:
                exit(56)
            index = find_index(instr.arg1.split('@')[0], instr.arg1.split('@')[1])
            var_typ = var_exist(instr.arg1)
            item = stack.pop
            #POPS GF
            if var_typ == 0:
                global_frame[index].value = item.value
                global_frame[index].val = item.val_type
            #POPS LF
            elif var_typ == 1:
                local_frame.top()[index].value = item.value
                local_frame.top()[index].val_type = item.val_type
            #POPS TF
            elif var_typ == 2:
                temporary_frame[index].value = item.value
                temporary_frame[index].val_type = item.val_type
        else:
            exit(32)

    elif instr.opcode == "ADD":
        symb = is_symb(instr.type1)
        symb2 = is_symb(instr.type2)
        symb3 = is_symb(instr.type3)
        index2 = ''
        index3 = ''
        var2 = ''
        var3 = ''
        #not var
        if symb != 1:
            exit(32)
        if symb2 == 2:
            exit(99)
        if symb3 == 2:
            exit(99)
        if symb2 == 1:
            var2 = get_var(instr.arg2)
        elif symb2 == 0:
            var2 = promenna("temp@temp")
            var2.value = instr.arg2
            var2.val_type = instr.type2
        if symb3 == 1:
            var3 = get_var(instr.arg3)
        elif symb3 == 0:
            var3 = promenna("temp@temp")
            var3.value = instr.arg3
            var3.val_type = instr.type3
        if var2.val_type != "int":
            exit(53)
        elif var3.val_type != "int":
            exit(53)
        index1 = find_index(instr.arg1.split('@')[0], instr.arg1.split('@')[1])
        var1 = var_exist(instr.arg1)
        #ADD GF
        if var1 == 0:
            global_frame[index].value = var2.value + var3.value
            global_frame[index].val_type = "int"
        #ADD LF
        elif var1 == 1:
            local_frame.top()[index].value = var2.value + var3.value
            local_frame.top()[index].val_type = "int"
        #ADD TF
        elif var1 == 2:
            temporary_frame[index].value = var2.value + var3.value
            temporary_frame[index].val_type = "int"
            


    #elif instr.opcode == "SUB":

    #elif instr.opcode == "MUL":

    #elif instr.opcode == "IDIV":

    #elif instr.opcode == "LT" or instr.opcode.upper == "GT" or instr.opcode.upper == "EQ":

    #elif instr.opcode == "AND" or instr.opcode.upper == "OR" or instr.opcode.upper == "NOT":

    #elif instr.opcode == "INT2CHAR":

    #elif instr.opcode == "STRI2INT":

    #elif instr.opcode == "READ":

    elif instr.opcode == "WRITE":
        value = is_symb(instr.type1)
        #var
        if value == 1:
            var_type = var_exist(instr.arg1)
            index = find_index(instr.arg1.split('@')[0],instr.arg1.split('@')[1])
            #var in GF
            if var_type == 0:
                var = global_frame[index]
                if var.val_type == "bool":
                    print("bool TODO")
                elif var.val_type == "nil":
                    print("nil TODO")
                else:
                    print(var.value)
            #var in LF
            if var_type == 1:
                var = local_frame.top()[index]
                if var.val_type == "bool":
                    print("bool TODO")
                elif var.val_type == "nil":
                    print("nil TODO")
                else:
                    print(var.value)
            #var in TF
            if var_type == 2:
                var = temporary_frame[index]
                if var.val_type == "bool":
                    print("bool TODO")
                elif var.val_type == "nil":
                    print("nil TODO")
                else:
                    print(var.value)
        elif value == 0:
            kon_type = instr.type1
            if  kon_type == "bool":
                print("bool TODO")
            elif kon_type == "nil":
                print("nil TODO")
            else:
                print(instr.arg1)
    #elif instr.opcode == "CONCAT":

    #elif instr.opcode == "STRLEN":

    #elif instr.opcode == "GETCHAR":

    #elif instr.opcode == "SETCHAR":

    #elif instr.opcode == "TYPE":

    #elif instr.opcode == "LABEL":

    #elif instr.opcode == "JUMP":

    #elif instr.opcode == "JUMPIFEQ":

    #elif instr.opcode == "JUMPIFNEQ":

    #elif instr.opcode == "EXIT":

    #elif instr.opcode == "DPRINT":

    #elif instr.opcode == "BREAK":

    else:
        exit(32)

#return 
def get_var(name):

    global global_frame
    global local_frame
    global temporary_frame


    frame = name.split('@')[0]
    nazev = name.split('@')[1]
    var_ex = var_exist(name)
    index = find_index(frame, nazev)
    if var_ex == 0:
        return global_frame[index]
    elif var_ex == 1:
        return local_frame.top()[index]
    elif var_ex == 2:
        return temporary_frame[index]
    else:
        exit(99)

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

#return index of variable in frame, else return -1
def find_index(frame, name):
    global global_frame
    global local_frame
    global temporary_frame
    
    frame = frame.lower()
    cnt = 0
    if frame == "gf":
        delka = len(global_frame)
        while cnt < delka:
            if global_frame[cnt].name == name:
                return cnt
            cnt += 1
    elif frame == "lf":
        delka = len(local_frame.top())
        while cnt < delka:
            if local_frame.top()[cnt].name == name:
                return cnt
            cnt += 1
    elif frame == "tf":
        delka = len(temporary_frame)
        while cnt < delka:
            if temporary_frame[cnt].name == name:
                return cnt
            cnt += 1
    
    exit(54)


#Does var exist ? @ret 0 if global, 1 if local, 2 if temporary, 3 if doesnt exist, else 4
def var_exist(par):

    global global_frame
    global local_frame
    global temporary_frame
    cnt = 0
    frame = is_frame(par)
    name = par.split('@')[1]
    if frame == 3:
        return 3
    elif frame == 0:
        delka = len(global_frame)
        while cnt < delka:
            if global_frame[cnt] == par:
                return 0
            cnt += 1
    elif frame == 1:
        delka = len(local_frame.top())
        while cnt < delka:
            if local_frame.top()[cnt] == par:
                return 1
            cnt += 1
    elif frame == 2:
        delka = len(temporary_frame)
        while cnt < delka:
            if temporary_frame[cnt].name == name:
                return 2
            cnt += 1
    return 3
def close_them():
    if inputs:
        close(inputs)
        print("closed inputs")

main()
