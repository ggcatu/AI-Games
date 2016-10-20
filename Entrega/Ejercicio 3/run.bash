#!/bin/bash

while read line
do 
  echo "$line" | timeout 23s ./16_pancake.dfs_2 >> e.txt   
done < pancake16.txt
