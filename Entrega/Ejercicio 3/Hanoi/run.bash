#!/bin/bash

echo "grupo, algorithm, heuristic, domain, instance, cost, h0, generated, time, gen_per_sec" >> hanoi4_14.txt

while read line
do 
  echo "$line" | timeout 100s ./hanoi_4_14.dfs_1 >> hanoi4_14.txt &

  [ $(jobs | wc -l) -ge $(nproc) ] && wait
  
done < tower14_4.txt
