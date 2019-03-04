#!/usr/bin/gnuplot

set terminal png size 640,480

set output './img/cpu_time.png'
plot './times/sequential_time.txt' with lines;
