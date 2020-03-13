#!/bin/bash

while read neco; do
    if [ $neco == "0" ] ; then
        exit 1;
    fi
done
