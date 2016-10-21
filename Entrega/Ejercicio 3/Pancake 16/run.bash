#!/bin/bash
echo "grupo, algorithm, heuristic, domain, instance, cost, h0, generated, time, gen_per_sec" >> e.txt

while read line
do 
  echo "$line" | timeout 600s ./16_pancake.dfs_2 >> e.txt &

  [ $(jobs | wc -l) -ge $(nproc) ] && wait
  
done < pancake16.txt
