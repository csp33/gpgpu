#!/usr/bin/gnuplot

set terminal png size 640,480

set output './img/gpu_time.png'
plot './times/gpu_time.txt' with lines;
