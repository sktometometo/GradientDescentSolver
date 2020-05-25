#!/bin/sh

degree=2
error=1.0

./02_convex -a 0 -d $degree -e 1 --printparam 1 > ./param2_1.dat
sleep 1
cat ./param2_1.dat | head -n $((2+1+$degree+1+$degree+1)) > ./param2_2.dat
./02_convex -a 0 -d $degree -e 1 --printparam 1 | tail -n $degree >> ./param2_2.dat
sleep 1
cat ./param2_1.dat | head -n $((2+1+$degree+1+$degree+1)) > ./param2_3.dat
./02_convex -a 0 -d $degree -e 1 --printparam 1 | tail -n $degree >> ./param2_3.dat
sleep 1
cat ./param2_1.dat | head -n $((2+1+$degree+1+$degree+1)) > ./param2_4.dat
./02_convex -a 0 -d $degree -e 1 --printparam 1 | tail -n $degree >> ./param2_4.dat
sleep 1
cat ./param2_1.dat | head -n $((2+1+$degree+1+$degree+1)) > ./param2_5.dat
./02_convex -a 0 -d $degree -e 1 --printparam 1 | tail -n $degree >> ./param2_5.dat

echo "params are genereated."

./02_convex -a 0 -e $error --loadparamfile ./param2_1.dat --debug 2 > ./result2_1.dat
./02_convex -a 0 -e $error --loadparamfile ./param2_2.dat --debug 2 > ./result2_2.dat
./02_convex -a 0 -e $error --loadparamfile ./param2_3.dat --debug 2 > ./result2_3.dat
./02_convex -a 0 -e $error --loadparamfile ./param2_4.dat --debug 2 > ./result2_4.dat
./02_convex -a 0 -e $error --loadparamfile ./param2_5.dat --debug 2 > ./result2_5.dat

echo "results are genereated."

gnuplot -e "
    set xlabel 'k' ;
    set ylabel 'error' ;
    set title 'convex without acceleration' ;
    set logscale y ;
    plot './result2_1.dat' using 1:2 ;
    replot './result2_2.dat' using 1:2 ;
    replot './result2_3.dat' using 1:2 ;
    replot './result2_4.dat' using 1:2 ;
    replot './result2_5.dat' using 1:2 ;
    set terminal png ;
    set output './result2.png' ;
    replot
    "

echo "plot is generated."
