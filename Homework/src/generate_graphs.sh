#!/bin/bash


for i in {1000..10000..500}
do
  ./bin/create_graph $i
done
mv graph*.txt ./graphs
