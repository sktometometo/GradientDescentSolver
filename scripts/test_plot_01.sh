#!/bin/sh

degree=2
error=1.0

./01_nonconvex -a 0 -d $degree -e 1.0 --printparam 1 > ./param1_1.dat
sleep 1
cat ./param1_1.dat | head -n $((2+1+$degree+1+$degree+2+1)) > ./param1_2.dat
./01_nonconvex -a 0 -d $degree -e 1.0 --printparam 1 | tail -n $degree >> ./param1_2.dat
sleep 1
cat ./param1_1.dat | head -n $((2+1+$degree+1+$degree+2+1)) > ./param1_3.dat
./01_nonconvex -a 0 -d $degree -e 1.0 --printparam 1 | tail -n $degree >> ./param1_3.dat
sleep 1
cat ./param1_1.dat | head -n $((2+1+$degree+1+$degree+2+1)) > ./param1_4.dat
./01_nonconvex -a 0 -d $degree -e 1.0 --printparam 1 | tail -n $degree >> ./param1_4.dat
sleep 1
cat ./param1_1.dat | head -n $((2+1+$degree+1+$degree+2+1)) > ./param1_5.dat
./01_nonconvex -a 0 -d $degree -e 1.0 --printparam 1 | tail -n $degree >> ./param1_5.dat

echo "params are genereated."

./01_nonconvex -a 0 -e $error --loadparamfile ./param1_1.dat --debug 2 > ./result1_1.dat
./01_nonconvex -a 0 -e $error --loadparamfile ./param1_2.dat --debug 2 > ./result1_2.dat
./01_nonconvex -a 0 -e $error --loadparamfile ./param1_3.dat --debug 2 > ./result1_3.dat
./01_nonconvex -a 0 -e $error --loadparamfile ./param1_4.dat --debug 2 > ./result1_4.dat
./01_nonconvex -a 0 -e $error --loadparamfile ./param1_5.dat --debug 2 > ./result1_5.dat

echo "results are genereated."

gnuplot -e "
    set xlabel 'k' ;
    set ylabel 'error' ;
    set title 'nonconvex' ;
    set logscale y ;
    plot './result1_1.dat' using 1:2 ;
    replot './result1_2.dat' using 1:2 ;
    replot './result1_3.dat' using 1:2 ;
    replot './result1_4.dat' using 1:2 ;
    replot './result1_5.dat' using 1:2 ;
    set terminal png ;
    set output './result1.png' ;
    replot
    "

echo "plot is generated."
