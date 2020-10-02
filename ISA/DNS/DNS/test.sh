#!/bin/bash

port=4400
ip=127.0.0.1
score=0

#TEST1 - Correct
nslookup -port=$port -type=A google.com $ip
ret="$(echo $?)"
if [ $ret -ne 0 ]; then
    score= expr $score + 1
    echo "Test 1: FAIL"
else
    echo "Test 1: PASS"
fi

#TEST2 - Wrong type
nslookup -port=$port -type=AAAA google.com $ip
ret="$(echo $?)"
if [ $ret -ne 0 ]; then
    score=$(expr($score + 1))
    echo "Test 2: FAIL"
else
    echo "Test 2: PASS"
fi

#TEST3 - Filtered domain
nslookup -port=$port -type=A filtered.com $ip 
ret="$(echo $?)"
if [ $ret -ne 0 ]; then
    score=$(expr($score + 1)) 
    echo "Test 3: FAIL"
else
    echo "Test 3: PASS"
fi

echo "Score is: $score"
