#!/bin/bash


for i in `seq 1 100`; do

    echo $i
    nslookup -port=5051 -type=A google.com localhost &
done
