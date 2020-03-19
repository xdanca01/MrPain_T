#!/bin/bash

regex_cislo='^[0-9]+$'
regex_znamenko='^[+-]$'
while /bin/true; do
    read a;
    if ! [[ $a =~ $regex_cislo ]] ; then
        exit 1;
    fi
    read znamenko;
    if ! [[ $znamenko =~ $regex_znamenko ]]; then
        exit 1;
    fi
    read b;
    if ! [[ $b =~ $regex_cislo ]]; then
        exit 1;
    fi
    c= expr $a $znamenko $b
    echo $c;
done
