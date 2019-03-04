#!/bin/bash


for i in {10..200..10}
do
  ./bin/create_graph $i
done
mv graph*.txt ./graphs
