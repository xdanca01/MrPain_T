#!/usr/bin/env python3

import sys
import re
import subprocess

root = "lists.txt"

pairs = []



def main():
    load_options(root)
    #True = Not found '---' yet, False = Found '---' waiting for next '---' to end
    start = False
    #go through all lines on STDIN
    for line in sys.stdin:
        #add option to replace
        if re.match(r'\[.*#replace=.*\]', line):
            #Create new pair of replacables
            add_pairs(line)
        #ignore, because its NOTE
        if re.match(r'^===+', line):
            pass
        #found start/end for test task
        elif re.match(r'^(.)\1{3,}', line):
            start = not start
        #Is between '---'
        if start is True:
            line = replace(line)
        print(line[:-1])
    return
        

#Add options from route
def load_options(route):
    global pairs
    file = open(route,"r")
    lines = file.readlines()
    for line in lines:
        line = line.split()
        line = line[0]
        p = pair(line,None)
        pairs.append(p)
        
        
#Create new pair of replacavles
def add_pairs(what):
    global pairs
    match = re.findall(r'#replace=(.*?,)',what)
    if match is None:
        return
    #remove brackets
    what = match[0][:-1]
    #remove '=' for better split
    line = what.replace('=', ' ')
    line = line.split()
    #count words on line
    count = len(line)
    #increment by 2, from 1 to count
    for numb in range(0,count,2):
        #create new pair
        p = pair(line[numb+1],line[numb])
        pairs.append(p)
    return

def replace(what):
    global pairs
    #go through all pair to find matching pair
    for P in pairs:
        if P.R:
            something = re.search(P.R,what)
        else:
            something = re.search(P.O,what)
        #if pair is on line replace it
        if something:
            #get N from lists.txt (by what to replace)
            if P.N is None:
                P.O_to_N()            
            return P.Change(what)
    return what

#R - Replace (What to replace), O - Option (By what to replace), N - New text (output from lists.txt)
class pair:
    N = None
    R = None
    def __init__(self, option, R):
        if option is None:
            raise Exception("Object pair got null atribute")
        self.O = option
        if R is not None:
            self.check_pairs()
            self.R = R
        return
       
    #find N by O
    def O_to_N(self):
        global root
        line = open(root, "r")
        #get first line on which are replaceables
        text = line.readlines()
        for txt in text:
            if re.search(self.O, txt) is not None:
                txt = txt.split()
                blabla = re.match('./',txt[1])
                line.close()
                if blabla is not None:
                    smthg = subprocess.run([txt[1]], stdout=subprocess.PIPE)
                    smthg.stdout = smthg.stdout.decode('utf-8')
                    self.N = str(smthg.stdout)
                else:    
                    self.N = txt[1]
                return
        raise Exception("Missing replacable in lists.txt for:" + self.O)
    
    #remove occurence with same OPTION from pairs list
    def check_pairs(self):
        global pairs
        new_pairs = pairs.copy()
        while True:
            try:
                p = new_pairs.pop()
                if p.O == self.O:
                    pairs.remove(p)
                    return
            except:
                #Option is not on list yet
                return
    def Change(self,what):
        return re.sub(str(self.R),str(self.N),str(what))
main()
