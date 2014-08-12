#!/bin/bash
for n in `ls ./[0-9][0-9]_*`; do
    if [ -x "$n" ]; then
        ($n)
        if [[ "$?" -ne "0" ]]; then
            echo "Whoops! Test failure detected. Aborting."
            exit 1
        fi
   fi
done

