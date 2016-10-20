#!/bin/bash

while read line
do 
  echo "$line" | timeout 100s ./16_pancake.dfs_2 >> e.txt &

  [ $(jobs | wc -l) -ge $(nproc) ] && wait
  
done < pancake16.txt
