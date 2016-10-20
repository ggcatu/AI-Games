#!/bin/bash

while read line
do 
  echo "$line" | timeout 600s ./aest >> 15PuzzleA.txt &

  [ $(jobs | wc -l) -ge 3 ] && wait
  
done < korf.txt