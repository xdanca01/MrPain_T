#!/bin/bash

testmsg="This is testing message: "
var=0
ip="0.0.0.0"

for var in `seq $2`
do
    echo $testmsg $var | nc -u $ip $1
done
