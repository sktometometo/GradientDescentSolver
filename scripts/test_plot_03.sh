#!/bin/sh

degree=2
error=0.1

./03_strongconvex -a 0 -d $degree -e 1 -l 0.9 --printparam 1 > ./param3_1.dat
sleep 1
cat ./param3_1.dat | head -n $((2+1+$degree+1+$degree+2+1)) > ./param3_2.dat
./03_strongconvex -a 0 -d $degree -e 1 -l 0.9 --printparam 1 | tail -n $degree >> ./param3_2.dat
sleep 1
cat ./param3_1.dat | head -n $((2+1+$degree+1+$degree+2+1)) > ./param3_3.dat
./03_strongconvex -a 0 -d $degree -e 1 -l 0.9 --printparam 1 | tail -n $degree >> ./param3_3.dat
sleep 1
cat ./param3_1.dat | head -n $((2+1+$degree+1+$degree+2+1)) > ./param3_4.dat
./03_strongconvex -a 0 -d $degree -e 1 -l 0.9 --printparam 1 | tail -n $degree >> ./param3_4.dat
sleep 1
cat ./param3_1.dat | head -n $((2+1+$degree+1+$degree+2+1)) > ./param3_5.dat
./03_strongconvex -a 0 -d $degree -e 1 -l 0.9 --printparam 1 | tail -n $degree >> ./param3_5.dat

echo "params are genereated."

./03_strongconvex -a 0 -e $error --loadparamfile ./param3_1.dat --debug 1 > ./result3_1.dat
./03_strongconvex -a 0 -e $error --loadparamfile ./param3_2.dat --debug 1 > ./result3_2.dat
./03_strongconvex -a 0 -e $error --loadparamfile ./param3_3.dat --debug 1 > ./result3_3.dat
./03_strongconvex -a 0 -e $error --loadparamfile ./param3_4.dat --debug 1 > ./result3_4.dat
./03_strongconvex -a 0 -e $error --loadparamfile ./param3_5.dat --debug 1 > ./result3_5.dat

echo "results are genereated."

gnuplot -e "
    set xlabel 'k' ;
    set ylabel 'error' ;
    set title 'strong convex' ;
    set logscale y ;
    plot './result3_1.dat' ;
    replot './result3_2.dat' ;
    replot './result3_3.dat' ;
    replot './result3_4.dat' ;
    replot './result3_5.dat' ;
    set terminal png ;
    set output './result3.png' ;
    replot
    "

echo "plot is generated."
