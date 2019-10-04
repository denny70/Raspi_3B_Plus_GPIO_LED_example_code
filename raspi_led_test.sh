#!/bin/sh


#arr="14 15 18 23 24 25 8 7"
arr="5 6 13 19 26 20 21"
for item in $arr; do
    sudo ./a.out $item $1
    sleep 0.5
done


