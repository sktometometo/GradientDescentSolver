#!/bin/sh

./03_strongconvex -a 0 -d 10 -e 1 -l 0.9 --printparam 1 > ./param3_1.dat
sleep 1
cat ./param3_1.dat | head -n 27 > ./param3_2.dat
./03_strongconvex -a 0 -d 10 -e 1 -l 0.9 --printparam 1 | tail -n 10 >> ./param3_2.dat
sleep 1
cat ./param3_1.dat | head -n 27 > ./param3_3.dat
./03_strongconvex -a 0 -d 10 -e 1 -l 0.9 --printparam 1 | tail -n 10 >> ./param3_3.dat
sleep 1
cat ./param3_1.dat | head -n 27 > ./param3_4.dat
./03_strongconvex -a 0 -d 10 -e 1 -l 0.9 --printparam 1 | tail -n 10 >> ./param3_4.dat
sleep 1
cat ./param3_1.dat | head -n 27 > ./param3_5.dat
./03_strongconvex -a 0 -d 10 -e 1 -l 0.9 --printparam 1 | tail -n 10 >> ./param3_5.dat

./03_strongconvex -a 0 -e 1 --loadparamfile ./param3_1.dat --debug 1 > ./result3_1.dat
./03_strongconvex -a 0 -e 1 --loadparamfile ./param3_2.dat --debug 1 > ./result3_2.dat
./03_strongconvex -a 0 -e 1 --loadparamfile ./param3_3.dat --debug 1 > ./result3_3.dat
./03_strongconvex -a 0 -e 1 --loadparamfile ./param3_4.dat --debug 1 > ./result3_4.dat
./03_strongconvex -a 0 -e 1 --loadparamfile ./param3_5.dat --debug 1 > ./result3_5.dat

gnuplot -e "
    set terminal svg ;
    set output 'result3.svg' ;
    set xlabel 'k' ;
    set ylabel 'error' ;
    set title 'strong convex'
    set yscale log ;
    plot './result3_1.dat' ;
    replot './result3_2.dat' ;
    replot './result3_3.dat' ;
    replot './result3_4.dat' ;
    replot './result3_5.dat' ;
    "
