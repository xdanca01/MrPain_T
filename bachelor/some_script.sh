#!/bin/bash

package=$(rpm -qf `which $1`);
return_code=$?;
if [ $return_code -ne 0 ]; then
    #check if cmd is system cmd
    return_code=$($1 2>/dev/null | grep "command not found" 2>&1>/dev/null; echo $?);
    if [ $return_code -ne 0 ]; then
        echo "didnt find command $1";
        exit 1;
    fi
fi
output=$($1 --help 2>/dev/null | grep -ve '\-help' -ve '\-h' 2>/dev/null | grep -o '\-.*' 2>/dev/null | awk '{print $1}' | tr ",\|)\|\.\|\[\|\]" " ");
zkouska=$($1 --help 2>/dev/null );
return_code=$?;
if [ $return_code -ne 0 ]; then
    output=$($1 -h 2>/dev/null | grep -ve '\-h' -ve '\-help' 2>/dev/null | grep -o '\-.*' 2>/dev/null | awk '{print $1}' | tr ",\|)\|\.\|\[\|\]" " ");
    zkouska=$($1 -h 2>/dev/null);
    return_code=$?;
    if [ $return_code -ne 0 ]; then
        output=$($1 -help 2>/dev/null | grep -ve '\-help' 2>/dev/null | grep -o '\-.*' 2>/dev/null | awk '{print $1}' | tr ",\|)\|\.\|\[\|\]" " ")
        zkouska=$($1 -help 2>/dev/null);
        return_code=$?;
        if [ $return_code -ne 0 ]; then
            echo "ERROR: couldnt get help option output for $1 command"
            exit 2;
        fi
    fi
fi
echo $output;
exit 0;
