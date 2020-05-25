#!/bin/sh

degree=2
error=1

./02_convex -a 2 -d $degree -e 1 --printparam 1 > ./param4_1.dat
sleep 1
cat ./param4_1.dat | head -n $((2+1+$degree+1+$degree+1)) > ./param4_2.dat
./02_convex -a 2 -d $degree -e 1 --printparam 1 | tail -n $degree >> ./param4_2.dat
sleep 1
cat ./param4_1.dat | head -n $((2+1+$degree+1+$degree+1)) > ./param4_3.dat
./02_convex -a 2 -d $degree -e 1 --printparam 1 | tail -n $degree >> ./param4_3.dat
sleep 1
cat ./param4_1.dat | head -n $((2+1+$degree+1+$degree+1)) > ./param4_4.dat
./02_convex -a 2 -d $degree -e 1 --printparam 1 | tail -n $degree >> ./param4_4.dat
sleep 1
cat ./param4_1.dat | head -n $((2+1+$degree+1+$degree+1)) > ./param4_5.dat
./02_convex -a 2 -d $degree -e 1 --printparam 1 | tail -n $degree >> ./param4_5.dat

echo "params are genereated."

./02_convex -a 2 -e $error --loadparamfile ./param4_1.dat --debug 2 > ./result4_1.dat
./02_convex -a 2 -e $error --loadparamfile ./param4_2.dat --debug 2 > ./result4_2.dat
./02_convex -a 2 -e $error --loadparamfile ./param4_3.dat --debug 2 > ./result4_3.dat
./02_convex -a 2 -e $error --loadparamfile ./param4_4.dat --debug 2 > ./result4_4.dat
./02_convex -a 2 -e $error --loadparamfile ./param4_5.dat --debug 2 > ./result4_5.dat

echo "results are genereated."

gnuplot -e "
    set xlabel 'k' ;
    set ylabel 'error' ;
    set title 'convex with acceleration' ;
    set logscale y ;
    plot './result4_1.dat' using 1:2 ;
    replot './result4_2.dat' using 1:2 ;
    replot './result4_3.dat' using 1:2 ;
    replot './result4_4.dat' using 1:2 ;
    replot './result4_5.dat' using 1:2 ;
    set terminal png ;
    set output './result4.png' ;
    replot
    "

echo "plot is generated."
