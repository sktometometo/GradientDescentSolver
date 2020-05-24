#!/bin/sh

./02_convex -a 0 -d 10 -e 1 --printparam 1 > ./param2_1.dat
sleep 1
cat ./param2_1.dat | head -n 25 > ./param2_2.dat
./02_convex -a 0 -d 10 -e 1 --printparam 1 | tail -n 10 >> ./param2_2.dat
sleep 1
cat ./param2_1.dat | head -n 25 > ./param2_3.dat
./02_convex -a 0 -d 10 -e 1 --printparam 1 | tail -n 10 >> ./param2_3.dat
sleep 1
cat ./param2_1.dat | head -n 25 > ./param2_4.dat
./02_convex -a 0 -d 10 -e 1 --printparam 1 | tail -n 10 >> ./param2_4.dat
sleep 1
cat ./param2_1.dat | head -n 25 > ./param2_5.dat
./02_convex -a 0 -d 10 -e 1 --printparam 1 | tail -n 10 >> ./param2_5.dat

./02_convex -a 0 -e 1 --loadparamfile ./param2_1.dat --debug 1 > ./result2_1.dat
./02_convex -a 0 -e 1 --loadparamfile ./param2_2.dat --debug 1 > ./result2_2.dat
./02_convex -a 0 -e 1 --loadparamfile ./param2_3.dat --debug 1 > ./result2_3.dat
./02_convex -a 0 -e 1 --loadparamfile ./param2_4.dat --debug 1 > ./result2_4.dat
./02_convex -a 0 -e 1 --loadparamfile ./param2_5.dat --debug 1 > ./result2_5.dat

gnuplot -e "
    set terminal svg ;
    set output 'result2.svg' ;
    set xlabel 'k' ;
    set ylabel 'error' ;
    set title 'convex with acceleration'
    set yscale log ;
    plot './result2_1.dat' ;
    replot './result2_2.dat' ;
    replot './result2_3.dat' ;
    replot './result2_4.dat' ;
    replot './result2_5.dat' ;
    "
