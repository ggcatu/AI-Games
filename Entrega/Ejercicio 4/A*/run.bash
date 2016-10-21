#!/bin/bash

ulimit -v 2097152
echo "grupo, algorithm, heuristic, domain, instance, cost, h0, generated, time, gen_per_sec" >> 15PuzzleAManh.txt

while read line
do 
  echo "$line" | timeout 600s ./15Puzzle.aestrella_2 0 >> 15PuzzleAManh.txt &

  [ $(jobs | wc -l) -ge 3 ] && wait
  
done < korf.txt