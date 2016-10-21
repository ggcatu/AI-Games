#!/bin/bash

while read line
do 
  echo "$line" | timeout 600s ./11Puzzle.dfs_2 >> e.txt &

  [ $(jobs | wc -l) -ge $(nproc) ] && wait
  
done < 11puzzle.txt
