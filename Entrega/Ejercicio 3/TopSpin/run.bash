#!/bin/bash

while read line
do 
  echo "$line" | timeout 100s ./16_4_topspin.dfs_2 >> e.txt &

  [ $(jobs | wc -l) -ge $(nproc) ] && wait
  
done < topspin16-4.txt
