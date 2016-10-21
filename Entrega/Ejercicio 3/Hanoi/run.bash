#!/bin/bash

echo "grupo, algorithm, heuristic, domain, instance, cost, h0, generated, time, gen_per_sec" >> hanoi14_4.txt

while read line
do 
  echo "$line" | timeout 100s ./4_14_Hanoi.dfs_2 >> hanoi14_4.txt &

  [ $(jobs | wc -l) -ge $(nproc) ] && wait
  
done < tower14_4.txt
