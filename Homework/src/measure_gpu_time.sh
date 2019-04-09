#!/bin/bash

echo "" > ./times/gpu_time.txt;
for graph in `ls ./graphs/*.txt | sort -V`;
do
  bin/dijkstra_gpu $graph >> ./times/gpu_time.txt;
done
