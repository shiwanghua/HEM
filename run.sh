#!/bin/sh

ulimit -s unlimited
sudo chmod 777 cmakeRB
sudo chmod 777 *.txt
#./cmakeRB
expID=$1
m=1000
<<EOF
for((bits=10;bits>=0;bits--));  #À¨ºÅ²»ÄÜÊ¡
do  
./cmakeRB $expID $bits
echo Experiment $expID \done. #$(expr $i \* 3 + 1); 
expID=`expr $expID + 1`
done
EOF

for((i=0;i<10;i++));
do
#./cmakeRB $expID $m
echo Experiment $expID m= $m \done.
expID=`expr $expID + 1`
m =`expr $m - 100`
done
