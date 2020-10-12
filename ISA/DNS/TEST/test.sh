#!/bin/bash

port=4400
ip=127.0.0.1
score=0

#TEST1 - Correct
nslookup -port=$port -type=A google.com $ip >/dev/null
ret="$(echo $?)"
if [ $ret -eq 1 ]; then
    score=$(expr $score + 1 )
    echo "Test 1: FAIL"
else
    echo "Test 1: PASS"
fi

#TEST2 - Wrong type
nslookup -port=$port -type=AAAA google.com $ip >/dev/null
ret="$(echo $?)"
if [ $ret -eq 0 ]; then
    score=$(expr $score + 1 )
    echo "Test 2: FAIL"
else
    echo "Test 2: PASS"
fi

#TEST3 - Filtered domain
nslookup -port=$port -type=A filtered.com $ip >/dev/null
ret="$(echo $?)"
if [ $ret -eq 0 ]; then
    score=$(expr $score + 1 )
    echo "Test 3: FAIL"
else
    echo "Test 3: PASS"
fi

#TEST4 - Filtered subdomain
nslookup -port=$port -type=A subdomain.filtered.com $ip >/dev/null
ret="$(echo $?)"
if [ $ret -eq 0 ]; then
    score=$(expr $score + 1 )
    echo "Test 4: FAIL"
else
    echo "Test 4: PASS"
fi

echo "Number of failed tests: $score"
