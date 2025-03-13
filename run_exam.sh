#!/bin/bash

n=9
if [ $# -gt 0 ]; then
    if [[ $1 -ge 1 && $1 -le 9 ]]; then
        n=$1
    else
        echo "n should between 1 and 9!"
        exit 1
    fi
fi

for ((i=1; i<=n && i<=8; i++))
do
    bash exam_$i.sh
    echo "exam_$i.sh done" >&2
done

if [ $n -ge 9 ]; then
    bash exam_9.sh $2 $3
    echo "exam_9.sh done" >&2
else
    exit 0
fi
