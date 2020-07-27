#!/usr/bin/env python3

import sys
import re


root = "lists.txt"

def main():
    #go through all lines on STDIN
    for line in sys.stdin:
        if re.match('^---', line):
            print(line)

def is_replacable(what):
    pass


def replace(by,what):
    pass

main()
