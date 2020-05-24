#!/bin/sh

./02_convex -a 2 -d 10 -e 1 --printparam 1 > ./param4_1.dat
sleep 1
cat ./param4_1.dat | head -n 25 > ./param4_2.dat
./02_convex -a 2 -d 10 -e 1 --printparam 1 | tail -n 10 >> ./param4_2.dat
sleep 1
cat ./param4_1.dat | head -n 25 > ./param4_3.dat
./02_convex -a 2 -d 10 -e 1 --printparam 1 | tail -n 10 >> ./param4_3.dat
sleep 1
cat ./param4_1.dat | head -n 25 > ./param4_4.dat
./02_convex -a 2 -d 10 -e 1 --printparam 1 | tail -n 10 >> ./param4_4.dat
sleep 1
cat ./param4_1.dat | head -n 25 > ./param4_5.dat
./02_convex -a 2 -d 10 -e 1 --printparam 1 | tail -n 10 >> ./param4_5.dat

./02_convex -a 2 -e 1 --loadparamfile ./param4_1.dat --debug 1 > ./result4_1.dat
./02_convex -a 2 -e 1 --loadparamfile ./param4_2.dat --debug 1 > ./result4_2.dat
./02_convex -a 2 -e 1 --loadparamfile ./param4_3.dat --debug 1 > ./result4_3.dat
./02_convex -a 2 -e 1 --loadparamfile ./param4_4.dat --debug 1 > ./result4_4.dat
./02_convex -a 2 -e 1 --loadparamfile ./param4_5.dat --debug 1 > ./result4_5.dat

gnuplot -e "
    set terminal svg ;
    set output 'result4.svg' ;
    set xlabel 'k' ;
    set ylabel 'error' ;
    set title 'convex with acceleration'
    set yscale log ;
    plot './result4_1.dat' ;
    replot './result4_2.dat' ;
    replot './result4_3.dat' ;
    replot './result4_4.dat' ;
    replot './result4_5.dat' ;
    "
