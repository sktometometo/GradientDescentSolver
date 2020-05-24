#!/bin/sh

./01_nonconvex -a 0 -d 10 -e 0.001 --printparam 1 > ./param1_1.dat
sleep 1
cat ./param1_1.dat | head -n 27 > ./param1_2.dat
./01_nonconvex -a 0 -d 10 -e 0.001 --printparam 1 | tail -n 10 >> ./param1_2.dat
sleep 1
cat ./param1_1.dat | head -n 27 > ./param1_3.dat
./01_nonconvex -a 0 -d 10 -e 0.001 --printparam 1 | tail -n 10 >> ./param1_3.dat
sleep 1
cat ./param1_1.dat | head -n 27 > ./param1_4.dat
./01_nonconvex -a 0 -d 10 -e 0.001 --printparam 1 | tail -n 10 >> ./param1_4.dat
sleep 1
cat ./param1_1.dat | head -n 27 > ./param1_5.dat
./01_nonconvex -a 0 -d 10 -e 0.001 --printparam 1 | tail -n 10 >> ./param1_5.dat

echo "params are genereated."

./01_nonconvex -a 0 -e 0.001 --loadparamfile ./param1_1.dat --debug 1 > ./result1_1.dat
./01_nonconvex -a 0 -e 0.001 --loadparamfile ./param1_2.dat --debug 1 > ./result1_2.dat
./01_nonconvex -a 0 -e 0.001 --loadparamfile ./param1_3.dat --debug 1 > ./result1_3.dat
./01_nonconvex -a 0 -e 0.001 --loadparamfile ./param1_4.dat --debug 1 > ./result1_4.dat
./01_nonconvex -a 0 -e 0.001 --loadparamfile ./param1_5.dat --debug 1 > ./result1_5.dat

echo "results are genereated."

gnuplot -e "
    set xlabel 'k' ;
    set ylabel 'error' ;
    set title 'nonconvex' ;
    set logscale y ;
    plot './result1_1.dat' ;
    replot './result1_2.dat' ;
    replot './result1_3.dat' ;
    replot './result1_4.dat' ;
    replot './result1_5.dat' ;
    set terminal png ;
    set output './result1.png' ;
    replot
    "

echo "plot is generated."
