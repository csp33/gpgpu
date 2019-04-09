#!/bin/bash


for graph in `ls ./graphs/*.txt | sort -V`;
do
  echo "**********************$graph************************************"
  echo "GPU: ";
  bin/dijkstra_gpu $graph;
  echo "CPU: ";
  bin/dijkstra_cpu $graph;

done
