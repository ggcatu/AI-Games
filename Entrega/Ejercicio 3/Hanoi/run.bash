#!/bin/bash

while read line
do 
  echo "$line" | timeout 100s ./4_14_Hanoi.dfs_2 >> e.txt &

  [ $(jobs | wc -l) -ge $(nproc) ] && wait
  
done < tower14_4.txt
