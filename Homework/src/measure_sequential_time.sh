#!/bin/bash

echo "" > ./times/sequential_time.txt;
for graph in `ls ./graphs/*.txt | sort -V`;
do
  bin/dijkstra_cpu $graph >> ./times/sequential_time.txt;
done
