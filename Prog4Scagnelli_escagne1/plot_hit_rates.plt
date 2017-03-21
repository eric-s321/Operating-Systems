#!/user/bin/gnuplot
reset

set terminal png
set datafile separator ","
set xlabel "Cache Size (Blocks)"
set ylabel "Hit Rate (%)"
set title title
set key inside right bottom

plot input_filename using 1:2 title "OPT", \
     input_filename using 1:3 title "LRU", \
     input_filename using 1:4 title "FIFO", \
     input_filename using 1:5 title "RAND", \
     input_filename using 1:6 title "CLOCK"
