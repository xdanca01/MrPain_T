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
temporary_frame = None
stack = []
instr_list = ""
my_instructions = ""
call_cnt = 0


def main():
    global XML_source
    global inputs
    global my_instructions
    global instr_list
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
                f.close()
            except:
                print("file:", arg,"doesnt exists, or cant be opened")
                exit(11)
            XML_source = arg
        elif opt == '--input':
            try:
                inputs = open(arg, "r")
            except:
                print("unexpected problem with opening file:", arg)
                exit(11)
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
        exit(31)
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
        vzt = re.match('^\+*[0-9]+$', order)
        if not vzt:
            print("wrong order type")
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
                if not arg1:
                    arg1 = ""
                check_syn(type1, arg1)
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
                if not arg2:
                    arg2 = ""
                check_syn(type2, arg2)
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
                if not arg3:
                    arg3 = ""
                check_syn(type3, arg3)

            #wrong attribute
            else:
                print("wrong attribute:", child.tag)
                exit(32)

        try:
            my_instructions.append(instrukce(int(order), opcode, type1, arg1, type2, arg2, type3, arg3))
        except:
            print("error when tried to append array of instructions, instruction:", opcode)
            exit(32)
    instr_list = my_instructions.copy()
    #get count of instructions in array
    count = len(my_instructions)

    #check that order isnt same on 2 places
    while count > 0: 
        cnt = 1
        min_instr = 0
        #search for lowest order value in array
        while cnt < count:
            if int(my_instructions[cnt].order) == int(my_instructions[min_instr].order):
                exit(32)
            if int(my_instructions[cnt].order) < int(my_instructions[min_instr].order):
                min_instr = cnt
            if my_instructions[cnt].opcode == my_instructions[min_instr].opcode and my_instructions[cnt].opcode == "LABEL":
                if my_instructions[cnt].arg1 == my_instructions[min_instr].arg1:
                    exit(52)
            cnt += 1
        instr = my_instructions[min_instr]
        if instr.order <= 0:
            exit(32)
        #remove used instruction from array and decrease count
        my_instructions.remove(instr)
        count -= 1

    my_instructions = instr_list.copy()
    count = len(my_instructions)
    #go through all instructions in array
    while count > 0:
        cnt = 1
        min_instr = 0
        #search for lowest order value in array
        while cnt < count:
            if int(my_instructions[cnt].order) < int(my_instructions[min_instr].order):
                min_instr = cnt
            cnt += 1
        instr = my_instructions[min_instr]
        #exec
        ret_val = instr_exec(instr)
        if ret_val:
            #jump to higher order (need to remove instr in list)
            if int(instr.order) < int(ret_val):
                cnt = 0
                my_count = len(my_instructions) - 1
                while cnt <= my_count:
                    if int(my_instructions[my_count].order) < ret_val:
                        my_instructions.remove(my_instructions[my_count])
                        count -= 1
                    my_count -= 1

            #jump to lower order (need to append list by instructions
            elif int(my_instructions[min_instr].order) > int(ret_val):
                cnt = 0
                my_list = instr_list.copy()
                my_count = len(my_list)
                while cnt < my_count:
                    if int(my_list[cnt].order) < int(instr.order) and int(my_list[cnt].order) > ret_val:
                        my_instructions.append(my_list[cnt])
                        count += 1
                    cnt += 1
            continue

        #remove used instruction from array and decrease count
        my_instructions.remove(instr)
        count = len(my_instructions)
    
class instrukce:
    def __init__(self, order, opcode, type1, arg1, type2, arg2, type3, arg3):
        opcode = opcode.upper()
        arg1 = re.sub("\\\\(\d+)", lambda x : chr(int(x.group(1))), arg1)
        arg2 = re.sub("\\\\(\d+)", lambda x : chr(int(x.group(1))), arg2)
        arg3 = re.sub("\\\\(\d+)", lambda x : chr(int(x.group(1))), arg3)
        self.order = order
        self.opcode = opcode
        self.type1 = type1
        self.arg1 = arg1
        self.type2 = type2
        self.arg2 = arg2
        self.type3 = type3
        self.arg3 = arg3
        count = 0
        if type1:
            count += 1
            if type2:
                count += 1
                if type3:
                    count += 1
        if opcode == "MOVE" and count == 2:
            return
        elif opcode == "CREATEFRAME" and count == 0:
            return
        elif opcode == "PUSHFRAME" and count == 0:
            return
        elif opcode == "POPFRAME" and count == 0:
            return
        elif opcode == "DEFVAR" and count == 1:
            return
        elif opcode == "CALL" and count == 1:
            return
        elif opcode == "RETURN" and count == 0:
            return
        elif opcode == "PUSHS" and count == 1:
            return
        elif opcode == "POPS" and count == 1:
            return
        elif opcode == "ADD" and count == 3:
            return
        elif opcode == "SUB" and count == 3:
            return
        elif opcode == "MUL" and count == 3:
            return
        elif opcode == "IDIV" and count == 3:
            return
        elif opcode == "LT" and count == 3:
            return
        elif opcode == "GT" and count == 3:
            return
        elif opcode == "EQ" and count == 3:
            return
        elif opcode == "AND" and count == 3:
            return
        elif opcode == "OR" and count == 3:
            return
        elif opcode == "NOT" and count == 2:
            return
        elif opcode == "INT2CHAR" and count == 2:
            return
        elif opcode == "STRI2INT" and count == 3:
            return
        elif opcode == "READ" and count == 2:
            return
        elif opcode == "WRITE" and count == 1:
            return
        elif opcode == "CONCAT" and count == 3:
            return
        elif opcode == "STRLEN" and count == 2:
            return
        elif opcode == "GETCHAR" and count == 3:
            return
        elif opcode == "SETCHAR" and count == 3:
            return
        elif opcode == "TYPE" and count == 2:
            return
        elif opcode == "LABEL" and count == 1:
            return
        elif opcode == "JUMP" and count == 1:
            return
        elif opcode == "JUMPIFEQ" and count == 3:
            return
        elif opcode == "JUMPIFNEQ" and count == 3:
            return
        elif opcode == "EXIT" and count == 1:
            return
        elif opcode == "DPRINT" and count == 1:
            return
        elif opcode == "BREAK" and count == 0:
            return
        else:
            exit(32)








class promenna:
    value = ''
    val_type = ''
    def __init__(self, arg):
        self.name = arg.split('@')[1]

class zas_item:
    def __init__(self, value, value_type):
        self.value = value
        self.val_type = value_type


def check_syn(arg_type, arg):
    arg_type = arg_type.lower()
    if not arg_type:
        exit(32)
    if not arg and (arg_type != "string" and arg_type != "nil"):
        exit(32)
    #int
    if arg_type == "int":
        match = re.match('^(-|\+)*[0-9]+$', arg)
        if not match:
            exit(32)
    #string
    elif arg_type == "string":
        arg
    #bool
    elif arg_type == "bool":
        if arg != "true" and arg != "false":
            exit(32)
    #var
    elif arg_type == "var":
        frame = arg.split('@')[0]
        if frame != "GF" and frame != "LF" and frame != "TF":
            exit(32)
        try:
            name = arg.split('@')[1]
        except:
            exit(32)

        match = re.match('^[0-9]', name)
        if match:
            exit(32)
        match = re.match(' ', name)
        if match:
            exit(32)
        if name == "":
            exit(32)
    #label
    elif arg_type == "label":
        match = re.match('^[0-9]', arg)
        if match:
            exit(32)
        match = re.match(' ', arg)
        if match:
            exit(32)
    #type
    elif arg_type == "type":
        if arg != "string" and arg != "int" and arg != "bool" and arg != "nil":
            exit(32)
    #nil
    elif arg_type == "nil":
        if arg != "nil":
            exit(32)
    else:
        exit(32)
    return 0

#ret 0 if ok, else 1
def instr_exec(instr):

    global local_frame
    global temporary_frame
    global global_frame
    global inputs
    global call_cnt
    lf_index = len(local_frame) - 1

    if instr.opcode == "MOVE":
        #type of arg2
        blabla = is_symb(instr.type2)
        
        #var
        if is_symb(instr.type1) != 1:
            exit(99)
        elif blabla == 2:
            exit(99)
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
                    var2 = global_frame[index2]
                    is_defined(var2)
                    global_frame[index].value = var2.value
                    global_frame[index].val_type = var2.val_type
                # MOVE GF LF
                elif var_type2 == 1:
                    var2 = local_frame[lf_index][index2]
                    is_defined(var2)
                    global_frame[index].value = var2.value
                    global_frame[index].val_type = var2.val_type
                # MOVE GF TF
                elif var_type2 == 2:
                    var2 = temporary_frame[index2]
                    is_defined(var2)
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
                    var2 = global_frame[index2]
                    is_defined(var2)
                    local_frame[lf_index][index].value = var2.value
                    local_frame[lf_index][index].val_type = var2.val_type
                # MOVE LF LF
                elif var_type2 == 1:
                    var2 = local_frame[lf_index][index2]
                    is_defined(var2)
                    local_frame[lf_index][index].value = var2.value
                    local_frame[lf_index][index].val_type = var2.val_type
                # MOVE LF TF
                elif var_type2 == 2:
                    var2 = temporary_frame[index2]
                    is_defined(var2)
                    local_frame[lf_index][index].value = var2.value
                    local_frame[lf_index][index].val_type = var2.val_type
                else:
                    exit(99)
            # MOVE LF konst
            elif blabla == 0:
                local_frame[lf_index][index].value = instr.arg2
                local_frame[lf_index][index].val_type = instr.type2

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
                    var2 = global_frame[index2]
                    is_defined(var2)
                    temporary_frame[index].value = var2.value
                    temporary_frame[index].val_type = var2.val_type
                # MOVE TF LF
                elif var_type2 == 1:
                    var2 = local_frame[lf_index][index2]
                    is_defined(var2)
                    temporary_frame[index].value = var2.value
                    temporary_frame[index].val_type = var2.val_type
                # MOVE TF TF
                elif var_type2 == 2:
                    var2 = temporary_frame[index2]
                    is_defined(var2)
                    temporary_frame[index].value = var2.value
                    temporary_frame[index].val_type = var2.val_type
                else:
                    exit(99)
            # MOVE TF konst
            elif blabla == 0:
                temporary_frame[index].value = instr.arg2
                temporary_frame[index].val_type = instr.type2
        else:
            exit(99)
        
    elif instr.opcode == "CREATEFRAME":
        temporary_frame = []

    elif instr.opcode == "PUSHFRAME":
        #not defined temp_frame
        if temporary_frame == None:
            exit(55)
        local_frame.append(temporary_frame)
        temporary_frame = None

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
            if lf_index < 0:
                local_frame.append([])
                lf_index += 1
            local_frame[lf_index].append(promenna(instr.arg1))
        #tf
        elif frame == 2:
            temporary_frame.append(promenna(instr.arg1))

    elif instr.opcode == "CALL":

        is_label(instr.type1)
        my_order = get_label(instr.arg1)
        int_cycles(my_order)


    elif instr.opcode == "RETURN":
        if call_cnt == 0:
            exit(56)
        return "end"

    elif instr.opcode == "PUSHS":
        typ = is_symb(instr.type1)
        #var
        if typ == 1:
            index = find_index(instr.arg1.split('@')[0], instr.arg1.split('@')[1])
            var_typ = var_exist(instr.arg1)
            #PUSHS GF
            if var_typ == 0:
                var = global_frame[index]
                is_defined(var)
                stack.append(zas_item(var.value, var.val_type))
            #PUSHS LF
            elif var_typ == 1:
                var = local_frame[lf_index][index]
                is_defined(var)
                stack.append(zas_item(var.value, var.val_type))
            #PUSHS TF
            elif var_typ == 2:
                var = temporary_frame[index]
                is_defined(var)
                stack.append(zas_item(var.value, var.val_type))
        elif typ == 0:
            stack.append(zas_item(instr.arg1,instr.type1))
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
            item = stack.pop()
            #POPS GF
            if var_typ == 0:
                global_frame[index].value = item.value
                global_frame[index].val_type = item.val_type
            #POPS LF
            elif var_typ == 1:
                local_frame[lf_index][index].value = item.value
                local_frame[lf_index][index].val_type = item.val_type
            #POPS TF
            elif var_typ == 2:
                temporary_frame[index].value = item.value
                temporary_frame[index].val_type = item.val_type
        else:
            exit(32)

    elif instr.opcode == "ADD" or instr.opcode == "SUB" or instr.opcode == "MUL" or instr.opcode == "IDIV":
        
        symb = is_symb(instr.type1)
        symb2 = is_symb(instr.type2)
        symb3 = is_symb(instr.type3)

        #not var
        if symb != 1:
            exit(32)
        if symb2 == 2:
            exit(99)
        if symb3 == 2:
            exit(99)
        if symb2 == 1:
            var2 = get_var(instr.arg2)
            is_defined(var2)
        elif symb2 == 0:
            var2 = promenna("temp@temp")
            var2.value = instr.arg2
            var2.val_type = instr.type2
        if symb3 == 1:
            var3 = get_var(instr.arg3)
            is_defined(var3)
        elif symb3 == 0:
            var3 = promenna("temp@temp")
            var3.value = instr.arg3
            var3.val_type = instr.type3
        if not var2.val_type or not var3.val_type:
            exit(56)
        if var2.val_type != "int":
            exit(53)
        elif var3.val_type != "int":
            exit(53)

        variable = promenna(instr.arg1)
        index1 = find_index(instr.arg1.split('@')[0], instr.arg1.split('@')[1])
        var1 = var_exist(instr.arg1)

        if instr.opcode == "ADD":
            variable.value = int(var2.value) + int(var3.value)
        elif instr.opcode == "SUB":
            variable.value = int(var2.value) - int(var3.value)
        elif instr.opcode == "MUL":
            variable.value = int(var2.value) * int(var3.value)
        elif instr.opcode == "IDIV":
            if int(var3.value) == 0:
                exit(57)
            variable.value = int(var2.value) // int(var3.value)
        variable.val_type = "int"
        var_to_f(variable, instr.arg1.split('@')[0])
            


    elif instr.opcode == "LT" or instr.opcode == "GT" or instr.opcode == "EQ":
        symb = is_symb(instr.type1)
        symb2 = is_symb(instr.type2)
        symb3 = is_symb(instr.type3)
        #not var
        if symb != 1:
            exit(32)
        if symb2 == 2:
            exit(99)
        if symb3 == 2:
            exit(99)
        if symb2 == 1:
            var2 = get_var(instr.arg2)
            is_defined(var2)
        elif symb2 == 0:
            var2 = promenna("temp@temp")
            var2.value = instr.arg2
            var2.val_type = instr.type2
        if symb3 == 1:
            var3 = get_var(instr.arg3)
            is_defined(var3)
        elif symb3 == 0:
            var3 = promenna("temp@temp")
            var3.value = instr.arg3
            var3.val_type = instr.type3
        variable = promenna(instr.arg1)
        #undefined
        if not var2.val_type or not var3.val_type:
            exit(56)
        #bad operand type
        if var2.val_type != var3.val_type and str(var2.val_type) != "nil" and str(var3.val_type) != "nil":
            exit(53)

        if instr.opcode == "EQ":

            if var2.val_type == "nil" or var3.val_type == "nil":
                variable.value = var2.value == var3.value
                variable.value = str(variable.value).lower()
            elif(var2.val_type == "int" or var3.val_type == "int"):
                variable.value = int(var2.value) == int(var3.value)
                variable.value = str(variable.value).lower()
            elif(var2.val_type == "string" or var3.val_type == "string"):
                variable.value = str(var2.value) == str(var3.value)
                variable.value = str(variable.value).lower()
            elif(var2.val_type == "bool" or var3.val_type == "bool"):
                if var2.value == "false":
                    var2.value = 0
                else:
                    var2.value = 1
                if var3.value == "false":
                    var3.value = 0
                else:
                    var3.value = 1
                variable.value = int(var2.value) == int(var3.value)
                variable.value = str(variable.value).lower()
            else:
                variable.value = var2.value == var3.value
                variable.value = variable.value.lower()


        elif instr.opcode == "LT":
            if str(var2.val_type) == "nil" or str(var3.val_type) == "nil":
                exit(53)
            if(var2.val_type == "int"):
                variable.value = int(var2.value) < int(var3.value)
                variable.value = str(variable.value).lower()
            elif(var2.val_type == "string"):
                variable.value = str(var2.value) < str(var3.value)
                variable.value = str(variable.value).lower()
            elif(var2.val_type == "bool"):
                if var2.value == "false":
                    var2.value = 0
                else:
                    var2.value = 1
                if var3.value == "false":
                    var3.value = 0
                else:
                    var3.value = 1
                variable.value = int(var2.value) < int(var3.value)
                variable.value = str(variable.value).lower()

        elif instr.opcode == "GT":
            if str(var2.val_type) == "nil" or str(var3.val_type) == "nil":
                exit(53)
            if(var2.val_type == "int"):
                variable.value = int(var2.value) > int(var3.value)
                variable.value = str(variable.value).lower()
            elif(var2.val_type == "string"):
                variable.value = str(var2.value) > str(var3.value)
                variable.value = str(variable.value).lower()
            elif(var2.val_type == "bool"):
                if var2.value == "false":
                    var2.value = 0
                else:
                    var2.value = 1
                if var3.value == "false":
                    var3.value = 0
                else:
                    var3.value = 1
                variable.value = int(var2.value) > int(var3.value)
                variable.value = str(variable.value).lower()

        else:
            exit(99)

        variable.val_type = "bool"
        var_to_f(variable, instr.arg1.split('@')[0])

    elif instr.opcode == "AND" or instr.opcode == "OR":
        symb = is_symb(instr.type1)
        symb2 = is_symb(instr.type2)
        symb3 = is_symb(instr.type3)
        #not var
        if symb != 1:
            exit(32)
        if symb2 == 2:
            exit(99)
        if symb3 == 2:
            exit(99)
        if symb2 == 1:
            var2 = get_var(instr.arg2)
            is_defined(var2)
        elif symb2 == 0:
            var2 = promenna("temp@temp")
            var2.value = instr.arg2
            var2.val_type = instr.type2
        if symb3 == 1:
            var3 = get_var(instr.arg3)
            is_defined(var3)
        elif symb3 == 0:
            var3 = promenna("temp@temp")
            var3.value = instr.arg3
            var3.val_type = instr.type3
        variable = promenna(instr.arg1)
        
        if var2.val_type != "bool" or var3.val_type != "bool":
            exit(53)
        variable.val_type = "bool"
        if instr.opcode == "AND":
            if str(var2.value) == "true" and str(var3.value) == "true":
                variable.value = "true"
            else:
                variable.value = "false"
        elif instr.opcode == "OR":
            if str(var2.value) == "false" and str(var3.value) == "false":
                variable.value = "false"
            else:
                variable.value = "true"
        var_to_f(variable, instr.arg1.split('@')[0])

    elif instr.opcode == "NOT":

        symb = is_symb(instr.type1)
        symb2 = is_symb(instr.type2)
        #not var
        if symb != 1:
            exit(32)
        if symb2 == 2:
            exit(99)
        if symb2 == 1:
            var2 = get_var(instr.arg2)
            is_defined(var2)
        elif symb2 == 0:
            var2 = promenna("temp@temp")
            var2.value = instr.arg2
            var2.val_type = instr.type2
        #undefined
        if var2.val_type != "bool":
            exit(53)
        variable = promenna(instr.arg1)
        if str(var2.value) != "true":
            variable.value = "true"
        else:
            variable.value = "false"
        variable.val_type = "bool"
        var_to_f(variable, instr.arg1.split('@')[0])


    elif instr.opcode == "INT2CHAR":
        symb = is_symb(instr.type1)
        symb2 = is_symb(instr.type2)
        #not var
        if symb != 1:
            exit(32)
        if symb2 == 2:
            exit(99)
        if symb2 == 1:
            var2 = get_var(instr.arg2)
            is_defined(var2)
        elif symb2 == 0:
            var2 = promenna("temp@temp")
            var2.value = instr.arg2
            var2.val_type = instr.type2

        if var2.val_type != "int":
            exit(53)
        variable = promenna(instr.arg1)
        if not var2.value or int(var2.value) < 0 or int(var2.value) >= 1114112:
            exit(58)
        variable.value = chr(int(var2.value))
        variable.val_type = "string"
        var_to_f(variable, instr.arg1.split('@')[0])

    elif instr.opcode == "STRI2INT":
        symb = is_symb(instr.type1)
        symb2 = is_symb(instr.type2)
        symb3 = is_symb(instr.type3)
        #not var
        if symb != 1:
            exit(32)
        if symb2 == 2:
            exit(99)
        if symb3 == 2:
            exit(99)
        if symb2 == 1:
            var2 = get_var(instr.arg2)
            is_defined(var2)
        elif symb2 == 0:
            var2 = promenna("temp@temp")
            var2.value = instr.arg2
            var2.val_type = instr.type2
        if symb3 == 1:
            var3 = get_var(instr.arg3)
            is_defined(var3)
        elif symb3 == 0:
            var3 = promenna("temp@temp")
            var3.value = instr.arg3
            var3.val_type = instr.type3

        if var2.val_type != "string" or var3.val_type != "int":
            exit(53)
        variable = promenna(instr.arg1)
        variable.val_type = "int"
        if int(var3.value) >= len(var2.value) or int(var3.value) < 0:
            exit(58)
        variable.value = ord(var2.value[int(var3.value)])
        var_to_f(variable, instr.arg1.split('@')[0])

    elif instr.opcode == "READ":
        neco = inputs.readline()
        konec = True
        if len(neco) != 1:
            neco = neco.rstrip("\n")
        else:
            neco = neco.rstrip("\n")
            konec = False
        if instr.type2 != "type":
            exit(32)
        #not var
        if is_symb(instr.type1) != 1:
            exit(32)
        variable = promenna(instr.arg1)
        variable.val_type = instr.arg2
        if len(neco) == 0 and konec == True:
            variable.value = ""
            variable.val_type = "nil"
        elif instr.arg2 == "bool":
            if neco.lower() == "true":
                variable.value = "true"
            else:
                variable.value = "false"
            variable.val_type = "bool"
        elif instr.arg2 == "int":
            match = re.match('^(-|\+)*[0-9]+$', neco)
            if not match:
                variable.value = ""
                variable.val_type = "nil"
            else:
                variable.value = neco
        else:
            variable.value = neco
            variable.val_type = instr.arg2
        var_to_f(variable, instr.arg1.split('@')[0])



    elif instr.opcode == "WRITE":
        value = is_symb(instr.type1)
        #var
        if value == 1:
            var_type = var_exist(instr.arg1)
            index = find_index(instr.arg1.split('@')[0],instr.arg1.split('@')[1])
            #var in GF
            if var_type == 0:
                var = global_frame[index]
                is_defined(var)
                if var.val_type == "bool":
                    print(var.value, end='')
                elif var.val_type == "nil":
                    print("", end='')
                else:
                    print(var.value, end='')
            #var in LF
            if var_type == 1:
                var = local_frame[lf_index][index]
                is_defined(var)
                if var.val_type == "bool":
                    print(var.value, end='')
                elif var.val_type == "nil":
                    print("", end='')
                else:
                    print(var.value, end='')
            #var in TF
            if var_type == 2:
                var = temporary_frame[index]
                is_defined(var)
                if var.val_type == "bool":
                    print(var.value, end='')
                elif var.val_type == "nil":
                    print("", end='')
                else:
                    print(var.value, end='')
        elif value == 0:
            kon_type = instr.type1
            kon_value = instr.arg1
            if  kon_type == "bool":
                print(kon_value, end='')
            elif kon_type == "nil":
                print("", end='')
            else:
                print(kon_value, end='')

    elif instr.opcode == "CONCAT":
        symb = is_symb(instr.type1)
        symb2 = is_symb(instr.type2)
        symb3 = is_symb(instr.type3)
        #not var
        if symb != 1:
            exit(32)
        if symb2 == 2:
            exit(99)
        if symb3 == 2:
            exit(99)
        if symb2 == 1:
            var2 = get_var(instr.arg2)
            is_defined(var2)
        elif symb2 == 0:
            var2 = promenna("temp@temp")
            var2.value = instr.arg2
            var2.val_type = instr.type2
        if symb3 == 1:
            var3 = get_var(instr.arg3)
            is_defined(var3)
        elif symb3 == 0:
            var3 = promenna("temp@temp")
            var3.value = instr.arg3
            var3.val_type = instr.type3
        variable = promenna(instr.arg1)
        variable.val_type = "string"
        if not var2.val_type or not var3.val_type:
            exit(56)
        if var2.val_type != var3.val_type:
            exit(53)
        if var2.val_type != "string":
            exit(53)
        elif var3.val_type != "string":
            exit(53)
        variable.value = str(var2.value) + str(var3.value)
        var_to_f(variable, instr.arg1.split('@')[0])

    elif instr.opcode == "STRLEN":
        symb = is_symb(instr.type1)
        symb2 = is_symb(instr.type2)
        #not var
        if symb != 1:
            exit(32)
        if symb2 == 2:
            exit(99)
        if symb2 == 1:
            var2 = get_var(instr.arg2)
            is_defined(var2)
        elif symb2 == 0:
            var2 = promenna("temp@temp")
            var2.value = instr.arg2
            var2.val_type = instr.type2
        variable = promenna(instr.arg1)
        variable.val_type = "int"
        if not var2.val_type:
            exit(56)
        if var2.val_type != "string":
            exit(53)
        variable.value = len(var2.value)
        var_to_f(variable, instr.arg1.split('@')[0])

    elif instr.opcode == "GETCHAR":
        symb = is_symb(instr.type1)
        symb2 = is_symb(instr.type2)
        symb3 = is_symb(instr.type3)
        #not var
        if symb != 1:
            exit(32)
        if symb2 == 2:
            exit(99)
        if symb3 == 2:
            exit(99)
        if symb2 == 1:
            var2 = get_var(instr.arg2)
            is_defined(var2)
        elif symb2 == 0:
            var2 = promenna("temp@temp")
            var2.value = instr.arg2
            var2.val_type = instr.type2
        if symb3 == 1:
            var3 = get_var(instr.arg3)
            is_defined(var3)
        elif symb3 == 0:
            var3 = promenna("temp@temp")
            var3.value = instr.arg3
            var3.val_type = instr.type3
        variable = promenna(instr.arg1)
        variable.val_type = "string"
        if var2.val_type != "string":
            exit(53)
        elif var3.val_type != "int":
            exit(53)
        if len(var2.value) <= int(var3.value) or int(var3.value) < 0:
            exit(58)
        variable.value = str(var2.value)[int(var3.value)]
        var_to_f(variable, instr.arg1.split('@')[0])

    elif instr.opcode == "SETCHAR":
        symb = is_symb(instr.type1)
        symb2 = is_symb(instr.type2)
        symb3 = is_symb(instr.type3)
        #not var
        if symb != 1:
            exit(32)
        if symb2 == 2:
            exit(99)
        if symb3 == 2:
            exit(99)
        if symb2 == 1:
            var2 = get_var(instr.arg2)
            is_defined(var2)
        elif symb2 == 0:
            var2 = promenna("temp@temp")
            var2.value = instr.arg2
            var2.val_type = instr.type2
        if symb3 == 1:
            var3 = get_var(instr.arg3)
            is_defined(var3)
        elif symb3 == 0:
            var3 = promenna("temp@temp")
            var3.value = instr.arg3
            var3.val_type = instr.type3
        variable = get_var(instr.arg1)

        if not var2.val_type or not var3.val_type or not variable.val_type:
            exit(56)
        if var2.val_type != "int" or variable.val_type != "string":
            exit(53)
        elif var3.val_type != "string":
            exit(53)
        if int(var2.value) < 0 or int(var2.value) >= len(str(variable.value)) or len(str(variable.value)) == 0:
            exit(58)
        variable_1 = list(str(variable.value))
        if len(str(var3.value)) == 0:
            exit(58)
        variable_1[int(var2.value)] = str(var3.value)[0]
        variable.value = "".join(variable_1)
        var_to_f(variable, instr.arg1.split('@')[0])


    elif instr.opcode == "TYPE":
        symb = is_symb(instr.type1)
        symb2 = is_symb(instr.type2)
        #not var
        if symb != 1:
            exit(32)
        if symb2 == 2:
            exit(99)
        if symb2 == 1:
            var2 = get_var(instr.arg2)
        elif symb2 == 0:
            var2 = promenna("temp@temp")
            var2.value = instr.arg2
            var2.val_type = instr.type2
        variable = promenna(instr.arg1)
        variable.val_type = "string"
        variable.value = str(var2.val_type)
        if not var2.val_type:
            variable.value = ""
        var_to_f(variable, instr.arg1.split('@')[0])



    elif instr.opcode == "LABEL":
        return

    elif instr.opcode == "JUMP":
        is_label(instr.type1)
        return get_label(instr.arg1)

    elif instr.opcode == "JUMPIFEQ":
        is_label(instr.type1)
        symb2 = is_symb(instr.type2)
        symb3 = is_symb(instr.type3)
        labell = get_label(instr.arg1)
        if symb2 == 2 or symb3 == 2:
            exit(53)
        #var
        if symb2 == 1:
            var2 = get_var(instr.arg2)
            is_defined(var2)
        elif symb2 == 0:
            var2 = promenna("temp@temp")
            var2.value = instr.arg2
            var2.val_type = instr.type2
        #var
        if symb3 == 1:
            var3 = get_var(instr.arg3)
            is_defined(var3)
        elif symb3 == 0:
            var3 = promenna("temp@temp")
            var3.value = instr.arg3
            var3.val_type = instr.type3
        if var2.val_type != var3.val_type and str(var2.val_type) != "nil" and str(var3.val_type) != "nil":
            exit(53)
        if var2.val_type == "int" and var3.val_type == "int":
            if int(var2.value) == int(var3.value):
                return labell
        elif var2.val_type == "string" and var3.val_type == "string":
            if str(var2.value) == str(var3.value):
                return labell
        elif var2.val_type == "bool" and var3.val_type == "bool":
            if str(var2.value) == str(var3.value):
                return labell
        elif var2.val_type == "nil" and var3.val_type == "nil":
            if str(var2.value) and str(var3.value):
                return labell

    elif instr.opcode == "JUMPIFNEQ":

        is_label(instr.type1)
        symb2 = is_symb(instr.type2)
        symb3 = is_symb(instr.type3)
        labell = get_label(instr.arg1)
        if symb2 == 2 or symb3 == 2:
            exit(53)
        #var
        if symb2 == 1:
            var2 = get_var(instr.arg2)
            is_defined(var2)
        elif symb2 == 0:
            var2 = promenna("temp@temp")
            var2.value = instr.arg2
            var2.val_type = instr.type2
        #var
        if symb3 == 1:
            var3 = get_var(instr.arg3)
            is_defined(var3)
        elif symb3 == 0:
            var3 = promenna("temp@temp")
            var3.value = instr.arg3
            var3.val_type = instr.type3
        if var2.val_type != var3.val_type and str(var2.val_type) != "nil" and str(var3.val_type) != "nil":
            exit(53)
        if var2.val_type == "int" and var3.val_type == "int":
            if int(var2.value) == int(var3.value):
                return

        elif var2.val_type == "string" and var3.val_type == "string":
            if str(var2.value) == str(var3.value):
                return

        elif var2.val_type == "bool" and var3.val_type == "bool":
            if var2.value == var3.value:
                return
            
        elif var2.val_type == "nil" and var3.val_type == "nil":
            if var2.value == var3.value:
                return
        return labell


    elif instr.opcode == "EXIT":
        symb = is_symb(instr.type1)
        if symb == 2:
            exit(99)
        elif symb == 1:
            var = get_var(instr.arg1)
            is_defined(var)
        elif symb == 0:
            var = promenna("temp@temp")
            var.value = instr.arg1
            var.val_type = instr.type1

        if not var.val_type:
            exit(56)
        if var.val_type != "int":
            exit(53)
        if int(var.value) < 0 or int(var.value) > 49:
            exit(57)
        exit(int(var.value))

    elif instr.opcode == "DPRINT":
        symb = is_symb(instr.type1)
        if symb == 2:
            exit(99)
        elif symb == 1:
            var = get_var(instr.arg1)
            is_defined(var)
        elif symb == 0:
            var = promenna("temp@temp")
            var.value = instr.arg2
            var.val_type = instr.type2
        print(var.value, file=sys.stderr)

    elif instr.opcode == "BREAK":
        print("\nBREAK:\ngf:",global_frame,"\nlf:",local_frame[lf_cnt],"\ntf:",temporary_frame, file=sys.stderr)

    else:
        exit(32)

#return 
def get_var(name):

    global global_frame
    global local_frame
    global temporary_frame
    lf_index = len(local_frame) - 1

    frame = name.split('@')[0]
    nazev = name.split('@')[1]
    var_ex = var_exist(name)
    index = find_index(frame, nazev)
    if var_ex == 0:
        return global_frame[index]
    elif var_ex == 1:
        return local_frame[lf_index][index]
    elif var_ex == 2:
        return temporary_frame[index]
    else:
        exit(99)

#add var to frame, @ret 0 if ok
def var_to_f(var, frame):

    global global_frame
    global local_frame
    global temporary_frame
    lf_index = len(local_frame) - 1

    frame = frame.lower()
    index = find_index(frame, var.name)
    if frame == "gf":
        global_frame[index] = var
    elif frame == "lf":
        local_frame[lf_index][index] = var
    elif frame == "tf":
        temporary_frame[index] = var
    return 0

#is par symb ? @ret if symb 0, 1 if var, else 2
def is_symb(par):
    if par == "int" or par == "bool" or par == "string" or par == "nil":
        return 0
    if par == "var":
        return 1
    return 2

def is_label(label):
    if label == "label":
        return
    exit(53)

#is par frame ? @ret if global 0, if local 1, if temp 2, else 3
def is_frame(par):
    #for case insensitive
    frame = par.split('@')[0]
    frame = frame.lower()
    if frame != "gf":
        if frame != "lf":
            if frame != "tf":
                return 3
            return 2
        return 1
    return 0

#return index of variable in frame, else return -1
def find_index(frame, name):
    global global_frame
    global local_frame
    global temporary_frame
    lf_index = len(local_frame) - 1

    frame = frame.lower()
    cnt = 0
    if frame == "gf":
        delka = len(global_frame)
        while cnt < delka:
            if global_frame[cnt].name == name:
                return cnt
            cnt += 1
    elif frame == "lf":
        if len(local_frame) == 0:
            exit(55)
        delka = len(local_frame[lf_index])
        if delka == 0:
            exit(54)
        while cnt < delka:
            if local_frame[lf_index][cnt].name == name:
                return cnt
            cnt += 1
    elif frame == "tf":
        try:
            delka = len(temporary_frame)
        except:
            exit(55)
        while cnt < delka:
            if temporary_frame[cnt].name == name:
                return cnt
            cnt += 1
    
    exit(54)


#Does var exist ? @ret 0 if global, 1 if local, 2 if temporary, 3 if doesnt exist
def var_exist(par):

    global global_frame
    global local_frame
    global temporary_frame
    lf_index = len(local_frame) - 1


    cnt = 0
    frame = is_frame(par)
    name = par.split('@')[1]
    if frame == 3:
        return 3
    elif frame == 0:
        delka = len(global_frame)
        while cnt < delka:
            if global_frame[cnt].name == name:
                return 0
            cnt += 1
    elif frame == 1:
        #empty local_frame
        if len(local_frame) == 0:
            exit(55)
        delka = len(local_frame[lf_index])
        while cnt < delka:
            if local_frame[lf_index][cnt].name == name:
                return 1
            cnt += 1
    elif frame == 2:
        try:
            delka = len(temporary_frame)
        except:
            exit(55)
        while cnt < delka:
            if temporary_frame[cnt].name == name:
                return 2
            cnt += 1
    return 3

def int_cycles(order):

    global instr_list
    global call_cnt
    my_instructions = instr_list.copy()
    count = len(instr_list)

    #go through all instructions in array
    while count > 0:
        cnt = 1
        min_instr = 0
        #search for lowest order value in array
        while cnt < count:
            if int(my_instructions[cnt].order) < int(my_instructions[min_instr].order) :
                min_instr = cnt
            cnt += 1
        if int(my_instructions[min_instr].order) == order:
            break
        instr = my_instructions[min_instr]

        #remove used instruction from array and decrease count
        my_instructions.remove(instr)
        count -= 1

    call_cnt += 1
    #go through all instructions in array
    while count > 0:
        cnt = 1
        min_instr = 0
        #search for lowest order value in array
        while cnt < count:
            if int(my_instructions[cnt].order) < int(my_instructions[min_instr].order) :
                min_instr = cnt
            cnt += 1
        instr = my_instructions[min_instr]

        #exec
        ret_val = instr_exec(instr)
        if ret_val:
            if ret_val == "end":
                call_cnt -= 1
                return
            #jump to higher order (need to remove instr in list)
            if int(instr.order) < int(ret_val):
                cnt = 0
                my_count = len(my_instructions)
                while cnt < my_count:
                    if int(my_instructions[cnt].order) < ret_val:
                        my_instructions.remove(my_instructions[cnt])
                        count -= 1
                    my_count -= 1 

            #jump to lower order (need to append list by instructions
            elif int(my_instructions[min_instr].order) > int(ret_val):
                cnt = 0
                my_list = instr_list.copy()
                my_count = len(my_list)
                while cnt < my_count:
                    if int(my_list[cnt].order) < int(instr.order) and int(my_list[cnt].order) > ret_val:
                        my_instructions.append(my_list[cnt].copy())
                        count += 1
                    cnt += 1
            continue



        #remove used instruction from array and decrease count
        my_instructions.remove(instr)
        count -= 1


def is_defined(var):
    if not var.val_type:
        exit(56)

#return order of label
def get_label(label):
    global instr_list
    delka = len(instr_list)
    cnt = 0
    while delka > cnt:
        if instr_list[cnt].opcode == "LABEL":
            if instr_list[cnt].arg1 == label:
                return int(instr_list[cnt].order)
        cnt += 1
    exit(52)


def close_them():
    if inputs:
        close(inputs)
        print("closed inputs")

main()
