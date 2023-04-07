#!/bin/bash

IFS=","
textname=$1
index=0
LINE=()
str_count=0
sum=2
while read line; do
    LINE="$line"
    for item in $LINE; do
        array[$index]=$item
        index=$(($index+1))
    done
    str_count=$[$str_count+1]
done < $textname

for (( i=0; i < $str_count; i++ )); do
    h=`echo 1"/"${array[$i*($str_count+1)+$i]} | bc -l`
    for (( j=$i*($str_count+1); j <= $i*($str_count+1)+$str_count; j++ )); do
        array[$j]=`echo ${array[$j]}"*"$h | bc -l`
    done   
    for (( n=$i+1; n < $str_count; n++ )); do
        k=${array[$n*($str_count+1)+$i]}
        for (( l=$i; l < $str_count+1; l++ )); do
            array[$n*($str_count+1)+$l]=`echo "${array[$n*($str_count+1)+$l]}-($k*${array[$i*($str_count+1)+$l]})" | bc -l`
        done
    done
done

for (( int=$str_count-1; int > 0; int--)); do
    for (( i=$int; i > 0; i-- )); do
        k=${array[($i)*($str_count+1)-$sum]}
        array[($i-1)*($str_count+1) + $str_count]=`echo "${array[($i-1)*($str_count+1) + $str_count]}-($k*${array[$int*(str_count+1)+$str_count]})" | bc -l`
    done
    sum=`echo $sum"+"1 | bc -l`
done

for (( i=0; i < $str_count; i++ )); do
    echo ${array[$i*($str_count+1)+$str_count]}
done