#!/bin/bash

testmsg2="google.com"
testmsg="This is testing message: "
var=0
ip="0.0.0.0"

for var in `seq $2`
do
    echo $testmsg2 | nc -u $ip $1
done
