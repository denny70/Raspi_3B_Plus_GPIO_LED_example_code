#!/bin/sh


#arr="14 15 18 23 24 25 8 7"
arr="2 3 4 5 6 7 8 9"
while [ 1 ]; do
    for item in $arr; do
        sudo ./a.out -w $item 1
        sleep 0.1
    done
    for item in $arr; do
        sudo ./a.out -w $item 0
        sleep 0.1
    done
done

