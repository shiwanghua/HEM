sudo g++ -std=c++11 *.h *.cpp -o linuxRB  -O3
echo "Compilation finishes."
ulimit -s unlimited
sudo chmod 777 linuxRB

./linuxRB 136 7

expID=$1

for((bits=0;bits<=8;bits++));  #À¨ºÅ²»ÄÜÊ¡
do  
sudo ./cmakeRB $expID $bits
echo Experiment $expID \done. #$(expr $i \* 3 + 1); 
expID=`expr $expID + 1`
done
